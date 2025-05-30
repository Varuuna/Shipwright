#ifdef ENABLE_REMOTE_CONTROL

#include "CrowdControl.h"
#include "CrowdControlTypes.h"
#include <libultraship/bridge.h>
#include <libultraship/libultraship.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
#include <regex>
#include "soh/OTRGlobals.h"

extern "C" {
#include <z64.h>
#include "variables.h"
#include "functions.h"
#include "macros.h"
extern PlayState* gPlayState;
}

void CrowdControl::Enable() {
    Network::Enable(CVarGetString(CVAR_REMOTE_CROWD_CONTROL("Host"), "127.0.0.1"),
                    CVarGetInteger(CVAR_REMOTE_CROWD_CONTROL("Port"), 43384));
}

void CrowdControl::OnConnected() {
    ccThreadProcess = std::thread(&CrowdControl::ProcessActiveEffects, this);
}

void CrowdControl::OnDisconnected() {
    ccThreadProcess.join();
}

void CrowdControl::OnIncomingJson(nlohmann::json payload) {
    Effect* incomingEffect = ParseMessage(payload);
    if (!incomingEffect) {
        return;
    }

    // If effect is not a timed effect, execute and return result.
    if (!incomingEffect->timeRemaining) {
        EffectResult result = CrowdControl::ExecuteEffect(incomingEffect);
        EmitMessage(incomingEffect->id, incomingEffect->timeRemaining, result);
    } else {
        // If another timed effect is already active that conflicts with the incoming effect.
        bool isConflictingEffectActive = false;
        for (Effect* effect : activeEffects) {
            if (effect != incomingEffect && effect->category == incomingEffect->category &&
                effect->id < incomingEffect->id) {
                isConflictingEffectActive = true;
                EmitMessage(incomingEffect->id, incomingEffect->timeRemaining, EffectResult::Retry);
                break;
            }
        }

        if (!isConflictingEffectActive) {
            // Check if effect can be applied, if it can't, let CC know.
            EffectResult result = CrowdControl::CanApplyEffect(incomingEffect);
            if (result == EffectResult::Retry || result == EffectResult::Failure) {
                EmitMessage(incomingEffect->id, incomingEffect->timeRemaining, result);
                return;
            }

            activeEffectsMutex.lock();
            activeEffects.push_back(incomingEffect);
            activeEffectsMutex.unlock();
        }
    }
}

void CrowdControl::ProcessActiveEffects() {
    while (isEnabled) {
        // We only want to send events when status changes, on start we send Success.
        // If it fails at some point, we send Pause, and when it starts to succeed again we send Success.
        // CC uses this to pause the timer on the overlay.
        activeEffectsMutex.lock();
        auto it = activeEffects.begin();

        while (it != activeEffects.end()) {
            Effect* effect = *it;
            EffectResult result = CrowdControl::ExecuteEffect(effect);

            if (result == EffectResult::Success) {
                // If time remaining has reached 0, we have finished the effect.
                if (effect->timeRemaining <= 0) {
                    it = activeEffects.erase(std::remove(activeEffects.begin(), activeEffects.end(), effect),
                                             activeEffects.end());
                    GameInteractor::RemoveEffect(dynamic_cast<RemovableGameInteractionEffect*>(effect->giEffect));
                    delete effect;
                } else {
                    // If we have a success after previously being paused, tell CC to resume timer.
                    if (effect->isPaused) {
                        effect->isPaused = false;
                        EmitMessage(effect->id, effect->timeRemaining, EffectResult::Resumed);
                        // If not paused before, subtract time from the timer and send a Success event if
                        // the result is different from the last time this was ran.
                        // Timed events are put on a thread that runs once per second.
                    } else {
                        effect->timeRemaining -= 1000;
                        if (result != effect->lastExecutionResult) {
                            effect->lastExecutionResult = result;
                            EmitMessage(effect->id, effect->timeRemaining, EffectResult::Success);
                        }
                    }
                    it++;
                }
            } else { // Timed effects only do Success or Retry
                if (!effect->isPaused && effect->timeRemaining > 0) {
                    effect->isPaused = true;
                    EmitMessage(effect->id, effect->timeRemaining, EffectResult::Paused);
                }
                it++;
            }
        }

        activeEffectsMutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    SPDLOG_TRACE("[CrowdControl] Ending Process thread...");
}

void CrowdControl::EmitMessage(uint32_t eventId, long timeRemaining, EffectResult status) {
    nlohmann::json payload;

    payload["id"] = eventId;
    payload["type"] = 0;
    payload["timeRemaining"] = timeRemaining;
    payload["status"] = status;

    SPDLOG_INFO("[CrowdControl] Sending payload:\n{}", payload.dump());

    SendJsonToRemote(payload);
}

CrowdControl::EffectResult CrowdControl::ExecuteEffect(Effect* effect) {
    GameInteractionEffectQueryResult giResult;
    if (effect->category == kEffectCatSpawnEnemy) {
        giResult = GameInteractor::RawAction::SpawnEnemyWithOffset(effect->spawnParams[0], effect->spawnParams[1]);
    } else if (effect->category == kEffectCatSpawnActor) {
        giResult = GameInteractor::RawAction::SpawnActor(effect->spawnParams[0], effect->spawnParams[1]);
    } else {
        giResult = GameInteractor::ApplyEffect(effect->giEffect);
    }

    return TranslateGiEnum(giResult);
}

/// Checks if effect can be applied -- should not be used to check for spawn enemy effects.
CrowdControl::EffectResult CrowdControl::CanApplyEffect(Effect* effect) {
    assert(effect->category != kEffectCatSpawnEnemy || effect->category != kEffectCatSpawnActor);
    GameInteractionEffectQueryResult giResult = GameInteractor::CanApplyEffect(effect->giEffect);

    return TranslateGiEnum(giResult);
}

CrowdControl::EffectResult CrowdControl::TranslateGiEnum(GameInteractionEffectQueryResult giResult) {
    // Translate GameInteractor result into CC's own enums.
    EffectResult result;
    if (giResult == GameInteractionEffectQueryResult::Possible) {
        result = EffectResult::Success;
    } else if (giResult == GameInteractionEffectQueryResult::TemporarilyNotPossible) {
        result = EffectResult::Retry;
    } else {
        result = EffectResult::Failure;
    }

    return result;
}

CrowdControl::Effect* CrowdControl::ParseMessage(nlohmann::json dataReceived) {
    if (!dataReceived.contains("id") || !dataReceived.contains("type")) {
        SPDLOG_ERROR("[CrowdControl] Invalid payload received:\n{}", dataReceived.dump());
        return nullptr;
    }

    SPDLOG_INFO("[CrowdControl] Received payload:\n{}", dataReceived.dump());

    if (!dataReceived.contains("code")) {
        // This seems to happen when the CC session ends
        SPDLOG_ERROR("[CrowdControl] Payload does not contain code, ignoring.");
        return nullptr;
    }

    Effect* effect = new Effect();
    effect->lastExecutionResult = EffectResult::Initiate;
    effect->id = dataReceived["id"];
    auto parameters = dataReceived["parameters"];
    uint32_t receivedParameter = 0;
    auto effectName = dataReceived["code"].get<std::string>();

    if (parameters.size() > 0) {
        receivedParameter = dataReceived["parameters"][0];
    }

    // Assign GameInteractionEffect + values to CC effect.
    // Categories are mostly used for checking for conflicting timed effects.
    switch (effectStringToEnum[effectName]) {

        // Spawn Enemies and Objects
        case kEffectSpawnCuccoStorm:
            effect->spawnParams[0] = ACTOR_EN_NIW;
            effect->category = kEffectCatSpawnActor;
            break;
        case kEffectSpawnLitBomb:
            effect->spawnParams[0] = ACTOR_EN_BOM;
            effect->category = kEffectCatSpawnActor;
            break;
        case kEffectSpawnExplosion:
            effect->spawnParams[0] = ACTOR_EN_BOM;
            effect->spawnParams[1] = 1;
            effect->category = kEffectCatSpawnActor;
            break;
        case kEffectSpawnArwing:
            effect->spawnParams[0] = ACTOR_EN_CLEAR_TAG;
            // Parameter for no cutscene Arwing
            effect->spawnParams[1] = 1;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnDarklink:
            effect->spawnParams[0] = ACTOR_EN_TORCH2;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnIronKnuckle:
            effect->spawnParams[0] = ACTOR_EN_IK;
            // Parameter for black standing Iron Knuckle
            effect->spawnParams[1] = 2;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnStalfos:
            effect->spawnParams[0] = ACTOR_EN_TEST;
            // Parameter for gravity-obeying Stalfos
            effect->spawnParams[1] = 2;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnFreezard:
            effect->spawnParams[0] = ACTOR_EN_FZ;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnLikeLike:
            effect->spawnParams[0] = ACTOR_EN_RR;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnGibdo:
            effect->spawnParams[0] = ACTOR_EN_RD;
            // Parameter for Gibdo
            effect->spawnParams[1] = 32766;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnKeese:
            effect->spawnParams[0] = ACTOR_EN_FIREFLY;
            // Parameter for normal keese
            effect->spawnParams[1] = 2;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnIceKeese:
            effect->spawnParams[0] = ACTOR_EN_FIREFLY;
            // Parameter for ice keese
            effect->spawnParams[1] = 4;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnFireKeese:
            effect->spawnParams[0] = ACTOR_EN_FIREFLY;
            // Parameter for fire keese
            effect->spawnParams[1] = 1;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnWolfos:
            effect->spawnParams[0] = ACTOR_EN_WF;
            effect->category = kEffectCatSpawnEnemy;
            break;
        case kEffectSpawnWallmaster:
            effect->spawnParams[0] = ACTOR_EN_WALLMAS;
            effect->category = kEffectCatSpawnEnemy;
            break;

        // Link Modifiers
        case kEffectTakeHalfDamage:
            effect->category = kEffectCatDamageTaken;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyDefenseModifier();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 2;
            break;
        case kEffectTakeDoubleDamage:
            effect->category = kEffectCatDamageTaken;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyDefenseModifier();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = -2;
            break;
        case kEffectOneHitKo:
            effect->category = kEffectCatDamageTaken;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::OneHitKO();
            break;
        case kEffectInvincibility:
            effect->category = kEffectCatDamageTaken;
            effect->timeRemaining = 15000;
            effect->giEffect = new GameInteractionEffect::PlayerInvincibility();
            break;
            break;
        case kEffectIncreaseSpeed:
            effect->category = kEffectCatSpeed;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyRunSpeedModifier();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 2;
            break;
        case kEffectDecreaseSpeed:
            effect->category = kEffectCatSpeed;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyRunSpeedModifier();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = -2;
            break;
        case kEffectLowGravity:
            effect->category = kEffectCatGravity;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyGravity();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_GRAVITY_LEVEL_LIGHT;
            break;
        case kEffectHighGravity:
            effect->category = kEffectCatGravity;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyGravity();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_GRAVITY_LEVEL_HEAVY;
            break;
        case kEffectForceIronBoots:
            effect->category = kEffectCatBoots;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ForceEquipBoots();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = EQUIP_VALUE_BOOTS_IRON;
            break;
        case kEffectForceHoverBoots:
            effect->category = kEffectCatBoots;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ForceEquipBoots();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] =
                EQUIP_VALUE_BOOTS_HOVER;
            break;
        case kEffectSlipperyFloor:
            effect->category = kEffectCatSlipperyFloor;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::SlipperyFloor();
            break;
        case kEffectNoLedgeGrabs:
            effect->category = kEffectCatNoLedgeGrabs;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::DisableLedgeGrabs();
            break;
        case kEffectRandomWind:
            effect->category = kEffectCatRandomWind;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::RandomWind();
            break;
        case kEffectRandomBonks:
            effect->category = kEffectCatRandomBonks;
            effect->timeRemaining = 60000;
            effect->giEffect = new GameInteractionEffect::RandomBonks();
            break;

        // Hurt or Heal Link
        case kEffectEmptyHeart:
            effect->giEffect = new GameInteractionEffect::ModifyHealth();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            break;
        case kEffectFillHeart:
            effect->giEffect = new GameInteractionEffect::ModifyHealth();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            break;
        case kEffectKnockbackLinkWeak:
            effect->giEffect = new GameInteractionEffect::KnockbackPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 1;
            break;
        case kEffectKnockbackLinkStrong:
            effect->giEffect = new GameInteractionEffect::KnockbackPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 3;
            break;
        case kEffectKnockbackLinkMega:
            effect->giEffect = new GameInteractionEffect::KnockbackPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 6;
            break;
        case kEffectBurnLink:
            effect->giEffect = new GameInteractionEffect::BurnPlayer();
            break;
        case kEffectFreezeLink:
            effect->giEffect = new GameInteractionEffect::FreezePlayer();
            break;
        case kEffectElectrocuteLink:
            effect->giEffect = new GameInteractionEffect::ElectrocutePlayer();
            break;
        case kEffectKillLink:
            effect->giEffect = new GameInteractionEffect::SetPlayerHealth();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 0;
            break;

        // Give Items and Consumables
        case kEffectAddHeartContainer:
            effect->giEffect = new GameInteractionEffect::ModifyHeartContainers();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 1;
            break;
        case kEffectFillMagic:
            effect->giEffect = new GameInteractionEffect::FillMagic();
            break;
        case kEffectAddRupees:
            effect->giEffect = new GameInteractionEffect::ModifyRupees();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            break;
        case kEffectGiveDekuShield:
            effect->giEffect = new GameInteractionEffect::GiveOrTakeShield();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = ITEM_SHIELD_DEKU;
            break;
        case kEffectGiveHylianShield:
            effect->giEffect = new GameInteractionEffect::GiveOrTakeShield();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = ITEM_SHIELD_HYLIAN;
            break;
        case kEffectRefillSticks:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_STICK;
            break;
        case kEffectRefillNuts:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_NUT;
            break;
        case kEffectRefillBombs:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOMB;
            break;
        case kEffectRefillSeeds:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_SLINGSHOT;
            break;
        case kEffectRefillArrows:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOW;
            break;
        case kEffectRefillBombchus:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOMBCHU;
            break;

        // Take Items and Consumables
        case kEffectRemoveHeartContainer:
            effect->giEffect = new GameInteractionEffect::ModifyHeartContainers();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = -1;
            break;
        case kEffectEmptyMagic:
            effect->giEffect = new GameInteractionEffect::EmptyMagic();
            break;
        case kEffectRemoveRupees:
            effect->giEffect = new GameInteractionEffect::ModifyRupees();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            break;
        case kEffectTakeDekuShield:
            effect->giEffect = new GameInteractionEffect::GiveOrTakeShield();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = -ITEM_SHIELD_DEKU;
            break;
        case kEffectTakeHylianShield:
            effect->giEffect = new GameInteractionEffect::GiveOrTakeShield();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = -ITEM_SHIELD_HYLIAN;
            break;
        case kEffectTakeSticks:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_STICK;
            break;
        case kEffectTakeNuts:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_NUT;
            break;
        case kEffectTakeBombs:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOMB;
            break;
        case kEffectTakeSeeds:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_SLINGSHOT;
            break;
        case kEffectTakeArrows:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOW;
            break;
        case kEffectTakeBombchus:
            effect->giEffect = new GameInteractionEffect::AddOrTakeAmmo();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = receivedParameter * -1;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = ITEM_BOMBCHU;
            break;

        // Link Size Modifiers
        case kEffectGiantLink:
            effect->category = kEffectCatLinkSize;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyLinkSize();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_LINK_SIZE_GIANT;
            break;
        case kEffectMinishLink:
            effect->category = kEffectCatLinkSize;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyLinkSize();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_LINK_SIZE_MINISH;
            break;
        case kEffectPaperLink:
            effect->category = kEffectCatLinkSize;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyLinkSize();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_LINK_SIZE_PAPER;
            break;
        case kEffectSquishedLink:
            effect->category = kEffectCatLinkSize;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::ModifyLinkSize();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_LINK_SIZE_SQUISHED;
            break;
        case kEffectInvisibleLink:
            effect->category = kEffectCatLinkSize;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::InvisibleLink();
            break;

        // Generic Effects
        case kEffectRandomBombTimer:
            effect->category = kEffectCatRandomBombFuseTimer;
            effect->timeRemaining = 60000;
            effect->giEffect = new GameInteractionEffect::RandomBombFuseTimer();
            break;
        case kEffectSetTimeToDawn:
            effect->giEffect = new GameInteractionEffect::SetTimeOfDay();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TIMEOFDAY_DAWN;
            break;
        case kEffectSetTimeToDusk:
            effect->giEffect = new GameInteractionEffect::SetTimeOfDay();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TIMEOFDAY_DUSK;
            break;

        // Visual Effects
        case kEffectNoUi:
            effect->category = kEffectCatUi;
            effect->timeRemaining = 60000;
            effect->giEffect = new GameInteractionEffect::NoUI();
            break;
        case kEffectRainstorm:
            effect->category = kEffectCatWeather;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::WeatherRainstorm();
            break;
        case kEffectDebugMode:
            effect->category = kEffectCatDebugMode;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::SetCollisionViewer();
            break;
        case kEffectRandomCosmetics:
            effect->giEffect = new GameInteractionEffect::RandomizeCosmetics();
            break;

        // Controls
        case kEffectNoZButton:
            effect->category = kEffectCatNoZ;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::DisableZTargeting();
            break;
        case kEffectReverseControls:
            effect->category = kEffectCatReverseControls;
            effect->timeRemaining = 60000;
            effect->giEffect = new GameInteractionEffect::ReverseControls();
            break;
        case kEffectPacifistMode:
            effect->category = kEffectCatPacifist;
            effect->timeRemaining = 15000;
            effect->giEffect = new GameInteractionEffect::PacifistMode();
            break;
        case kEffectPressRandomButtons:
            effect->category = kEffectCatRandomButtons;
            effect->timeRemaining = 30000;
            effect->giEffect = new GameInteractionEffect::PressRandomButton();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = 30;
            break;
        case kEffectClearCbuttons:
            effect->giEffect = new GameInteractionEffect::ClearAssignedButtons();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_BUTTONS_CBUTTONS;
            break;
        case kEffectClearDpad:
            effect->giEffect = new GameInteractionEffect::ClearAssignedButtons();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_BUTTONS_DPAD;
            break;

        // Teleport Player
        case kEffectTpLinksHouse:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_LINKSHOUSE;
            break;
        case kEffectTpMinuet:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_MINUET;
            break;
        case kEffectTpBolero:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_BOLERO;
            break;
        case kEffectTpSerenade:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_SERENADE;
            break;
        case kEffectTpRequiem:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_REQUIEM;
            break;
        case kEffectTpNocturne:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_NOCTURNE;
            break;
        case kEffectTpPrelude:
            effect->giEffect = new GameInteractionEffect::TeleportPlayer();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_TP_DEST_PRELUDE;
            break;

        // Tunic Color (Bidding War)
        case kEffectTunicRed:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_RED;
            break;
        case kEffectTunicGreen:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_GREEN;
            break;
        case kEffectTunicBlue:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLUE;
            break;
        case kEffectTunicOrange:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_ORANGE;
            break;
        case kEffectTunicYellow:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_YELLOW;
            break;
        case kEffectTunicPurple:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PURPLE;
            break;
        case kEffectTunicPink:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PINK;
            break;
        case kEffectTunicBrown:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BROWN;
            break;
        case kEffectTunicBlack:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_TUNICS;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLACK;
            break;

        // Navi Color (Bidding War)
        case kEffectNaviRed:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_RED;
            break;
        case kEffectNaviGreen:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_GREEN;
            break;
        case kEffectNaviBlue:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLUE;
            break;
        case kEffectNaviOrange:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_ORANGE;
            break;
        case kEffectNaviYellow:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_YELLOW;
            break;
        case kEffectNaviPurple:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PURPLE;
            break;
        case kEffectNaviPink:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PINK;
            break;
        case kEffectNaviBrown:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BROWN;
            break;
        case kEffectNaviBlack:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_NAVI;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLACK;
            break;

        // Link's Hair Color (Bidding War)
        case kEffectHairRed:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_RED;
            break;
        case kEffectHairGreen:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_GREEN;
            break;
        case kEffectHairBlue:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLUE;
            break;
        case kEffectHairOrange:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_ORANGE;
            break;
        case kEffectHairYellow:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_YELLOW;
            break;
        case kEffectHairPurple:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PURPLE;
            break;
        case kEffectHairPink:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_PINK;
            break;
        case kEffectHairBrown:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BROWN;
            break;
        case kEffectHairBlack:
            effect->giEffect = new GameInteractionEffect::SetCosmeticsColor();
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[0] = GI_COSMETICS_HAIR;
            dynamic_cast<ParameterizedGameInteractionEffect*>(effect->giEffect)->parameters[1] = GI_COLOR_BLACK;
            break;

        default:
            break;
    }

    return effect;
}
#endif
