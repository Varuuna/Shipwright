#include "custom_messages.hpp"

#include "../randomizerTypes.h"
#include "../context.h"
#include "../static_data.h"

using namespace CustomMessages;
using namespace std::literals::string_literals;

// Big thanks to Lioncache, Gabyelnuevo, Danius88, and Charade for their translations!

// Text is {english, german, french}

// there are special characters that are read for certain in game commands:
// ^ is a box break
// & is a new line
// @ will print the player name
// surrounding text with '#' will make it a different color
// - OoT Randomizer

// '[[d]]' indicates a number will be placed there, numbers in [[]] will be replaced by other text in hints
// '€' at the end of an item name indicates that it's plural.
// '|' separates singular and plural form |singular|plural|.

namespace Rando {

void StaticData::HintTable_Init() {
    // clang-format off
    /*--------------------------
    |       GENERAL TEXT        |
    ---------------------------*/
    hintTextTable[RHT_NONE] = HintText(CustomMessage("No Hint", "Kein Hinweis", "Pas d'Indice" ));
    hintTextTable[RHT_WAY_OF_THE_HERO] =
        HintText(CustomMessage( "They say that #[[1]]# is on #the way of the hero#.", 
                     /*german*/ "Man erzählt sich, daß #[[1]]# #der Weg des Helden# verlaufe.",
                     /*french*/ "Selon moi, #[[1]]# est sur #la voie du héros#.", {QM_RED, QM_LBLUE}));
                 // /*spanish*/ "Según dicen, #[[1]]# conduce a la senda del héroe."
    hintTextTable[RHT_FOOLISH] = 
        HintText(CustomMessage( "They say that plundering #[[1]]# is #a foolish choice#.",
                     /*german*/ "Man erzählt sich, daß es #eine törichte Entscheidung# sei #[[1]]# Schätze zu plündern.",
                     /*french*/ "Selon moi, explorer #[[1]]# est #futile#.", {QM_RED, QM_PINK}));
                 // /*spanish*/ "Según dicen, inspeccionar #[[1]]# #no es una sabia decisión#."
    hintTextTable[RHT_CAN_BE_FOUND_AT] =
        HintText(CustomMessage( "They say that #[[1]]# can be found at #[[2]]#.",
                     /*german*/ "Man erzählt sich, daß #[[1]]# #[[2]]# zu finden |sei|seien|.",
                     /*french*/ "Selon moi, #[[1]]# se trouve dans #[[2]]#.", {QM_GREEN, QM_RED}));
                 // /*spanish*/ "Según dicen, #[[1]]# aguarda en #[[2]]#."
    hintTextTable[RHT_HOARDS] = 
        HintText(CustomMessage( "They say that #[[2]]# hoards #[[1]]#.",
                     /*german*/ "Man erzählt sich, daß #[[2]]# #[[1]]# |horte|horten|.",
                     /*french*/ "Selon moi, #[[2]]# recèle #[[1]]#.", {QM_RED, QM_GREEN}));
                 // /*spanish*/ "Según dicen, #[[2]]# acapara #[[1]]#."
    // clang-format on

    HintTable_Init_Item();
    HintTable_Init_Exclude_Overworld();
    HintTable_Init_Exclude_Dungeon();

    // clang-format off
    /*--------------------------
    |    SOMETIMES HINT TEXT   |
    ---------------------------*/

    hintTextTable[RHT_KF_LINKS_HOUSE_COW] = HintText(CustomMessage("They say that #Malon's obstacle course# leads to #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß das Erzielen eines neuen Rekordes bei #Malons Hindernisparcours# #[[1]]# gewähre.",
                                                        /*french*/ "Selon moi, la #course à obstacle de Malon# amène à #[[1]]#.", {QM_RED, QM_GREEN}),
                                                     // /*spanish*/la #carrera de obstáculos de Malon# brinda #[[1]]#.
                                                     {},
                                                     {CustomMessage("They say that the #bovine bounty of a horseback hustle# gifts #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß der #bovine Schatz einer Pferdehast# #[[1]]# |sei|seien|.",
                                                         /*french*/ "Selon moi, le cadeau #qui découle d'une réussite équestre# est #[[1]]#.", {QM_RED, QM_GREEN})});
                                                      // /*spanish*/la #recompensa bovina de un paseo a caballo# brinda #[[1]]#.

    hintTextTable[RHT_KAK_100_GOLD_SKULLTULA_REWARD] = HintText(CustomMessage("They say that slaying #100 Gold Skulltulas# reveals #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß das Vernichten von #100 Goldenen Skulltulas# #[[1]]# enthülle.",
                                                                   /*french*/ "Selon moi, détruire #100 Skulltulas d'or# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/exterminar #100 skulltulas doradas# revela #[[1]]#.
                                                                {},
                                                                {CustomMessage("They say that #100 bug badges# rewards #[[1]]#.",
                                                                    /*german*/ "Man erzählt sich, daß die Belohnung für #100 Insektenplaketten# #[[1]]# |sei|seien|.",
                                                                    /*french*/ "Selon moi, #100 écussons# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                 // /*spanish*/#100 medallas de insectos# otorgan #[[1]]#.
                                                                 CustomMessage("They say that #100 spider souls# yields #[[1]]#.",
                                                                    /*german*/ "Man erzählt sich, daß #100 Spinnenseelen# #[[1]]# einbrächten.",
                                                                    /*french*/ "Selon moi, #100 âmes d'arachnide# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                 // /*spanish*/#100 almas de araña# otorgan #[[1]]#.
                                                                 CustomMessage("They say that #100 auriferous arachnids# lead to #[[1]]#.",
                                                                    /*german*/ "Man erzählt sich, daß #100 goldhaltige Arachniden# #[[1]]# gewähren.",
                                                                    /*french*/ "Selon moi, #100 arachnides aurifères# donnent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                 // /*spanish*/#100 arácnidos auríferos# otorgan #[[1]]#.

    hintTextTable[RHT_SONG_FROM_OCARINA_OF_TIME] = HintText(CustomMessage("They say that the #Ocarina of Time# teaches #[[1]]#.",
                                                               /*german*/ "Man erzählt sich, daß die #Okarina der Zeit# #[[1]]# lehre.",
                                                               /*french*/ "Selon moi, l'#Ocarina du Temps# est accompagné par #[[1]]#.", {QM_RED, QM_GREEN})),
                                                            // /*spanish*/la #Ocarina del Tiempo# enseña #[[1]]#.

    hintTextTable[RHT_SONG_FROM_ROYAL_FAMILYS_TOMB] = HintText(CustomMessage("They say that #ReDead in the Composers' Grave# guard #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß #Remorts im Grab des Komponisten# #[[1]]# bewachen.",
                                                                  /*french*/ "Selon moi, les #Éffrois du tombeau des compositeurs# protègent #[[1]]#.", {QM_RED, QM_GREEN})),
                                                               // /*spanish*/los #ReDeads del Panteón Real# guardan #[[1]]#.

    hintTextTable[RHT_SHEIK_IN_FOREST] = HintText(CustomMessage("They say that #in a meadow# Sheik teaches #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß Shiek #auf einer Wiese# #[[1]]# lehre.",
                                                     /*french*/ "Selon moi, Sheik confiera, #dans un bosquet#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                  // /*spanish*/#en la pradera sagrada# Sheik enseña #[[1]]#.

    hintTextTable[RHT_SHEIK_AT_TEMPLE] = HintText(CustomMessage("They say that Sheik waits at a #monument to time# to teach #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß Shiek auf einem #Monument der Zeit# warte und #[[1]]# lehre.",
                                                     /*french*/ "Selon moi, Sheik confiera, #au pied de l'épée légendaire#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                  // /*spanish*/Sheik espera en el #momumento del tiempo# para enseñar #[[1]]#.

    hintTextTable[RHT_SHEIK_IN_CRATER] = HintText(CustomMessage("They say that the #crater's melody# is #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß die #Melodie des Kraters# #[[1]]# |sei|seien|.",
                                                     /*french*/ "Selon moi, Sheik confiera, #entouré de lave#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                  // /*spanish*/la #melodía del cráter# otorga #[[1]]#.

    hintTextTable[RHT_SHEIK_IN_ICE_CAVERN] = HintText(CustomMessage("They say that the #frozen cavern# echoes with #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß die #gefrorene Kaverne# #[[1]]# ertönen lasse.",
                                                         /*french*/ "Selon moi, Sheik confiera, #dans une caverne enneigée#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                      // /*spanish*/en la #caverna de hielo# retumban los ecos de #[[1]]#.

    hintTextTable[RHT_SHEIK_IN_KAKARIKO] = HintText(CustomMessage("They say that a #ravaged village# mourns with #[[1]]#.",
                                                       /*german*/ "Man erzählt sich, daß ein #verwüstetes Dorf# um #[[1]]# trauere.",
                                                       /*french*/ "Selon moi, Sheik confirera, #au coeur d'un village ravagé#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                    // /*spanish*/un #arrasado pueblo# llora #[[1]]#.

    hintTextTable[RHT_SHEIK_AT_COLOSSUS] = HintText(CustomMessage("They say that a hero ventures #beyond the wasteland# to learn #[[1]]#.",
                                                       /*german*/ "Man erzählt sich, daß ein Held sich #jenseits des Ödlands# wage und #[[1]]# lerne.",
                                                       /*french*/ "Selon moi, Sheik confiera, #au bout d'un chemin sableux#, #[[1]]#.", {QM_RED, QM_GREEN})),
                                                    // /*spanish*/el héroe que se adentre #más allá del desierto# aprenderá #[[1]]#.

    hintTextTable[RHT_MARKET_10_BIG_POES] = HintText(CustomMessage("They say that catching #Big Poes# leads to #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß die Belohnung für das Fangen von #Nachtschwärmern# #[[1]]# |sei|seien|.",
                                                        /*french*/ "Selon moi, #d'attraper des Àmes# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                     // /*spanish*/hacerte con #Grandes Poes# conduce a #[[1]]#.
                                                     {},
                                                     {CustomMessage("They say that #ghost hunters# will be rewarded with #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß die Belohnung für #Geisterjäger# #[[1]]# |sei|seien|.",
                                                         /*french*/ "Selon moi, #les chasseurs de fantômes# sont récompensés avec #[[1]]#.", {QM_RED, QM_GREEN})});
                                                      // /*spanish*/los #cazafantasmas# son premiados con #[[1]]#.

    hintTextTable[RHT_DEKU_THEATER_SKULL_MASK] = HintText(CustomMessage("They say that the #Skull Mask# yields #[[1]]#.",
                                                             /*german*/ "Man erzählt sich, daß die #Schädel-Maske# #[[1]]# einbrächte.",
                                                             /*french*/ "Selon moi, le #Masque de Mort# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                          // /*spanish*/la #máscara de calavera# otorga #[[1]]#.

    hintTextTable[RHT_DEKU_THEATER_MASK_OF_TRUTH] = HintText(CustomMessage("They say that the #Mask of Truth# yields #[[1]]#.",
                                                                /*german*/ "Man erzählt sich, daß die #Maske der Wahrheit# #[[1]]# einbrächte.",
                                                                /*french*/ "Selon moi, le #Masque de Vérité# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                             // /*spanish*/la #máscara de la verdad# premia #[[1]]#.
                                                             {},
                                                             {CustomMessage("They say that showing a #truthful eye to the crowd# rewards #[[1]]#.",
                                                                 /*german*/ "Man erzählt sich, daß der #Menge ein wahrhaftiges Auge# zu zeigen #[[1]]# einbrächte.",
                                                                 /*french*/ "Selon moi, montrer #l'oeil de vérité à la foule# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                              // /*spanish*/#mostrarle el ojo verdadero# a una multitud brinda #[[1]]#.

    hintTextTable[RHT_HF_OCARINA_OF_TIME_ITEM] = HintText(CustomMessage("They say that the #treasure thrown by Princess Zelda# is #[[1]]#.",
                                                             /*german*/ "Man erzählt sich, daß der #von Prinzessin Zelda geworfene Schatz# #[[1]]# |sei|seien|.",
                                                             /*french*/ "Selon moi, le trésor #laissé par la princesse# est #[[1]]#.", {QM_RED, QM_GREEN})),
                                                          // /*spanish*/el #tesoro arrojado por la Princesa Zelda# se trata de #[[1]]#.

    hintTextTable[RHT_DMT_TRADE_BROKEN_SWORD] = HintText(CustomMessage("They say that a #blinded Biggoron# entrusts #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß ein #erblindeter Biggoron# #[[1]]# anvertraue.",
                                                            /*french*/ "Selon moi, They say that un #Grogoron aveuglé# confie #[[1]]#.", {QM_RED, QM_GREEN})),
                                                         // /*spanish*/un #miope Biggoron# otorga #[[1]]#.

    hintTextTable[RHT_DMT_TRADE_EYEDROPS] = HintText(CustomMessage("They say that while you wait, #Biggoron# gives #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß während man warte, #Biggoron# #[[1]]# gäbe.",
                                                        /*french*/ "Selon moi, pendant que tu attends, #Biggoron# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                     // /*spanish*/#Biggoron# está a la espera de otorgar #[[1]]#.

    hintTextTable[RHT_DMT_TRADE_CLAIM_CHECK] = HintText(CustomMessage("They say that #Biggoron# crafts #[[1]]#.",
                                                           /*german*/ "Man erzählt sich, daß #Biggoron# #[[1]]# fertige.",
                                                           /*french*/ "Selon moi, #Biggoron# fabrique #[[1]]#.", {QM_RED, QM_GREEN})),
                                                        // /*spanish*/#Biggoron# forja #[[1]]#.

    hintTextTable[RHT_KAK_50_GOLD_SKULLTULA_REWARD] = HintText(CustomMessage("They say that slaying #50 Gold Skulltulas# reveals #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß das Vernichten von #50 Goldenen Skulltulas# #[[1]]# enthülle.",
                                                                  /*french*/ "Selon moi, détruire #50 Skulltulas d'or# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                               // /*spanish*/exterminar #50 skulltulas doradas# revela #[[1]]#.
                                                               {},
                                                               {CustomMessage("They say that #50 bug badges# rewards #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß die Belohnung für #50 Insektenplaketten# #[[1]]# |sei|seien|.",
                                                                   /*french*/ "Selon moi, #50 écussons# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#50 medallas de insectos# otorgan #[[1]]#.
                                                                CustomMessage("They say that #50 spider souls# yields #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #50 Spinnenseelen# #[[1]]# einbrächte.",
                                                                   /*french*/ "Selon moi, #50 âmes d'arachnide# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#50 almas de araña# otorgan #[[1]]#.
                                                                CustomMessage("They say that #50 auriferous arachnids# lead to #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #50 goldhaltige Arachniden# #[[1]]# gewähren.",
                                                                   /*french*/ "Selon moi, #50 arachnides aurifères# donnent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                // /*spanish*/#50 arácnidos auríferos# otorgan #[[1]]#.

    hintTextTable[RHT_KAK_40_GOLD_SKULLTULA_REWARD] = HintText(CustomMessage("They say that slaying #40 Gold Skulltulas# reveals #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß das Vernichten von #40 Goldenen Skulltulas# #[[1]]# enthülle.",
                                                                  /*french*/ "Selon moi, détruire #40 Skulltulas d'or# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                               // /*spanish*/exterminar #40 skulltulas doradas# revela #[[1]]#.
                                                               {},
                                                               {CustomMessage("They say that #40 bug badges# rewards #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß die Belohnung für #40 Insektenplaketten# #[[1]]# |sei|seien|.",
                                                                   /*french*/ "Selon moi, #40 écussons# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#40 medallas de insectos# otorgan #[[1]]#.
                                                                CustomMessage("They say that #40 spider souls# yields #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #40 Spinnenseelen# #[[1]]# einbrächte.",
                                                                   /*french*/ "Selon moi, #40 âmes d'arachnide# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#40 almas de araña# otorgan #[[1]]#.
                                                                CustomMessage("They say that #40 auriferous arachnids# lead to #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #40 goldhaltige Arachniden# #[[1]]# gewähren.",
                                                                   /*french*/ "Selon moi, #40 arachnides aurifères# donnent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                // /*spanish*/#40 arácnidos auríferos# otorgan #[[1]]#.

    hintTextTable[RHT_KAK_30_GOLD_SKULLTULA_REWARD] = HintText(CustomMessage("They say that slaying #30 Gold Skulltulas# reveals #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß das Vernichten von #30 Goldenen Skulltulas# #[[1]]# enthülle.",
                                                                  /*french*/ "Selon moi, détruire #30 Skulltulas d'or# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                               // /*spanish*/exterminar #30 skulltulas doradas# revela #[[1]]#.
                                                               {},
                                                               {CustomMessage("They say that #30 bug badges# rewards #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß die Belohnung für #30 Insektenplaketten# #[[1]]# |sei|seien|.",
                                                                   /*french*/ "Selon moi, #30 écussons# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#30 medallas de insectos# otorgan #[[1]]#.
                                                                CustomMessage("They say that #30 spider souls# yields #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #30 Spinnenseelen# #[[1]]# einbrächte.",
                                                                   /*french*/ "Selon moi, #30 âmes d'arachnide# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#30 almas de araña# otorgan #[[1]]#.
                                                                CustomMessage("They say that #30 auriferous arachnids# lead to #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #30 goldhaltige Arachniden# #[[1]]# gewähren.",
                                                                   /*french*/ "Selon moi, #30 arachnides aurifères# donnent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                // /*spanish*/#30 arácnidos auríferos# otorgan #[[1]]#.

    hintTextTable[RHT_KAK_20_GOLD_SKULLTULA_REWARD] = HintText(CustomMessage("They say that slaying #20 Gold Skulltulas# reveals #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß das Vernichten von #20 Goldenen Skulltulas# #[[1]]# enthülle.",
                                                                  /*french*/ "Selon moi, détruire #20 Skulltulas d'or# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                               // /*spanish*/exterminar #20 skulltulas doradas# revela #[[1]]#.
                                                               {},
                                                               {CustomMessage("They say that #20 bug badges# rewards #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß die Belohnung für #20 Insektenplaketten# #[[1]]# |sei|seien|.",
                                                                   /*french*/ "Selon moi, #20 écussons# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#20 medallas de insectos# otorgan #[[1]]#.
                                                                CustomMessage("They say that #20 spider souls# yields #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #20 Spinnenseelen# #[[1]]# einbrächte.",
                                                                   /*french*/ "Selon moi, #20 âmes d'arachnide# donnent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                // /*spanish*/#20 almas de araña# otorgan #[[1]]#.
                                                                CustomMessage("They say that #20 auriferous arachnids# lead to #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß #20 goldhaltige Arachniden# #[[1]]# gewähren.",
                                                                   /*french*/ "Selon moi, #20 arachnides aurifères# donnent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                // /*spanish*/#20 arácnidos auríferos# otorgan #[[1]]#.

    hintTextTable[RHT_KAK_ANJU_AS_CHILD] = HintText(CustomMessage("They say that #collecting cuccos# rewards #[[1]]#.",
                                                       /*german*/ "Man erzählt sich, daß das #Sammeln von Hühnern# #[[1]]# einbrächte.",
                                                       /*french*/ "Selon moi, #rapporter les Cocottes# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                    // /*spanish*/#hacerte con todos los cucos# premia #[[1]]#.
                                                    {},
                                                    {CustomMessage("They say that #wrangling roosters# rewards #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß das #Rangeln mit Hähnen# #[[1]]# einbrächte.",
                                                        /*french*/ "Selon moi, #plumer des poulets# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                     // /*spanish*/#atrapar a las gallinas# premia #[[1]]#.
                                                     CustomMessage("They say that #chucking chickens# gifts #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß das #Werfen von Hühnchen# #[[1]]# einbrächte.",
                                                        /*french*/ "Selon moi, #lancer des poulets# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                     // /*spanish*/#reunir a unos emplumados# premia #[[1]]#.

    hintTextTable[RHT_KAK_TRADE_POCKET_CUCCO] = HintText(CustomMessage("They say that an adult's #happy Cucco# awards #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß das #glückliche Huhn# eines Erwachsenen #[[1]]# verleihe.",
                                                            /*french*/ "Selon moi, un adulte avec une #poulette joyeuse# obtient #[[1]]#.", {QM_RED, QM_GREEN})),
                                                         // /*spanish*/un #alegre cuco# en la madurez otorga #[[1]]#.

    hintTextTable[RHT_KAK_TRADE_ODD_MUSHROOM] = HintText(CustomMessage("They say that the #potion shop lady# entrusts #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß die #Dame des Hexenladens# #[[1]]# anvertraue.",
                                                            /*french*/ "Selon moi, la #gribiche du magasin de potion# confie #[[1]]#.", {QM_RED, QM_GREEN})),
                                                         // /*spanish*/la #señora de la tienda de pociones# otorga #[[1]]#.

    hintTextTable[RHT_GC_DARUNIAS_JOY] = HintText(CustomMessage("They say that #Darunia's dance# leads to #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß #Darunias Tanz# #[[1]]# gewähre.",
                                                     /*french*/ "Selon moi, #la dance de Darunia# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                  // /*spanish*/#el baile de Darunia# conduce a #[[1]]#.
                                                  {},
                                                  {CustomMessage("They say that a #groovin' goron# gifts #[[1]]#.",
                                                      /*german*/ "Man erzählt sich, daß ein #grooviger Gorone# #[[1]]# schenke.",
                                                      /*french*/ "Selon moi, #le Goron joyeux# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                   // /*spanish*/#un goron marchoso# otorga #[[1]]#.

    hintTextTable[RHT_LW_SKULL_KID] = HintText(CustomMessage("They say that the #Skull Kid# grants #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß das #Horror-Kid# #[[1]]# gewähre.",
                                                  /*french*/ "Selon moi, le #Skull Kid# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                               // /*spanish*/#Skull Kid# otorga #[[1]]#.

    hintTextTable[RHT_LW_TRADE_COJIRO] = HintText(CustomMessage("They say that returning a #special Cucco# awards #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß das Zurückbringen eines #speziellen Huhns# #[[1]]# einbrächte.",
                                                     /*french*/ "Selon moi, ramener une #poulette précieuse# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                  // /*spanish*/quien devuelva un #cuco especial# encontrará #[[1]]#.

    hintTextTable[RHT_LW_TRADE_ODD_POTION] = HintText(CustomMessage("They say that a #Kokiri girl in the woods# leaves #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß ein #Kokiri-Mädchen in den Wäldern# #[[1]]# überließe.",
                                                         /*french*/ "Selon moi, la #fillette Kokiri dans les bois# laisse #[[1]]#.", {QM_RED, QM_GREEN})),
                                                      // /*spanish*/una #chica kokiri del bosque# otorga #[[1]]#.

    hintTextTable[RHT_LH_SUN] = HintText(CustomMessage("They say that shooting #the sun# grants #[[1]]#.",
                                            /*german*/ "Man erzählt sich, daß das Beschießen #der Sonne# #[[1]]# gewähre.",
                                            /*french*/ "Selon moi, tirer une flèche dans #sur le soleil# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                         // /*spanish*/#disparar al sol# revela #[[1]]#.
                                         {},
                                         {CustomMessage("They say that staring into #the sun# grants #[[1]]#.",
                                             /*german*/ "Man erzählt sich, daß das Starren in #die Sonne# #[[1]]# gewähre.",
                                             /*french*/ "Selon moi, regarder #le soleil# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                          // /*spanish*/#mirar al sol# revela #[[1]]#.

    hintTextTable[RHT_LH_TRADE_FROG] = HintText(CustomMessage("They say that #Lake Hylia's scientist# hurriedly entrusts #[[1]]#.",
                                                   /*german*/ "Man erzählt sich, daß der #Wissenschaftler des Hylia-Sees# #[[1]]# zügig anvertraue.",
                                                   /*french*/ "Selon moi, le #scientifique du lac# confie rapidement #[[1]]#.", {QM_RED, QM_GREEN})),
                                                // /*spanish*/el #científico del Lago Hylia# otorga con prisa #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_REWARD] = HintText(CustomMessage("They say that the #treasure chest game# grants #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß die #Truhenlotterie# #[[1]]# gewähre.",
                                                                       /*french*/ "Selon moi, la #Chasse-aux-Trésors# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#el Cofre del Tesoro# premia #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#los juegos de azar# revelan #[[1]]#.
                                                                     CustomMessage("They say that there is a #1/32 chance# to win #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß es eine #Chance von 1 zu 32# gäbe um #[[1]]# zu gewinnen.",
                                                                        /*french*/ "Selon moi, être #le gagnant parmi 32# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/hay una #probabilidad de 1/32# de ganar #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_ITEM_1] = HintText(CustomMessage("They say that the #first locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #erste verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #première salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la primera sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling once# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #einmaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier une fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar solo una vez# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #first or second game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #erste oder zweite Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #premier ou deuxième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el primer o segundo cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_ITEM_2] = HintText(CustomMessage("They say that the #second locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #zweite verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #deuxième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la segunda sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling twice# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #zweimaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier deux fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar dos veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #third or fourth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #dritte oder vierte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #troisième ou quatrième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el tercer o cuarto cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_ITEM_3] = HintText(CustomMessage("They say that the #third locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #dritte verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #troisième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la tercera sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 3 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #dreimaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier trois fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar tres veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #fifth or sixth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #fünfte oder sechste Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #cinquième ou sixième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el quinto o sexto cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_ITEM_4] = HintText(CustomMessage("They say that the #fourth locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #vierte verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #quatrième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la cuarta sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 4 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #viermaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier quatre fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar cuatro veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #seventh or eighth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #siebente oder achte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #septième ou huitième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el séptimo u octavo cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_ITEM_5] = HintText(CustomMessage("They say that the #fifth locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #fünfte verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #cinquième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la quinta sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 5 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #fünfmaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier cinq fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar cinco veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #ninth or tenth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #neunte oder zehnte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #neuvième ou dixième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el noveno o décimo cofre del azar# revela #[[1]]#.

   hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_KEY_1] = HintText(CustomMessage("They say that the #first locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #erste verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #première salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la primera sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling once# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #einmaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier une fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar solo una vez# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #first or second game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #erste oder zweite Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #premier ou deuxième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el primer o segundo cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_KEY_2] = HintText(CustomMessage("They say that the #second locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #zweite verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #deuxième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la segunda sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling twice# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #zweimaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier deux fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar dos veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #third or fourth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #dritte oder vierte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #troisième ou quatrième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el tercer o cuarto cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_KEY_3] = HintText(CustomMessage("They say that the #third locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #dritte verschloßene Raum# in der Truhenlotterie, #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #troisième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la tercera sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 3 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #dreimaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier trois fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar tres veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #fifth or sixth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #fünfte oder sechste Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #cinquième ou sixième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el quinto o sexto cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_KEY_4] = HintText(CustomMessage("They say that the #fourth locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #vierte verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #quatrième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la cuarta sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 4 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #viermaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier quatre fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar cuatro veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #seventh or eighth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #siebente oder achte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #septième ou huitième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el séptimo u octavo cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME_KEY_5] = HintText(CustomMessage("They say that the #fifth locked room# in the chest game contains #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #fünfte verschloßene Raum# in der Truhenlotterie #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, la #cinquième salle# de la Chasse-aux-Trésors contient #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/#en la quinta sala del Cofre del Tesoro# aguarda #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that #gambling 5 times# grants #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß #fünfmaliges Glücksspiel# #[[1]]# gewähre.",
                                                                        /*french*/ "Selon moi, #parier cinq fois# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                     // /*spanish*/#apostar cinco veces# revelará #[[1]]#.
                                                                     CustomMessage("They say that the #ninth or tenth game chest# contains #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #neunte oder zehnte Spieltruhe# #[[1]]# enthielte.",
                                                                        /*french*/ "Selon moi, le #neuvième ou dixième coffre à jeu# contient #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/#el noveno o décimo cofre del azar# revela #[[1]]#.

    hintTextTable[RHT_GF_HBA_1500_POINTS] = HintText(CustomMessage("They say that scoring 1500 in #horseback archery# grants #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß das Erzielen von 1.500 Punkten beim #Bogenschießen zu Pferde# #[[1]]# gewähre.",
                                                        /*french*/ "Selon moi, obtenir 1500 points dans l'#archerie équestre# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                     // /*spanish*/conseguir 1500 puntos en el #tiro con arco a caballo# premia #[[1]]#.
                                                     {},
                                                     {CustomMessage("They say that mastery of #horseback archery# grants #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß das Meistern des #Bogenschießens zu Pferde# #[[1]]# gewähre.",
                                                         /*french*/ "Selon moi, maîtriser l'#archerie équestre# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                      // /*spanish*/dominar el #tiro con arco a caballo# premia con #[[1]]#.

    hintTextTable[RHT_GRAVEYARD_HEART_PIECE_GRAVE_CHEST] = HintText(CustomMessage("They say that playing #Sun's Song# in a grave spawns #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß das Spielen der #Hymne der Sonne# in einem Grab #[[1]]# erscheinen ließe.",
                                                                       /*french*/ "Selon moi, jouer le #chant du soleil# dans un tombeau donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                    // /*spanish*/#tocar la Canción del Sol# en una cripta conduce a #[[1]]#.

    hintTextTable[RHT_GC_MAZE_LEFT_CHEST] = HintText(CustomMessage("They say that in #Goron City# the hammer unlocks #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß der Stahlhammer in #Goronia# #[[1]]# freilege.",
                                                        /*french*/ "Selon moi, dans le #village Goron#, le marteau donne accès à #[[1]]#.", {QM_RED, QM_GREEN})),
                                                     // /*spanish*/en la #Ciudad Goron# el martillo desbloquea #[[1]]#.

    hintTextTable[RHT_GV_CHEST] = HintText(CustomMessage("They say that in #Gerudo Valley# the hammer unlocks #[[1]]#.",
                                              /*german*/ "Man erzählt sich, daß der Stahlhammer im #Gerudotal# #[[1]]# freilege.",
                                              /*french*/ "Selon moi, dans la #Vallée Gerudo#, le marteau donne accès à #[[1]]#.", {QM_RED, QM_GREEN})),
                                           // /*spanish*/en el #Valle Gerudo# el martillo desbloquea #[[1]]#.

    hintTextTable[RHT_GV_TRADE_SAW] = HintText(CustomMessage("They say that the #boss of the carpenters# leaves #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß der #Chef der Zimmerleute# #[[1]]# hinterließe.",
                                                  /*french*/ "Selon moi, le #patron des ouvriers# laisse #[[1]]#.", {QM_RED, QM_GREEN})),
                                               // /*spanish*/el #capataz de los carpinteros# otorga #[[1]]#.

    hintTextTable[RHT_GV_COW] = HintText(CustomMessage("They say that a #cow in Gerudo Valley# gifts #[[1]]#.",
                                            /*german*/ "Man erzählt sich, daß eine #Kuh im Gerudotal# #[[1]]# schenke.",
                                            /*french*/ "Selon moi, la #vache de la Vallée Gerudo# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                         // /*spanish*/una #vaca del Valle Gerudo# brinda #[[1]]#.

    hintTextTable[RHT_HC_GS_STORMS_GROTTO] = HintText(CustomMessage("They say that a #spider behind a muddy wall# in a grotto holds #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß eine #Spinne hinter einer schlammigen Wand# in einer Grotte #[[1]]# hielte.",
                                                         /*french*/ "Selon moi, l'#araignée derrière un mur de boue# dans une grotte donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                      // /*spanish*/una #Skulltula tras la agrietada pared# de una cueva otorga #[[1]]#.

    hintTextTable[RHT_HF_GS_COW_GROTTO] = HintText(CustomMessage("They say that a #spider behind webs# in a grotto holds #[[1]]#.",
                                                      /*german*/ "Man erzählt sich, daß eine #Spinne hinter Weben# in einer Grotte #[[1]]# hielte.",
                                                      /*french*/ "Selon moi, l'#araignée derrière une toile# dans une grotte donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                   // /*spanish*/una #Skulltula tras la telaraña# de una cueva otorga #[[1]]#.

    hintTextTable[RHT_HF_COW_GROTTO_COW] = HintText(CustomMessage("They say that a #cow behind webs# in a grotto gifts #[[1]]#.",
                                                       /*german*/ "Man erzählt sich, daß eine #Kuh, hinter Weben# in einer Grotte, #[[1]]# hielte.",
                                                       /*french*/ "Selon moi, la #vache derrière les toiles# d'une grotte donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                    // /*spanish*/una #vaca tras la telaraña# de una cueva brinda #[[1]]#.
                                                    {},
                                                    {CustomMessage("They say that the #cobwebbed cow# gifts #[[1]]#.",
                                                        /*german*/ "Man erzählt sich, daß die #in Spinnweben eingehüllte Kuh# #[[1]]# schenke.",
                                                        /*french*/ "Selon moi, la #vache prisonnière d'araignées# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                     // /*spanish*/una #vaca tras una telaraña# brinda #[[1]]#.

    hintTextTable[RHT_ZR_FROGS_OCARINA_GAME] = HintText(CustomMessage("They say that the final reward from the #Frogs of Zora's River# is #[[1]]#.",
                                                           /*german*/ "Man erzählt sich, daß die letzte Belohnung der #Frösche des Zora-Flußes# #[[1]]# |sei|seien|.",
                                                           /*french*/ "Selon moi, la dernière récompense des #grenouilles de la Rivière Zora# est #[[1]]#.", {QM_RED, QM_GREEN}),
                                                        // /*spanish*/la recompensa final de las #ranas del Río Zora# premia #[[1]]#.
                                                        {},
                                                        {CustomMessage("They say that an #amphibian feast# yields #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß ein #amphibisches Bankett# #[[1]]# einbrächte.",
                                                            /*french*/ "Selon moi, un #festin d'amphibiens# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                         // /*spanish*/una #fiesta anfibia# brinda #[[1]]#.
                                                         CustomMessage("They say that the #croaking choir's magnum opus# awards #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß das #Opus magnum eines quakenden Chors# #[[1]]# verleihe.",
                                                            /*french*/ "Selon moi, la #chorale coassante# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                         // /*spanish*/un #coro maestro de ancas# premia #[[1]]#.
                                                         CustomMessage("They say that the #froggy finale# yields #[[1]]#.",
                                                            /*german*/ "Man erzählt sich, daß das #froschige Finale# #[[1]]# einbrächte.",
                                                            /*french*/ "Selon moi, la #finale amphibienne# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                         // /*spanish*/el #gran final batracio# brinda #[[1]]#.

    hintTextTable[RHT_ZF_GS_HIDDEN_CAVE] = HintText(CustomMessage("They say that a spider high #above the icy waters# holds #[[1]]#.",
                                                       /*german*/ "Man erzählt sich, daß eine Spinne hoch #auf den eisigen Gewässern# #[[1]]# hielte.",
                                                       /*french*/ "Selon moi, l'araignée #en haut des eaux glacées# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                                    // /*spanish*/una Skulltula en lo #alto de las congeladas aguas# otorga #[[1]]#.

    hintTextTable[RHT_WASTELAND_CHEST] = HintText(CustomMessage("They say that #deep in the wasteland# is #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß #tief im Ödland# #[[1]]# |sei|seien|.",
                                                     /*french*/ "Selon moi, #loin dans le désert# gît #[[1]]#.", {QM_RED, QM_GREEN})),
                                                  // /*spanish*/en lo #profundo del desierto encantado# yace #[[1]]#.

    hintTextTable[RHT_WASTELAND_GS] = HintText(CustomMessage("They say that a #spider in the wasteland# holds #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß eine #Spinne im Ödland# #[[1]]# hielte.",
                                                  /*french*/ "Selon moi, #l'araignée dans le désert# donne #[[1]]#.", {QM_RED, QM_GREEN})),
                                               // /*spanish*/una #Skulltula del desierto encantado# otorga #[[1]]#.

    hintTextTable[RHT_GRAVEYARD_ROYAL_FAMILYS_TOMB_CHEST] = HintText(CustomMessage("They say that #flames in the Composers' Grave# reveal #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß die #Flammen im Grab des Komponisten# #[[1]]# enthüllen würden.",
                                                                        /*french*/ "Selon moi, #les flammes dans le tombeau des compositeurs# cachent #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                     // /*spanish*/#las llamas del Panteón Real# revelan #[[1]]#.

    hintTextTable[RHT_ZF_BOTTOM_FREESTANDING_POH] = HintText(CustomMessage("They say that #under the icy waters# lies #[[1]]#.",
                                                                /*german*/ "Man erzählt sich, daß #unter den eisigen Gewässern# #[[1]]# |läge|lägen|.",
                                                                /*french*/ "Selon moi, #sous les eaux glacées# se cache #[[1]]#.", {QM_RED, QM_GREEN})),
                                                             // /*spanish*/#bajo las congeladas aguas# yace #[[1]]#.

    hintTextTable[RHT_GC_POT_FREESTANDING_POH] = HintText(CustomMessage("They say that spinning #Goron pottery# contains #[[1]]#.",
                                                             /*german*/ "Man erzählt sich, daß die drehende #Goronenkeramik# #[[1]]# enthielte.",
                                                             /*french*/ "Selon moi, la #potterie Goron# contient #[[1]]#.", {QM_RED, QM_GREEN})),
                                                          // /*spanish*/una #cerámica goron# contiene #[[1]]#.

    hintTextTable[RHT_ZD_KING_ZORA_THAWED] = HintText(CustomMessage("They say that unfreezing #King Zora# grants #[[1]]#.",
                                                         /*german*/ "Man erzählt sich, daß das Auftauen #König Zoras# #[[1]]# gewähre.",
                                                         /*french*/ "Selon moi, dégeler #le Roi Zora# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                      // /*spanish*/#descongelar al Rey Zora# conduce a #[[1]]#.
                                                      {},
                                                      {CustomMessage("They say that a #defrosted dignitary# gifts #[[1]]#.",
                                                          /*german*/ "Man erzählt sich, daß ein #enteister Würdenträger# #[[1]]# schenke.",
                                                          /*french*/ "Selon moi, le #monarque libéré# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                       // /*spanish*/una #liberación monárquica# brinda #[[1]]#.

    hintTextTable[RHT_ZD_TRADE_PRESCRIPTION] = HintText(CustomMessage("They say that #King Zora# hurriedly entrusts #[[1]]#.",
                                                           /*german*/ "Man erzählt sich, daß #König Zora# #[[1]]# zügig anvertraue.",
                                                           /*french*/ "Selon moi, le #roi Zora# confie rapidement #[[1]]#.", {QM_RED, QM_GREEN})),
                                                        // /*spanish*/el #Rey Zora# otorga con prisa #[[1]]#.

    hintTextTable[RHT_DMC_DEKU_SCRUB] = HintText(CustomMessage("They say that a single #scrub in the crater# sells #[[1]]#.",
                                                    /*german*/ "Man erzählt sich, daß ein einzelner #Deku-Händler im Krater# #[[1]]# verkaufe.",
                                                    /*french*/ "Selon moi, la #peste Mojo dans le cratère# vend #[[1]]#.", {QM_RED, QM_GREEN})),
                                                 // /*spanish*/un solitario #deku del cráter# vende #[[1]]#.
    hintTextTable[RHT_DMC_GS_CRATE] = HintText(CustomMessage("They say that a spider under a #crate in the crater# holds #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß eine Spinne unter einer #Kiste im Krater# #[[1]]# hielte.",
                                                  /*french*/ "Selon moi, la Skulltula dans une #boîte volcanique# a #[[1]]#.", {QM_RED, QM_GREEN})),
                                               // /*spanish*/una Skulltula bajo una #caja del cráter# otorga #[[1]]#.

    hintTextTable[RHT_DEKU_TREE_MQ_AFTER_SPINNING_LOG_CHEST] = HintText(CustomMessage("They say that a #temporal stone within the Deku Tree# contains #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß ein #zeitlicher Stein innerhalb des Deku-Baumes# #[[1]]# enthielte.",
                                                                           /*french*/ "Selon moi, la #pierre temporelle dans l'Arbre Mojo# cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                        // /*spanish*/un #bloque temporal del Gran Árbol Deku# contiene #[[1]]#.
                                                                        {},
                                                                        {CustomMessage("They say that a #temporal stone within a tree# contains #[[1]]#.",
                                                                            /*german*/ "Man erzählt sich, daß ein #zeitlicher Stein innerhalb eines Baumes# #[[1]]# enthielte.",
                                                                            /*french*/ "Selon moi, la #pierre bleue dans un arbre# mène à #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                         // /*spanish*/un #bloque temporal de un árbol# contiene #[[1]]#.

    hintTextTable[RHT_DEKU_TREE_MQ_GS_BASEMENT_GRAVES_ROOM] = HintText(CustomMessage("They say that a #spider on a ceiling in the Deku Tree# holds #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß eine #Spinne an einer Decke im Deku-Baum# #[[1]]# hielte.",
                                                                          /*french*/ "Selon moi, la #Skulltula dans le Cimetière de l'Arbre Mojo# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                       // /*spanish*/una #Skulltula en el techo del Gran Árbol Deku# otorga #[[1]]#.
                                                                       {},
                                                                       {CustomMessage("They say that a #spider on a ceiling in a tree# holds #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß eine #Spinne an einer Decke in einem Baum# #[[1]]# hielte.",
                                                                           /*french*/ "Selon moi, l'#araignée haut-perchée dans un arbre# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                        // /*spanish*/una #Skulltula en el techo de un árbol# otorga #[[1]]#.

    hintTextTable[RHT_DODONGOS_CAVERN_MQ_GS_SONG_OF_TIME_BLOCK_ROOM] = HintText(CustomMessage("They say that a spider under #temporal stones in Dodongo's Cavern# holds #[[1]]#.",
                                                                                   /*german*/ "Man erzählt sich, daß eine Spinne unter #zeitlichen Steinen in Dodongos Höhle# #[[1]]# hielte.",
                                                                                   /*french*/ "Selon moi, la Skulltula sous #la pierre temporelle dans la Caverne Dodongo# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                                // /*spanish*/una Skulltula bajo #bloques temporales de la Cueva de los Dodongos# otorga #[[1]]#.
                                                                                {},
                                                                                {CustomMessage("They say that a spider under #temporal stones in a cavern# holds #[[1]]#.",
                                                                                    /*german*/ "Man erzählt sich, daß eine Spinne unter #zeitlichen Steinen in einer Kaverne# #[[1]]# hielte.",
                                                                                    /*french*/ "Selon moi, l'araignée sous #une pierre bleue dans une caverne# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                                 // /*spanish*/una Skulltula bajo #bloques temporales de una cueva# otorga #[[1]]#.

    hintTextTable[RHT_JABU_JABUS_BELLY_BOOMERANG_CHEST] = HintText(CustomMessage("They say that a school of #stingers swallowed by Jabu-Jabu# guard #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß ein #von Jabu-Jabu verschluckter Rochenschwarm# #[[1]]# bewache.",
                                                                      /*french*/ "Selon moi, les #raies dans Jabu-Jabu# protègent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                   // /*spanish*/unos #stingers engullidos por Jabu-Jabu# guardan #[[1]]#.
                                                                   {},
                                                                   {CustomMessage("They say that a school of #stingers swallowed by a deity# guard #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß ein #von einer Gottheit verschluckter Rochenschwarm# #[[1]]# bewache.",
                                                                       /*french*/ "Selon moi, les #raies dans un gardien# protègent #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                    // /*spanish*/unos de #stingers engullidos por cierta deidad# guardan #[[1]]#.

    hintTextTable[RHT_JABU_JABUS_BELLY_MQ_GS_INVISIBLE_ENEMIES_ROOM] = HintText(CustomMessage("They say that a spider surrounded by #shadows in Jabu-Jabu's Belly# holds #[[1]]#.",
                                                                                   /*german*/ "Man erzählt sich, daß eine von #Schatten umgebene Spinne in Jabu-Jabus Bauch# #[[1]]# hielte.",
                                                                                   /*french*/ "Selon moi, la Skulltula entourée d'#ombres dans Jabu-Jabu# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                                // /*spanish*/una Skulltula rodeada de #sombras en la Tripa de Jabu-Jabu# otorga #[[1]]#.
                                                                                {},
                                                                                {CustomMessage("They say that a spider surrounded by #shadows in the belly of a deity# holds #[[1]]#.",
                                                                                    /*german*/ "Man erzählt sich, daß eine von #Schatten umgebene Spinne im Bauche einer Gottheit#, #[[1]]# hielte.",
                                                                                    /*french*/ "Selon moi, l'araignée entourée d'#ombres dans le ventre du gardien# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                                 // /*spanish*/una Skulltula rodeada de #sombras en la tripa de cierta diedad# otorga #[[1]]#.

    hintTextTable[RHT_JABU_JABUS_BELLY_MQ_COW] = HintText(CustomMessage("They say that a #cow swallowed by Jabu-Jabu# gifts #[[1]]#.",
                                                             /*german*/ "Man erzählt sich, daß eine #von Jabu-Jabu verschluckte Kuh# #[[1]]# schenke.",
                                                             /*french*/ "Selon moi, la #vache avallée par Jabu-Jabu# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                          // /*spanish*/una #vaca engullida por Jabu-Jabu# brinda #[[1]]#.
                                                          {},
                                                          {CustomMessage("They say that a #cow swallowed by a deity# gifts #[[1]]#.",
                                                              /*german*/ "Man erzählt sich, daß eine #von einer Gottheit verschluckte Kuh# #[[1]]# schenke.",
                                                              /*french*/ "Selon moi, la #vache dans le gardien# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                           // /*spanish*/una #vaca engullida por cierta deidad# brinda #[[1]]#.

    hintTextTable[RHT_FIRE_TEMPLE_SCARECROW_CHEST] = HintText(CustomMessage("They say that #Pierre atop the Fire Temple# hides #[[1]]#.",
                                                                 /*german*/ "Man erzählt sich, daß #Pierre, an der Spitze des Feuertempels#, #[[1]]# verstecke.",
                                                                 /*french*/ "Selon moi, #Pierre au sommet du Temple du Feu# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                              // /*spanish*/#Pierre en lo alto del Templo del Fuego# esconde #[[1]]#.
                                                              {},
                                                              {CustomMessage("They say that a #scarecrow atop the volcano# hides #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß eine #Vogelscheuche, an der Spitze des Vulkans#, #[[1]]# verstecke.",
                                                                  /*french*/ "Selon moi, l'#épouvantail au sommet d'un volcan# donne #[[1]]#.", {QM_RED, QM_GREEN})});
                                                               // /*spanish*/un #espantapájaros en lo alto del volcán# esconde #[[1]]#.

    hintTextTable[RHT_FIRE_TEMPLE_MEGATON_HAMMER_CHEST] = HintText(CustomMessage("They say that the #Flare Dancer atop the Fire Temple# guards a chest containing #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß der #Flammenderwisch, an der Spitze des Feuertempels#, eine Truhe bewache welche #[[1]]# enthielte.",
                                                                      /*french*/ "Selon moi, le #Danse-Flamme au sommet du Temple du Feu# protège #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                   // /*spanish*/el #Bailaguego en lo alto del Templo del Fuego# otorga #[[1]]#.
                                                                   {},
                                                                   {CustomMessage("They say that the #Flare Dancer atop the volcano# guards a chest containing #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß der #Flammenderwisch, an der Spitze des Vulkans#, eine Truhe bewache welche #[[1]]# enthielte.",
                                                                       /*french*/ "Selon moi, le #danseur au sommet du volcan# protège #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                    // /*spanish*/el #Bailafuego en lo alto del volcán# otorga #[[1]]#.

    hintTextTable[RHT_FIRE_TEMPLE_MQ_CHEST_ON_FIRE] = HintText(CustomMessage("They say that the #Flare Dancer atop the Fire Temple# guards a chest containing #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß der #Flammenderwisch, an der Spitze des Feuertempels#, eine Truhe bewache welche #[[1]]# enthielte.",
                                                                  /*french*/ "Selon moi, le #Danse-Flamme au sommet du Temple du Feu# protège #[[1]]#.", {QM_RED, QM_GREEN}),
                                                               // /*spanish*/el #Bailafuego en lo alto del Templo del Fuego# otorga #[[1]]#.
                                                               {},
                                                               {CustomMessage("They say that the #Flare Dancer atop the volcano# guards a chest containing #[[1]]#.",
                                                                   /*german*/ "Man erzählt sich, daß der #Flammenderwisch, an der Spitze des Vulkans#, eine Truhe bewache welche #[[1]]# enthielte.",
                                                                   /*french*/ "Selon moi, le #danseur au sommet du volcan# protège #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                // /*spanish*/el #Bailafuego en lo alto del volcán# otorga #[[1]]#.

    hintTextTable[RHT_FIRE_TEMPLE_MQ_GS_SKULL_ON_FIRE] = HintText(CustomMessage("They say that a #spider under a block in the Fire Temple# holds #[[1]]#.",
                                                                     /*german*/ "Man erzählt sich, daß eine #Spinne unter einem Block im Feuertempel# #[[1]]# hielte.",
                                                                     /*french*/ "Selon moi, une #Skulltula sous un bloc dans le Temple du Feu# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                  // /*spanish*/una #Skulltula bajo un bloque del Templo del Fuego# otorga #[[1]]#.
                                                                  {},
                                                                  {CustomMessage("They say that a #spider under a block in the volcano# holds #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß eine #Spinne unter einem Block im Vulkan# #[[1]]# hielte.",
                                                                      /*french*/ "Selon moi, l'#araignée sous un bloc dans le volcan# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                   // /*spanish*/una #Skulltula bajo el bloque de un volcán# otorga #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_RIVER_CHEST] = HintText(CustomMessage("They say that beyond the #river in the Water Temple# waits #[[1]]#.",
                                                              /*german*/ "Man erzählt sich, daß jenseits des #Flußes im Wassertempel# #[[1]]# |warte|warten|.",
                                                              /*french*/ "Selon moi, au delà de #la rivière dans le Temple de l'Eau# se cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                           // /*spanish*/tras el #río del Templo del Agua# yace #[[1]]#.
                                                           {},
                                                           {CustomMessage("They say that beyond the #river under the lake# waits #[[1]]#.",
                                                               /*german*/ "Man erzählt sich, daß jenseits des #Flußes unterhalb des Sees# #[[1]]# |warte|warten|.",
                                                               /*french*/ "Selon moi, au delà de #la rivière sous le lac# se cache #[[1]]#.", {QM_RED, QM_GREEN})});
                                                            // /*spanish*/tras el #río bajo el lago# yace #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_BOSS_KEY_CHEST] = HintText(CustomMessage("They say that dodging #rolling boulders in the Water Temple# leads to #[[1]]#.",
                                                                 /*german*/ "Man erzählt sich, daß das Ausweichen von #rollenden Felsen im Wassertempel# #[[1]]# einbrächte.",
                                                                 /*french*/ "Selon moi, éviter des #rochers roulants dans le Temple de l'Eau# mène à #[[1]]#.", {QM_RED, QM_GREEN}),
                                                              // /*spanish*/esquivar #rocas rondantes del Templo del Agua# conduce a #[[1]]#.
                                                              {},
                                                              {CustomMessage("They say that dodging #rolling boulders under the lake# leads to #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß das Ausweichen von #rollenden Felsen unterhalb eines Sees# #[[1]]# einbrächte.",
                                                                  /*french*/ "Selon moi, éviter des #rochers roulants sous le lac# mène à #[[1]]#.", {QM_RED, QM_GREEN})});
                                                               // /*spanish*/esquivar #rocas rodantes bajo el lago# conduce a #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_GS_BEHIND_GATE] = HintText(CustomMessage("They say that a spider behind a #gate in the Water Temple# holds #[[1]]#.",
                                                                 /*german*/ "Man erzählt sich, daß eine Spinne hinter einem #Tor im Wassertempel# #[[1]]# hielte.",
                                                                 /*french*/ "Selon moi, la Skulltula derrière une #barrière dans le Temple de l'Eau# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                              // /*spanish*/una Skulltula tras #una valla del Templo del Agua# otorga #[[1]]#.
                                                              {},
                                                              {CustomMessage("They say that a spider behind a #gate under the lake# holds #[[1]]#.",
                                                                  /*german*/ "Man erzählt sich, daß eine Spinne hinter einem #Tor unterhalb des Sees# #[[1]]# hielte.",
                                                                  /*french*/ "Selon moi, l'araignée derrière une #barrière sous le lac# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                               // /*spanish*/una Skulltula tras #una valla bajo el lago# otorga #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_MQ_FREESTANDING_KEY] = HintText(CustomMessage("They say that hidden in a #box in the Water Temple# lies #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß versteckt in einem #Kasten im Wassertempel# #[[1]]# |läge|lägen|.",
                                                                      /*french*/ "Selon moi, dans une #boîte dans le Temple de l'Eau# gît #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                   // /*spanish*/en una #caja del Templo del Agua# yace #[[1]]#.
                                                                   {},
                                                                   {CustomMessage("They say that hidden in a #box under the lake# lies #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß versteckt in einem #Kasten unterhalb des Sees# #[[1]]# |läge|lägen|.",
                                                                       /*french*/ "Selon moi, dans une #boîte sous le lac# gît #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                    // /*spanish*/en una #caja bajo el lago# yace #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_MQ_GS_FREESTANDING_KEY_AREA] = HintText(CustomMessage("They say that the #locked spider in the Water Temple# holds #[[1]]#.",
                                                                              /*german*/ "Man erzählt sich, daß eine #eingeschloßene Spinne im Wassertempel# #[[1]]# hielte.",
                                                                              /*french*/ "Selon moi, une #Skulltula emprisonnée dans le Temple de l'Eau# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                           // /*spanish*/la #Skulltula enjaulada del Templo del Agua# otorga #[[1]]#.
                                                                           {},
                                                                           {CustomMessage("They say that the #locked spider under the lake# holds #[[1]]#.",
                                                                               /*german*/ "Man erzählt sich, daß eine #eingeschloßene Spinne unterhalb des Sees# #[[1]]# hielte.",
                                                                               /*french*/ "Selon moi, l'#araignée emprisonnée sous le lac# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                            // /*spanish*/la #Skulltula enjaulada bajo el lago# otorga #[[1]]#.

    hintTextTable[RHT_WATER_TEMPLE_MQ_GS_TRIPLE_WALL_TORCH] = HintText(CustomMessage("They say that a spider behind a #gate in the Water Temple# holds #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß eine Spinne hinter einem #Tor im Wassertempel# #[[1]]# hielte.",
                                                                          /*french*/ "Selon moi, une #Skulltula derrière une barrière dans le Temple de l'Eau# a #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                       // /*spanish*/una Skulltula tras una #valla del Templo del Agua# #[[1]]#.
                                                                       {},
                                                                       {CustomMessage("They say that a spider behind a #gate under the lake# holds #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß eine Spinne hinter einem #Tor unterhalb des Sees# #[[1]]# hielte.",
                                                                           /*french*/ "Selon moi, l'#araignée derrière une barrière sous le lac# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                        // /*spanish*/una Skulltula tras una #valla bajo el lago# otorga #[[1]]#.

    hintTextTable[RHT_GERUDO_TRAINING_GROUND_UNDERWATER_SILVER_RUPEE_CHEST] = HintText(CustomMessage("They say that those who seek #sunken silver rupees# will find #[[1]]#.",
                                                                                          /*german*/ "Man erzählt sich, daß jene, welche #versunkene silberne Rubine# suchen, #[[1]]# finden würden.",
                                                                                          /*french*/ "Selon moi, ceux qui pêchent les #joyaux argentés# trouveront #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                                       // /*spanish*/aquellos que busquen las #rupias plateadas sumergidas# encontrarán

    hintTextTable[RHT_GERUDO_TRAINING_GROUND_MQ_UNDERWATER_SILVER_RUPEE_CHEST] = HintText(CustomMessage("They say that those who seek #sunken silver rupees# will find #[[1]]#.",
                                                                                             /*german*/ "Man erzählt sich, daß jene, welche #versunkene silberne Rubine# suchen, #[[1]]# finden würden.",
                                                                                             /*french*/ "Selon moi, ceux qui pêchent les #joyaux argentés# trouveront #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                                          // /*spanish*/aquellos que busquen las #rupias plateadas sumergidas# encontrarán #[[1]]#.

    hintTextTable[RHT_GERUDO_TRAINING_GROUND_MAZE_PATH_FINAL_CHEST] = HintText(CustomMessage("They say that the final prize of #the thieves' training# is #[[1]]#.",
                                                                                  /*german*/ "Man erzählt sich, daß der letzte Preis des #Diebestrainings# #[[1]]# |sei|seien|.",
                                                                                  /*french*/ "Selon moi, la récompense ultime de #l'épreuve des voleurs# est #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                               // /*spanish*/la recompensa final de la #instrucción de las bandida# brinda #[[1]]#.

    hintTextTable[RHT_GERUDO_TRAINING_GROUND_MQ_ICE_ARROWS_CHEST] = HintText(CustomMessage("They say that the final prize of #the thieves' training# is #[[1]]#.",
                                                                                /*german*/ "Man erzählt sich, daß der letzte Preis des #Diebestrainings# #[[1]]# |sei|seien|.",
                                                                                /*french*/ "Selon moi, la récompense ultime de #l'épreuve des voleurs# est #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                             // /*spanish*/el premio final de la #instrucción de las bandidas# brinda #[[1]]#.

    hintTextTable[RHT_BOTTOM_OF_THE_WELL_LENS_OF_TRUTH_CHEST] = HintText(CustomMessage("They say that #Dead Hand in the well# holds #[[1]]#.",
                                                                            /*german*/ "Man erzählt sich, daß der #Hirnsauger im Brunnen# #[[1]]# hielte.",
                                                                            /*french*/ "Selon moi, le #Poigneur dans le Puits# cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                         // /*spanish*/la #Mano Muerta del pozo# concede #[[1]]#.
                                                                         {},
                                                                         {CustomMessage("They say that the well's #grasping ghoul# hides #[[1]]#.",
                                                                             /*german*/ "Man erzählt sich, daß der #habgierige Fledderer des Brunnens# #[[1]]# verstecke.",
                                                                             /*french*/ "Selon moi, la #terreur du Puits# cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                          // /*spanish*/en las #profundidades del pozo# se esconde #[[1]]#.
                                                                          CustomMessage("They say that a #nether dweller in the well# holds #[[1]]#.",
                                                                             /*german*/ "Man erzählt sich, daß der #Unterweltbewohner des Brunnens# #[[1]]# hielte.",
                                                                             /*french*/ "Selon moi, le #spectre qui réside dans le Puits# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                          // /*spanish*/el #temido morador del pozo# concede #[[1]]#.

    hintTextTable[RHT_BOTTOM_OF_THE_WELL_MQ_COMPASS_CHEST] = HintText(CustomMessage("They say that #Dead Hand in the well# holds #[[1]]#.",
                                                                         /*german*/ "Man erzählt sich, daß der #Hirnsauger des Brunnens# #[[1]]# hielte.",
                                                                         /*french*/ "Selon moi, le #Poigneur dans le Puits# cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                      // /*spanish*/la #Mano Muerta del pozo# concede #[[1]]#.
                                                                      {},
                                                                      {CustomMessage("They say that the well's #grasping ghoul# hides #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß der #habgierige Fledderer des Brunnens# #[[1]]# verstecke.",
                                                                          /*french*/ "Selon moi, la #terreur du Puits# cache #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                       // /*spanish*/en las #profundidades del pozo# se esconde #[[1]]#.
                                                                       CustomMessage("They say that a #nether dweller in the well# holds #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß der #Unterweltbewohner des Brunnens# #[[1]]# hielte.",
                                                                          /*french*/ "Selon moi, le #spectre qui réside dans le Puits# a #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                       // /*spanish*/el #temido morador del pozo# concede #[[1]]#.

    hintTextTable[RHT_SPIRIT_TEMPLE_SILVER_GAUNTLETS_CHEST] = HintText(CustomMessage("They say that upon the #Colossus's right hand# is #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß auf der #rechten Hand des Kolosses# #[[1]]# |sei|seien|.",
                                                                          /*french*/ "Selon moi, sur la #main droite du colosse# repose #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                       // /*spanish*/en la #mano derecha del Coloso# yace #[[1]]#.
                                                                       {},
                                                                       {CustomMessage("They say that the treasure #sought by Nabooru# is #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß der #von Naboru gesuchte Schatz# #[[1]]# |sei|seien|.",
                                                                           /*french*/ "Selon moi, le trésor que #recherche Nabooru# est #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                        // /*spanish*/el #ansiado tesoro de Nabooru# brinda #[[1]]#.

    hintTextTable[RHT_SPIRIT_TEMPLE_MIRROR_SHIELD_CHEST] = HintText(CustomMessage("They say that upon the #Colossus's left hand# is #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß auf der #linken Hand des Kolosses# #[[1]]# |sei|seien|.",
                                                                       /*french*/ "Selon moi, sur la #main gauche du colosse# repose #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                    // /*spanish*/en la #mano izquierda del Coloso# yace #[[1]]#.

    hintTextTable[RHT_SPIRIT_TEMPLE_MQ_CHILD_HAMMER_SWITCH_CHEST] = HintText(CustomMessage("They say that a #temporal paradox in the Spirit Temple# yields #[[1]]#.",
                                                                                /*german*/ "Man erzählt sich, daß ein #zeitliches Paradox im Geistertempel# #[[1]]# einbrächte.",
                                                                                /*french*/ "Selon moi, le #paradoxe temporel dans le Temple de l'Esprit# révèle #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                             // /*spanish*/una #paradoja temporal del Coloso# conduce a #[[1]]#.
                                                                             {},
                                                                             {CustomMessage("They say that a #temporal paradox in the Colossus# yields #[[1]]#.",
                                                                                 /*german*/ "Man erzählt sich, daß ein #zeitliches Paradox im Koloss# #[[1]]# einbrächte.",
                                                                                 /*french*/ "Selon moi, un #paradoxe temporel dans le colosse# révèle #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                              // /*spanish*/una #paradoja temporal del Coloso# conduce a #[[1]]#.

    hintTextTable[RHT_SPIRIT_TEMPLE_MQ_SYMPHONY_ROOM_CHEST] = HintText(CustomMessage("They say that a #symphony in the Spirit Temple# yields #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß eine #Symphonie im Geistertempel# #[[1]]# einbrächte.",
                                                                          /*french*/ "Selon moi, les #cinq chansons du Temple de l'Esprit# révèlent #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                       // /*spanish*/una #sinfonía del Coloso# conduce a #[[1]]#.
                                                                       {},
                                                                       {CustomMessage("They say that a #symphony in the Colossus# yields #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß eine #Symphonie im Koloss# #[[1]]# einbrächte.",
                                                                           /*french*/ "Selon moi, la #symphonie du colosse# révèle #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                        // /*spanish*/una #sinfonía del Coloso# conduce a #[[1]]#.

    hintTextTable[RHT_SPIRIT_TEMPLE_MQ_GS_SYMPHONY_ROOM] = HintText(CustomMessage("They say that a #spider's symphony in the Spirit Temple# yields #[[1]]#.",
                                                                       /*german*/ "Man erzählt sich, daß eine #Spinnensymphonie im Geistertempel# #[[1]]# einbrächte.",
                                                                       /*french*/ "Selon moi, la #mélodie de la Skulltula du Temple de l'Esprit# révèle #[[1]]#.", {QM_RED, QM_GREEN}),
                                                                    // /*spanish*/la #Skulltula de la sinfonía del Coloso# otorga #[[1]]#.
                                                                    {},
                                                                    {CustomMessage("They say that a #spider's symphony in the Colossus# yields #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß eine #Spinnensymphonie im Koloss# #[[1]]# einbrächte.",
                                                                        /*french*/ "Selon moi, la #mélodie de l'araignée du colosse# révèle #[[1]]#.", {QM_RED, QM_GREEN})});
                                                                     // /*spanish*/la #Skulltula de la sinfonía del Coloso# otorga #[[1]]#.

    hintTextTable[RHT_SHADOW_TEMPLE_INVISIBLE_FLOORMASTER_CHEST] = HintText(CustomMessage("They say that shadows in an #invisible maze# guard #[[1]]#.",
                                                                               /*german*/ "Man erzählt sich, daß Schatten in einem #unsichtbaren Labyrinth# #[[1]]# bewachen würden.",
                                                                               /*french*/ "Selon moi, les ombres dans le #labyrinthe invisible# protègent #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                            // /*spanish*/las sombras del #laberinto misterioso# esconden #[[1]]#.

    hintTextTable[RHT_SHADOW_TEMPLE_MQ_BOMB_FLOWER_CHEST] = HintText(CustomMessage("They say that shadows in an #invisible maze# guard #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß Schatten in einem #unsichtbaren Labyrinth# #[[1]]# bewachen würden.",
                                                                        /*french*/ "Selon moi, les ombres dans le #labyrinthe invisible# protègent #[[1]]#.", {QM_RED, QM_GREEN})),
                                                                     // /*spanish*/las sombras del #laberinto invisible# esconden #[[1]]#.

    /*--------------------------
    |    ENTRANCE HINT TEXT    |
    ---------------------------*/

    hintTextTable[RHT_DESERT_COLOSSUS_TO_COLOSSUS_GROTTO] = HintText(CustomMessage("They say that lifting a #rock in the desert# reveals #[[1]]#.",
                                                                        /*german*/ "Man erzählt sich, daß das Heben eines #Felsens in der Wüste# #[[1]]# enthülle.",
                                                                        /*french*/ "Selon moi, soulever une #roche dans le désert# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                     // /*spanish*/levantar una #roca del desierto# revela #[[1]]#.

    hintTextTable[RHT_GV_GROTTO_LEDGE_TO_GV_OCTOROK_GROTTO] = HintText(CustomMessage("They say that a rock on #a ledge in the valley# hides #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß ein Felsen auf #einem Vorsprung des Tals# #[[1]]# verstecke.",
                                                                          /*french*/ "Selon moi, soulever une #roche dans la vallée# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                       // /*spanish*/levantar una #roca al borde del valle# esconde #[[1]]#.

    hintTextTable[RHT_GC_GROTTO_PLATFORM_TO_GC_GROTTO] = HintText(CustomMessage("They say that a #pool of lava# in Goron City blocks the way to #[[1]]#.",
                                                                     /*german*/ "Man erzählt sich, daß ein #Lavateich# in Goronia #[[1]]# blockiere.",
                                                                     /*french*/ "Selon moi, l'#étang de lave# dans le village Goron renferme #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                  // /*spanish*/un #estanque de lava# en la Ciudad Goron bloquea el paso a #[[1]]#.

    hintTextTable[RHT_GERUDO_FORTRESS_TO_GF_STORMS_GROTTO] = HintText(CustomMessage("They say that a #storm within Gerudo's Fortress# reveals #[[1]]#.",
                                                                         /*german*/ "Man erzählt sich, daß ein #Sturm innerhalb der Gerudo-Festung# #[[1]]# enthülle.",
                                                                         /*french*/ "Selon moi, la #tempête dans la forteresse# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                      // /*spanish*/una #tormenta en la Fortaleza Gerudo# revela #[[1]]#.

    hintTextTable[RHT_ZORAS_DOMAIN_TO_ZD_STORMS_GROTTO] = HintText(CustomMessage("They say that a #storm within Zora's Domain# reveals #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß ein #Sturm innerhalb von Zoras Reich# #[[1]]# enthülle.",
                                                                      /*french*/ "Selon moi, la #tempête dans le Domaine Zora# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                   // /*spanish*/una #tormenta en la Región de los Zora# revela #[[1]]#.

    hintTextTable[RHT_HYRULE_CASTLE_GROUNDS_TO_HC_STORMS_GROTTO] = HintText(CustomMessage("They say that a #storm near the castle# reveals #[[1]]#.",
                                                                               /*german*/ "Man erzählt sich, daß ein #Sturm in der Nähe des Schloßes# #[[1]]# enthülle.",
                                                                               /*french*/ "Selon moi, la #tempête près du château# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                            // /*spanish*/una #tormenta junto al castillo# revela #[[1]]#.

    hintTextTable[RHT_GV_FORTRESS_SIDE_TO_GV_STORMS_GROTTO] = HintText(CustomMessage("They say that a #storm in the valley# reveals #[[1]]#.",
                                                                          /*german*/ "Man erzählt sich, daß ein #Sturm im Tal# #[[1]]# enthülle.",
                                                                          /*french*/ "Selon moi, la #tempête dans la vallée# révèle #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                       // /*spanish*/una #tormenta en el valle# revela #[[1]]#.

    hintTextTable[RHT_DESERT_COLOSSUS_TO_COLOSSUS_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("They say that a #fractured desert wall# hides #[[1]]#.",
                                                                                      /*german*/ "Man erzählt sich, daß eine #gebrochene Wüstenmauer# #[[1]]# verstecke.",
                                                                                      /*french*/ "Selon moi, le #mur fragile du désert# cache #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                                   // /*spanish*/una #agrietada pared del desierto# esconde #[[1]]#.

    hintTextTable[RHT_GANONS_CASTLE_GROUNDS_TO_OGC_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("They say that a #heavy pillar# outside the castle obstructs #[[1]]#.",
                                                                                       /*german*/ "Man erzählt sich, daß eine #schwere Säule# außerhalb des Schloßes #[[1]]# versperre.",
                                                                                       /*french*/ "Selon moi, le #rocher fragile près du château# cache #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                                    // /*spanish*/una #pesada columna# fuera del castillo obstruye #[[1]]#.

    hintTextTable[RHT_ZORAS_FOUNTAIN_TO_ZF_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("They say that a #fountain wall# hides #[[1]]#.",
                                                                               /*german*/ "Man erzählt sich, daß eine #Wasserwand# #[[1]]# verstecke.",
                                                                               /*french*/ "Selon moi, le #mur fragile du réservoir# cache #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                            // /*spanish*/una #pared de la fuente# esconde #[[1]]#.

    hintTextTable[RHT_GV_FORTRESS_SIDE_TO_GV_CARPENTER_TENT] = HintText(CustomMessage("They say that a #tent in the valley# covers #[[1]]#.",
                                                                           /*german*/ "Man erzählt sich, daß ein #Zelt im Tal# #[[1]]# verdecke.",
                                                                           /*french*/ "Selon moi, la #tente dans la vallée# recouvre #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                        // /*spanish*/una #tienda de campaña del valle# cubre #[[1]]#.

    hintTextTable[RHT_GRAVEYARD_WARP_PAD_REGION_TO_SHADOW_TEMPLE_ENTRYWAY] = HintText(CustomMessage("They say that at the #back of the Graveyard#, there is #[[1]]#.",
                                                                                         /*german*/ "Man erzählt sich, daß sich #hinter dem Friedhof# #[[1]]# |befände|befänden|.",
                                                                                         /*french*/ "Selon moi, #derrière le Cimetière# gît #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                                      // /*spanish*/en la #parte trasera del cementerio# se halla #[[1]]#.

    hintTextTable[RHT_LAKE_HYLIA_TO_WATER_TEMPLE_LOBBY] = HintText(CustomMessage("They say that deep #under a vast lake#, one can find #[[1]]#.",
                                                                      /*german*/ "Man erzählt sich, daß man tief #unterhalb eines gewaltigen Sees# #[[1]]# finden könne.",
                                                                      /*french*/ "Selon moi, #sous le lac# gît #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                   // /*spanish*/en las #profundidades de un lago inmenso# se halla #[[1]]#.

    hintTextTable[RHT_GERUDO_FORTRESS_TO_GERUDO_TRAINING_GROUND_LOBBY] = HintText(CustomMessage("They say that paying a #fee to the Gerudos# grants access to #[[1]]#.",
                                                                                      /*german*/ "Man erzählt sich, daß das Bezahlen einer #Gebühr an die Gerudos# #[[1]]# gewähre.",
                                                                                      /*french*/ "Selon moi, l'#entrée payante des Gerudo# donne accès à #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                                   // /*spanish*/pagarle una #tasa a las gerudo# da acceso a #[[1]]#.

    hintTextTable[RHT_ZORAS_FOUNTAIN_TO_JABU_JABUS_BELLY_BEGINNING] = HintText(CustomMessage("They say that inside #Jabu-Jabu#, one can find #[[1]]#.",
                                                                                  /*german*/ "Man erzählt sich, daß man #in Jabu-Jabu# #[[1]]# finden könne.",
                                                                                  /*french*/ "Selon moi, #dans Jabu-Jabu# se trouve #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                               // /*spanish*/dentro de #Jabu-Jabu# se halla #[[1]]#.

    hintTextTable[RHT_KAKARIKO_VILLAGE_TO_BOTTOM_OF_THE_WELL] = HintText(CustomMessage("They say that a #village well# leads to #[[1]]#.",
                                                                            /*german*/ "Man erzählt sich, daß man #in einem Dorfbrunnen# #[[1]]# finden könne.",
                                                                            /*french*/ "Selon moi, dans le fond du #Puits du village# gît #[[1]]#.", {QM_RED, QM_BLUE})),
                                                                         // /*spanish*/el #pozo de un pueblo# conduce a #[[1]]#.

    /*--------------------------
    |      EXIT HINT TEXT      |
    ---------------------------*/

    hintTextTable[RHT_LINKS_POCKET] = HintText(CustomMessage("Link's Pocket",
                                                  /*german*/ "in Links Tasche",
                                                  /*french*/ "les Poches de @"));
                                               // /*spanish*/el bolsillo de @

    hintTextTable[RHT_KOKIRI_FOREST] = HintText(CustomMessage("Kokiri Forest",
                                                   /*german*/ "im Kokiri-Wald",
                                                   /*french*/ "la Forêt Kokiri"));
                                                // /*spanish*/el Bosque Kokiri

    hintTextTable[RHT_THE_LOST_WOODS] = HintText(CustomMessage("the Lost Woods",
                                                    /*german*/ "in den Verlorenen Wäldern",
                                                    /*french*/ "les Bois Perdus"));
                                                 // /*spanish*/el Bosque Perdido

    hintTextTable[RHT_SACRED_FOREST_MEADOW] = HintText(CustomMessage("Sacred Forest Meadow",
                                                          /*german*/ "auf der Heiligen Lichtung",
                                                          /*french*/ "le Bosquet Sacré"));
                                                       // /*spanish*/la pradera sagrada del bosque

    hintTextTable[RHT_HYRULE_FIELD] = HintText(CustomMessage("Hyrule Field",
                                                  /*german*/ "in der Hylianischen Steppe",
                                                  /*french*/ "la Plaine d'Hyrule"));
                                               // /*spanish*/la Llanura de Hyrule

    hintTextTable[RHT_LAKE_HYLIA] = HintText(CustomMessage("Lake Hylia",
                                                /*german*/ "am Hylia-See",
                                                /*french*/ "le Lac Hylia"));
                                             // /*spanish*/el Lago Hylia

    hintTextTable[RHT_GERUDO_VALLEY] = HintText(CustomMessage("Gerudo Valley",
                                                   /*german*/ "im Gerudotal",
                                                   /*french*/ "la Vallée Gerudo"));
                                                // /*spanish*/el Valle Gerudo

    hintTextTable[RHT_GERUDO_FORTRESS] = HintText(CustomMessage("Gerudo's Fortress",
                                                     /*german*/ "in der Gerudo-Festung",
                                                     /*french*/ "le Repaire des Voleurs"));
                                                  // /*spanish*/la Fortaleza Gerudo

    hintTextTable[RHT_HAUNTED_WASTELAND] = HintText(CustomMessage("Haunted Wasteland",
                                                       /*german*/ "in der Gespensterwüste",
                                                       /*french*/ "le Désert Hanté"));
                                                    // /*spanish*/el desierto encantado

    hintTextTable[RHT_DESERT_COLOSSUS] = HintText(CustomMessage("Desert Colossus",
                                                     /*german*/ "am Wüstenkoloss",
                                                     /*french*/ "le Colosse du Désert"));
                                                  // /*spanish*/el Coloso del Desierto

    hintTextTable[RHT_THE_MARKET] = HintText(CustomMessage("the Market",
                                                /*german*/ "auf dem Markt",
                                                /*french*/ "la Place du Marché"));
                                             // /*spanish*/la plaza del mercado

    hintTextTable[RHT_TEMPLE_OF_TIME] = HintText(CustomMessage("Temple of Time",
                                                    /*german*/ "in der Zitadelle der Zeit",
                                                    /*french*/ "le Temple du Temps"));
                                                 // /*spanish*/el Templo del Tiempo

    hintTextTable[RHT_CASTLE_GROUNDS] = HintText(CustomMessage("the Castle Grounds",
                                                    /*german*/ "auf dem Schloßgelände",
                                                    /*french*/ "le Château d'Hyrule"));
                                                 // /*spanish*/

    hintTextTable[RHT_HYRULE_CASTLE] = HintText(CustomMessage("Hyrule Castle",
                                                   /*german*/ "in Schloß Hyrule",
                                                   /*french*/ "le Château d'Hyrule"));
                                                // /*spanish*/el Castillo de Hyrule

    hintTextTable[RHT_OUTSIDE_GANONS_CASTLE] = HintText(CustomMessage("outside Ganon's Castle",
                                                           /*german*/ "außerhalb von Ganons Schloß",
                                                           /*french*/ "les alentours du Château de Ganon"));
                                                        // /*spanish*/el exterior del Castillo de Ganon

    hintTextTable[RHT_KAKARIKO_VILLAGE] = HintText(CustomMessage("Kakariko Village",
                                                      /*german*/ "in Kakariko",
                                                      /*french*/ "le Village Cocorico"));
                                                   // /*spanish*/Kakariko

    hintTextTable[RHT_THE_GRAVEYARD] = HintText(CustomMessage("the Graveyard",
                                                   /*german*/ "auf dem Friedhof",
                                                   /*french*/ "le Cimetière"));
                                                // /*spanish*/el cementerio

    hintTextTable[RHT_DEATH_MOUNTAIN_TRAIL] = HintText(CustomMessage("Death Mountain Trail",
                                                          /*german*/ "auf dem Gebirgspfad",
                                                          /*french*/ "le Chemin du Péril"));
                                                       // /*spanish*/el sendero de la Montaña de la Muerte

    hintTextTable[RHT_GORON_CITY] = HintText(CustomMessage("Goron City",
                                                /*german*/ "in Goronia",
                                                /*french*/ "le Village Goron"));
                                             // /*spanish*/la Ciudad Goron

    hintTextTable[RHT_DEATH_MOUNTAIN_CRATER] = HintText(CustomMessage("Death Mountain Crater",
                                                           /*german*/ "im Todeskrater",
                                                           /*french*/ "le Cratère du Péril"));
                                                        // /*spanish*/el cráter de la Montaña de la Muerte

    hintTextTable[RHT_ZORAS_RIVER] = HintText(CustomMessage("Zora's River",
                                                 /*german*/ "am Zora-Fluß",
                                                 /*french*/ "la Rivière Zora"));
                                              // /*spanish*/el Río Zora

    hintTextTable[RHT_ZORAS_DOMAIN] = HintText(CustomMessage("Zora's Domain",
                                                  /*german*/ "in Zoras Reich",
                                                  /*french*/ "le Domaine Zora"));
                                               // /*spanish*/la Región de los Zora

    hintTextTable[RHT_ZORAS_FOUNTAIN] = HintText(CustomMessage("Zora's Fountain",
                                                    /*german*/ "an Zoras Quelle",
                                                    /*french*/ "la Fontaine Zora"));
                                                 // /*spanish*/la Fuente Zora

    hintTextTable[RHT_LON_LON_RANCH] = HintText(CustomMessage("Lon Lon Ranch",
                                                   /*german*/ "auf der Lon Lon-Farm",
                                                   /*french*/ "le Ranch Lon Lon"));
                                                // /*spanish*/el Rancho Lon Lon

    /*--------------------------
    |     REGION HINT TEXT     |
    ---------------------------*/

    hintTextTable[RHT_KF_LINKS_HOUSE] = HintText(CustomMessage("Link's House",
                                                    /*german*/ "in #Links Haus#",
                                                    /*french*/ "la #maison de @#"));
                                                 // /*spanish*/la casa de @

    hintTextTable[RHT_KF_MIDOS_HOUSE] = HintText(CustomMessage("Mido's house",
                                                    /*german*/ "in #Midos Haus#",
                                                    /*french*/ "la Cabane du Grand Mido"));
                                                 // /*spanish*/la casa de Mido

    hintTextTable[RHT_KF_SARIAS_HOUSE] = HintText(CustomMessage("Saria's House",
                                                     /*german*/ "in #Salias Haus#",
                                                     /*french*/ "la Cabane de Saria"));
                                                  // /*spanish*/la casa de Saria

    hintTextTable[RHT_KF_HOUSE_OF_TWINS] = HintText(CustomMessage("the #House of Twins#",
                                                       /*german*/ "im #Haus der Zwillinge#",
                                                       /*french*/ "la Cabane des Jumelles"));
                                                    // /*spanish*/la casa de las gemelas

    hintTextTable[RHT_KF_KNOW_IT_ALL_HOUSE] = HintText(CustomMessage("Know-It-All Brothers' House",
                                                          /*german*/ "im #Haus der allwissenden Brüder#",
                                                          /*french*/ "la Cabane des frères Je-Sais-Tout"));
                                                       // /*spanish*/la casa de los hermanos Sabelotodo

    hintTextTable[RHT_KF_KOKIRI_SHOP] = HintText(CustomMessage("the #Kokiri Shop#",
                                                    /*german*/ "im #Kokiri-Laden#",
                                                    /*french*/ "le #Magasin Kokiri#"));
                                                 // /*spanish*/la tienda kokiri

    hintTextTable[RHT_LH_LAB] = HintText(CustomMessage("the #Lakeside Laboratory#",
                                            /*german*/ "im #Labor am See#",
                                            /*french*/ "le #Laboratoire du Lac#"));
                                         // /*spanish*/el laboratorio del lago

    hintTextTable[RHT_LH_FISHING_POND] = HintText(CustomMessage("the #Fishing Pond#",
                                                     /*german*/ "am #Fischweiher#",
                                                     /*french*/ "l'#Étang#"));
                                                  // /*spanish*/el estanque

    hintTextTable[RHT_GV_CARPENTER_TENT] = HintText(CustomMessage("the #Carpenters' tent#",
                                                       /*german*/ "im #Zelt der Zimmerleute#",
                                                       /*french*/ "la #Tente des charpentiers#"));
                                                    // /*spanish*/la #tienda de campaña de los carpinteros#

    hintTextTable[RHT_MARKET_GUARD_HOUSE] = HintText(CustomMessage("the #Guard House#",
                                                        /*german*/ "im #Wachhaus#",
                                                        /*french*/ "le #poste de garde#"));
                                                     // /*spanish*/la caseta de guardia

    hintTextTable[RHT_MARKET_MASK_SHOP] = HintText(CustomMessage("the #Happy Mask Shop#",
                                                      /*german*/ "beim #Maskenhändler#",
                                                      /*french*/ "la #Foire Aux Masques#"));
                                                   // /*spanish*/la tienda de La Máscara Feliz

    hintTextTable[RHT_MARKET_BOMBCHU_BOWLING] = HintText(CustomMessage("the #Bombchu Bowling Alley#",
                                                            /*german*/ "bei der #Minenbowlingbahn",
                                                            /*french*/ "le #Bowling Teigneux#"));
                                                         // /*spanish*/la Bolera Bombchu

    hintTextTable[RHT_MARKET_POTION_SHOP] = HintText(CustomMessage("the #Market Potion Shop#",
                                                        /*german*/ "im #Magie-Laden des Marktes#",
                                                        /*french*/ "l'#apothicaire de la Place du Marché#"));
                                                     // /*spanish*/la tienda de pociones de la plaza del mercado

    hintTextTable[RHT_MARKET_TREASURE_CHEST_GAME] = HintText(CustomMessage("the #Treasure Chest Shop#",
                                                                /*german*/ "in der #Truhenlotterie#",
                                                                /*french*/ "la #Chasse-aux-Trésors#"));
                                                             // /*spanish*/el Cofre del Tesoro

    hintTextTable[RHT_MARKET_BOMBCHU_SHOP] = HintText(CustomMessage("the #Bombchu Shop#",
                                                         /*german*/ "im #Krabbelminen-Laden#",
                                                         /*french*/ "le #Magasin de Missiles#"));
                                                      // /*spanish*/la Tienda Bombchu

    hintTextTable[RHT_MARKET_MAN_IN_GREEN_HOUSE] = HintText(CustomMessage("Man in Green's House",
                                                               /*german*/ "im #Haus des Mannes in grün#",
                                                               /*french*/ "la #Maison de l'Homme en Vert#"));
                                                            // /*spanish*/la casa del hombre de verde

    hintTextTable[RHT_KAK_WINDMILL] = HintText(CustomMessage("the #Windmill#",
                                                  /*german*/ "in der #Windmühle#",
                                                  /*french*/ "le #Moulin#"));
                                               // /*spanish*/el #molino#

    hintTextTable[RHT_KAK_CARPENTER_BOSS_HOUSE] = HintText(CustomMessage("the #Carpenters' Boss House#",
                                                              /*german*/ "im #Haus des Chefs der Zimmerleute#",
                                                              /*french*/ "la #Maison du Chef des ouvriers#"));
                                                           // /*spanish*/la casa del capataz de los carpinteros

    hintTextTable[RHT_KAK_HOUSE_OF_SKULLTULA] = HintText(CustomMessage("the #House of Skulltula#",
                                                            /*german*/ "im #Haus der Skulltula#",
                                                            /*french*/ "la #Maison des Skulltulas#"));
                                                         // /*spanish*/la casa de las Skulltulas

    hintTextTable[RHT_KAK_IMPAS_HOUSE] = HintText(CustomMessage("Impa's House",
                                                     /*german*/ "in #Impas Haus#",
                                                     /*french*/ "la #Maison d'Impa#"));
                                                  // /*spanish*/la casa de Impa

    hintTextTable[RHT_KAK_IMPAS_HOUSE_BACK] = HintText(CustomMessage("Impa's cow cage",
                                                          /*german*/ "in #Impas Kuhkäfig#",
                                                          /*french*/ "la #cage à vache d'Impa#"));
                                                       // /*spanish*/la jaula de la vaca de Impa

    hintTextTable[RHT_KAK_ODD_POTION_BUILDING] = HintText(CustomMessage("Granny's Potion Shop",
                                                             /*german*/ "in Asas Hexenladen",
                                                             /*french*/ "la #maison bleue de Cocorico#"));
                                                          // /*spanish*/la tienda de pociones de la abuela

    hintTextTable[RHT_GRAVEYARD_DAMPES_HOUSE] = HintText(CustomMessage("Dampé's Hut",
                                                            /*german*/ "in der #Hütte des Totengräbers#",
                                                            /*french*/ "la #Cabane du Fossoyeur#"));
                                                         // /*spanish*/la cabaña de Dampé

    hintTextTable[RHT_GC_SHOP] = HintText(CustomMessage("the #Goron Shop#",
                                             /*german*/ "im #Goronen-Laden#",
                                             /*french*/ "la #Boutique Goron#"));
                                          // /*spanish*/la #tienda goron#

    hintTextTable[RHT_ZD_SHOP] = HintText(CustomMessage("the #Zora Shop#",
                                             /*german*/ "im #Zora-Laden#",
                                             /*french*/ "la #Boutique Zora#"));
                                          // /*spanish*/la #tienda zora#

    hintTextTable[RHT_LLR_TALONS_HOUSE] = HintText(CustomMessage("Talon's House",
                                                      /*german*/ "im #Haus von Talon#",
                                                      /*french*/ "la #Maison de Talon#"));
                                                   // /*spanish*/la casa de Talon

    hintTextTable[RHT_LLR_STABLES] = HintText(CustomMessage("a #stable#",
                                                 /*german*/ "in den #Ställen#",
                                                 /*french*/ "l'#Étable#"));
                                              // /*spanish*/el establo

    hintTextTable[RHT_LLR_TOWER] = HintText(CustomMessage("the #Lon Lon Tower#",
                                               /*german*/ "im #Lon Lon-Turm#",
                                               /*french*/ "le #silo du Ranch Lon Lon#"));
                                            // /*spanish*/la torre Lon Lon

    hintTextTable[RHT_MARKET_BAZAAR] = HintText(CustomMessage("the #Market Bazaar#",
                                                   /*german*/ "im #Basar des Marktes#",
                                                   /*french*/ "le #Bazar de la Place du Marché#"));
                                                // /*spanish*/el bazar de la plaza del mercado

    hintTextTable[RHT_MARKET_SHOOTING_GALLERY] = HintText(CustomMessage("a #Slingshot Shooting Gallery#",
                                                             /*german*/ "in der #Schießbude des Marktes#",
                                                             /*french*/ "le #Jeu d'Adresse de la Place du Marché#"));
                                                          // /*spanish*/el Tiro al Blanco con tirachinas

    hintTextTable[RHT_KAK_BAZAAR] = HintText(CustomMessage("the #Kakariko Bazaar#",
                                                /*german*/ "im #Basar von Kakariko#",
                                                /*french*/ "le #Bazar de Cocorico#"));
                                             // /*spanish*/el bazar de Kakariko

    hintTextTable[RHT_KAK_POTION_SHOP_FRONT] = HintText(CustomMessage("the #Kakariko Potion Shop#",
                                                           /*german*/ "im #Magie-Laden von Kakariko#",
                                                           /*french*/ "l'#apothicaire de Cocorico#"));
                                                        // /*spanish*/la tienda de pociones de Kakariko

    hintTextTable[RHT_KAK_POTION_SHOP_BACK] = HintText(CustomMessage("the #Kakariko Potion Shop#",
                                                          /*german*/ "im #Magie-Laden von Kakariko#",
                                                          /*french*/ "l'#apothicaire de Cocorico#"));
                                                       // /*spanish*/la tienda de pociones de Kakariko

    hintTextTable[RHT_KAK_SHOOTING_GALLERY] = HintText(CustomMessage("a #Bow Shooting Gallery#",
                                                          /*german*/ "in der #Schießbude von Kakariko#",
                                                          /*french*/ "le #jeu d'adresse de Cocorico#"));
                                                       // /*spanish*/el Tiro al Blanco con arco

    hintTextTable[RHT_COLOSSUS_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                                   /*german*/ "in einer #Feen-Quelle#",
                                                                   /*french*/ "une #Fontaine Royale des Fées#"));
                                                                // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_HC_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                             /*german*/ "in einer #Feen-Quelle#",
                                                             /*french*/ "une #Fontaine Royale des Fées#"));
                                                          // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_OGC_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                              /*german*/ "in einer #Feen-Quelle#",
                                                              /*french*/ "une #Fontaine Royale des Fées#"));
                                                           // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_DMC_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                              /*german*/ "in einer #Feen-Quelle#",
                                                              /*french*/ "une #Fontaine Royale des Fées#"));
                                                           // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_DMT_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                              /*german*/ "in einer #Feen-Quelle#",
                                                              /*french*/ "une #Fontaine Royale des Fées#"));
                                                           // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_ZF_GREAT_FAIRY_FOUNTAIN] = HintText(CustomMessage("a #Great Fairy Fountain#",
                                                             /*german*/ "in einer #Feen-Quelle#",
                                                             /*french*/ "une #Fontaine Royale des Fées#"));
                                                          // /*spanish*/una fuente de la Gran Hada

    hintTextTable[RHT_GRAVEYARD_SHIELD_GRAVE] = HintText(CustomMessage("a #grave with a free chest#",
                                                            /*german*/ "in einem #Grab mit einer Gratistruhe#",
                                                            /*french*/ "le #tombeau avec un trésor#"));
                                                         // /*spanish*/la #tumba con un cofre#

    hintTextTable[RHT_GRAVEYARD_HEART_PIECE_GRAVE] = HintText(CustomMessage("a chest spawned by #Sun's Song#",
                                                                 /*german*/ "in einer von der #Hymne der Sonne# geschaffenen Truhe",
                                                                 /*french*/ "un #coffre apparaît avec le Chant du Soleil#"));
                                                              // /*spanish*/la #tumba de la Canción del Sol#

    hintTextTable[RHT_GRAVEYARD_COMPOSERS_GRAVE] = HintText(CustomMessage("the #Composers' Grave#",
                                                               /*german*/ "im #Königsgrab#",
                                                               /*french*/ "la #Tombe royale#"));
                                                            // /*spanish*/el #Panteón Real#

    hintTextTable[RHT_GRAVEYARD_DAMPES_GRAVE] = HintText(CustomMessage("Dampé's Grave",
                                                            /*german*/ "im #Grab des Totengräbers#",
                                                            /*french*/ "la #Tombe d'Igor#"));
                                                         // /*spanish*/la #tumba de Dampé#

    hintTextTable[RHT_DMT_COW_GROTTO] = HintText(CustomMessage("a solitary #Cow#",
                                                    /*german*/ "bei einer einsamen #Kuh#",
                                                    /*french*/ "la #grotte avec une vache#"));
                                                 // /*spanish*/una #vaca# solitaria

    hintTextTable[RHT_HC_STORMS_GROTTO] = HintText(CustomMessage("a sandy grotto with #fragile walls#",
                                                      /*german*/ "in einer sandige Grotte mit #fragilen Wänden#",
                                                      /*french*/ "la #grotte avec des murs fragiles#"));
                                                   // /*spanish*/la arenosa gruta de #frágiles paredes#

    hintTextTable[RHT_HF_TEKTITE_GROTTO] = HintText(CustomMessage("a pool guarded by a #Tektite#",
                                                       /*german*/ "in einem #Schwimmbecken mit einem Arachno#",
                                                       /*french*/ "l'#étang sous-terrain avec un Araknon#"));
                                                    // /*spanish*/un charco custodiado por un #Tektite#

    hintTextTable[RHT_HF_NEAR_KAK_GROTTO] = HintText(CustomMessage("a #Big Skulltula# guarding a Gold one",
                                                        /*german*/ "bei #zwei unterirdischen Skulltulas#",
                                                        /*french*/ "la #grotte d'araignées#"));
                                                     // /*spanish*/una #gran Skulltula# custodiando una dorada

    hintTextTable[RHT_HF_COW_GROTTO] = HintText(CustomMessage("a grotto full of #spider webs#",
                                                   /*german*/ "in einer Grotte voller #Spinnweben#",
                                                   /*french*/ "la #grotte couverte de toiles d'araignées#"));
                                                // /*spanish*/una gruta llena de #telarañas#

    hintTextTable[RHT_KAK_REDEAD_GROTTO] = HintText(CustomMessage("#ReDeads# guarding a chest",
                                                       /*german*/ "in einer Grotte mit #zwei Remorts#",
                                                       /*french*/ "le tombeau de #deux morts#"));
                                                    // /*spanish*/los #ReDeads# que custodian un cofre

    hintTextTable[RHT_SFM_WOLFOS_GROTTO] = HintText(CustomMessage("#Wolfos# guarding a chest",
                                                       /*german*/ "in einer Grotte mit #zwei Wolfos#",
                                                       /*french*/ "la #grotte iridescente#"));
                                                    // /*spanish*/los #Wolfos# que custodian un cofre

    hintTextTable[RHT_GV_OCTOROK_GROTTO] = HintText(CustomMessage("an #Octorok# guarding a rich pool",
                                                       /*german*/ "in einem #Schwimmbecken mit einem Oktorok#",
                                                       /*french*/ "un #étang sous-terrain avec un Octorok#"));
                                                    // /*spanish*/un #Octorok# que custodia un lujoso charco

    hintTextTable[RHT_DEKU_THEATER] = HintText(CustomMessage("the #Lost Woods Stage#",
                                                  /*german*/ "auf der #Waldbühne#",
                                                  /*french*/ "le #théâtre sylvestre#"));
                                               // /*spanish*/el #escenario del Bosque Perdido#

    hintTextTable[RHT_ZR_OPEN_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                    /*german*/ "in einer #gewöhnlichen Grotte#",
                                                    /*french*/ "une #grotte avec un trésor#"));
                                                 // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_DMC_UPPER_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                      /*german*/ "in einer #gewöhnlichen Grotte#",
                                                      /*french*/ "une #grotte avec un trésor#"));
                                                   // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_DMT_STORMS_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                       /*german*/ "in einer #gewöhnlichen Grotte#",
                                                       /*french*/ "une #grotte avec un trésor#"));
                                                    // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_KAK_OPEN_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                     /*german*/ "in einer #gewöhnlichen Grotte#",
                                                     /*french*/ "une #grotte avec un trésor#"));
                                                  // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_HF_NEAR_MARKET_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                           /*german*/ "in einer #gewöhnlichen Grotte#",
                                                           /*french*/ "une #grotte avec un trésor#"));
                                                        // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_HF_OPEN_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                    /*german*/ "in einer #gewöhnlichen Grotte#",
                                                    /*french*/ "une #grotte avec un trésor#"));
                                                 // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_HF_SOUTHEAST_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                         /*german*/ "in einer #gewöhnlichen Grotte#",
                                                         /*french*/ "une #grotte avec un trésor#"));
                                                      // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_KF_STORMS_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                      /*german*/ "in einer #gewöhnlichen Grotte#",
                                                      /*french*/ "une #grotte avec un trésor#"));
                                                   // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_LW_NEAR_SHORTCUTS_GROTTO] = HintText(CustomMessage("a #generic grotto#",
                                                              /*german*/ "in einer #gewöhnlichen Grotte#",
                                                              /*french*/ "une #grotte avec un trésor#"));
                                                           // /*spanish*/una #cueva genérica#

    hintTextTable[RHT_HF_INSIDE_FENCE_GROTTO] = HintText(CustomMessage("a #single Upgrade Deku Scrub#",
                                                            /*german*/ "bei einem #Deku-Händler mit guter Ware#",
                                                            /*french*/ "une #grotte avec une peste Mojo#"));
                                                         // /*spanish*/una cueva con un #solitario mercader deku#

    hintTextTable[RHT_LW_SCRUBS_GROTTO] = HintText(CustomMessage("#2 Deku Scrubs# including an Upgrade one",
                                                      /*german*/ "bei #zwei Deku-Händlern mit guter Ware#",
                                                      /*french*/ "une #grotte avec deux pestes Mojo#"));
                                                   // /*spanish*/una cueva con #dos mercaderes deku#

    hintTextTable[RHT_COLOSSUS_GROTTO] = HintText(CustomMessage("2 Deku Scrubs",
                                                     /*german*/ "in einer Grotte mit #zwei Deku-Händlern#",
                                                     /*french*/ "une #grotte avec deux pestes Mojo#"));
                                                  // /*spanish*/una cueva con #dos mercaderes deku#

    hintTextTable[RHT_ZR_STORMS_GROTTO] = HintText(CustomMessage("2 Deku Scrubs",
                                                      /*german*/ "in einer Grotte mit #zwei Deku-Händlern#",
                                                      /*french*/ "une #grotte avec deux pestes Mojo#"));
                                                   // /*spanish*/una cueva con #dos mercaderes deku#

    hintTextTable[RHT_SFM_STORMS_GROTTO] = HintText(CustomMessage("2 Deku Scrubs",
                                                       /*german*/ "in einer Grotte mit #zwei Deku-Händlern#",
                                                       /*french*/ "une #grotte avec deux pestes Mojo#"));
                                                    // /*spanish*/una cueva con #dos mercaderes deku#

    hintTextTable[RHT_GV_STORMS_GROTTO] = HintText(CustomMessage("2 Deku Scrubs",
                                                      /*german*/ "in einer Grotte mit #zwei Deku-Händlern#",
                                                      /*french*/ "une #grotte avec deux pestes Mojo#"));
                                                   // /*spanish*/una cueva con #dos mercaderes deku#

    hintTextTable[RHT_LH_GROTTO] = HintText(CustomMessage("3 Deku Scrubs",
                                               /*german*/ "in einer Grotte mit #drei Deku-Händlern#",
                                               /*french*/ "une #grotte avec trois pestes Mojo#"));
                                            // /*spanish*/una cueva con #tres mercaderes deku#

    hintTextTable[RHT_DMC_HAMMER_GROTTO] = HintText(CustomMessage("3 Deku Scrubs",
                                                       /*german*/ "in einer Grotte mit #drei Deku-Händlern#",
                                                       /*french*/ "une #grotte avec trois pestes Mojo#"));
                                                    // /*spanish*/una cueva con #tres mercaderes deku#

    hintTextTable[RHT_GC_GROTTO] = HintText(CustomMessage("3 Deku Scrubs",
                                               /*german*/ "in einer Grotte mit #drei Deku-Händlern#",
                                               /*french*/ "une #grotte avec trois pestes Mojo#"));
                                            // /*spanish*/una cueva con #tres mercaderes deku#

    hintTextTable[RHT_LLR_GROTTO] = HintText(CustomMessage("3 Deku Scrubs",
                                                /*german*/ "in einer Grotte mit #drei Deku-Händlern#",
                                                /*french*/ "une #grotte avec trois pestes Mojo#"));
                                             // /*spanish*/una cueva con #tres mercaderes deku#

    hintTextTable[RHT_ZR_FAIRY_GROTTO] = HintText(CustomMessage("a small #Fairy Fountain#",
                                                     /*german*/ "in einem #Feen-Brunnen#",
                                                     /*french*/ "une #fontaine de fées#"));
                                                  // /*spanish*/una pequeña #fuente de hadas#

    hintTextTable[RHT_HF_FAIRY_GROTTO] = HintText(CustomMessage("a small #Fairy Fountain#",
                                                     /*german*/ "in einem #Feen-Brunnen#",
                                                     /*french*/ "une #fontaine de fées#"));
                                                  // /*spanish*/una pequeña #fuente de hadas#

    hintTextTable[RHT_SFM_FAIRY_GROTTO] = HintText(CustomMessage("a small #Fairy Fountain#",
                                                      /*german*/ "in einem #Feen-Brunnen#",
                                                      /*french*/ "une #fontaine de fées#"));
                                                   // /*spanish*/una pequeña #fuente de hadas#

    hintTextTable[RHT_ZD_STORMS_GROTTO] = HintText(CustomMessage("a small #Fairy Fountain#",
                                                      /*german*/ "in einem #Feen-Brunnen#",
                                                      /*french*/ "une #fontaine de fées#"));
                                                   // /*spanish*/una pequeña #fuente de hadas#

    hintTextTable[RHT_GF_STORMS_GROTTO] = HintText(CustomMessage("a small #Fairy Fountain#",
                                                      /*german*/ "in einem #Feen-Brunnen#",
                                                      /*french*/ "une #fontaine de fées#"));
                                                   // /*spanish*/una pequeña #fuente de hadas#

    /*--------------------------
    |      JUNK HINT TEXT      |
    ---------------------------*/

    hintTextTable[RHT_JUNK01] = HintText(CustomMessage("They say you must read the names of \"Special Deal\" shop items carefully.",
                                            /*german*/ "Man erzählt sich, daß man die \"Sonderangebote\" in Läden sorgfältig lesen sollte!",
                                            /*french*/ "Selon moi, les \"Offres spéciales\" sont parfois trompeuses... Lisez les attentivement!"));

    hintTextTable[RHT_JUNK02] = HintText(CustomMessage("They say that Zelda is a poor leader.",
                                            /*german*/ "Man erzählt sich, daß Zelda eine schlechte Anführerin sei.",
                                            /*french*/ "Selon moi, Zelda ne ferait pas un bon monarque."));

    hintTextTable[RHT_JUNK03] = HintText(CustomMessage("These hints can be quite useful. This is an exception.",
                                            /*german*/ "Die Hinweise von uns Mythensteinen können sehr nützlich sein. Dies ist eine Ausnahme.",
                                            /*french*/ "Ces indices sont très utiles, à l'exception de celui-ci."));

    hintTextTable[RHT_JUNK04] = HintText(CustomMessage("They say that the Lizalfos in Dodongo's Cavern like to play in lava.",
                                            /*german*/ "Man erzählt sich, daß die Echsalfos in Dodongos Höhle gerne in Lava spielen.",
                                            /*french*/ "Selon moi, les Lézalfos de la Caverne Dodongo aiment patauger dans la lave."));

    hintTextTable[RHT_JUNK05] = HintText(CustomMessage("They say that all the Zora drowned in Wind Waker.",
                                            /*german*/ "Man erzählt sich, daß alle Zoras in Wind Waker ertrunken wären.",
                                            /*french*/ "Selon moi, les Zoras se sont noyés dans Wind Waker."));

    hintTextTable[RHT_JUNK06] = HintText(CustomMessage("If Gorons eat rocks, does that mean I'm in danger?",
                                            /*german*/ "Da Goronen Steine essen… heißt das, ich sollte mir Sorgen machen?",
                                            /*french*/ "Ne dis pas au Gorons que je suis ici. Ils mangent des roches, tu sais!"));

    hintTextTable[RHT_JUNK07] = HintText(CustomMessage("'Member when Ganon was a blue pig?^I 'member.",
                                            /*german*/ "Weißte noch als Ganon nur n' blaues Schwein war?^Damals war alles besser. Kein unnötiger Schnickschnack!",
                                            /*french*/ "Dans mon temps, Ganon était un cochon bleu...^Pff! Les jeunes de nos jours, et leur Ganondorf!"));

    hintTextTable[RHT_JUNK08] = HintText(CustomMessage("One who does not have Triforce can't go in.",
                                            /*german*/ "Jemand, der nicht im Besitz des Triforce ist, darf nicht eintreten!",
                                            /*french*/ "Ceux sans Triforce doivent rebrousser chemin."));

    hintTextTable[RHT_JUNK09] = HintText(CustomMessage("Save your future, end the Happy Mask Salesman.",
                                            /*german*/ "Rette die Zukunft, stoppe den Maskenhändler solange Du noch kannst.",
                                            /*french*/ "Selon moi, tu t'éviteras des jours de malheur si tu vaincs le vendeur de masques..."));

    hintTextTable[RHT_JUNK10] = HintText(CustomMessage("Glitches are a pathway to many abilities some consider to be... Unnatural.",
                                            /*german*/ "Glitches ermöglichen viele Fähigkeiten, die manche als... unnatürlich betrachten würden.",
                                            /*french*/ "Les glitchs sont un moyen d'acquérir de nombreuses facultés considérées par certains comme... contraire "));

    hintTextTable[RHT_JUNK11] = HintText(CustomMessage("I'm stoned. Get it?",
                                            /*german*/ "Ich bin stoned. Verstehst du?",
                                            /*french*/ "J'ai été pétrifié.&Tu as compris?"));

    hintTextTable[RHT_JUNK12] = HintText(CustomMessage("Hoot! Hoot! Would you like me to repeat that?",
                                            /*german*/ "Uhuuu! Uhu! Soll ich mich noch einmal wiederholen?",
                                            /*french*/ "Hou hou! Veux-tu que je répète tout ça?"));

    hintTextTable[RHT_JUNK13] = HintText(CustomMessage("Gorons are stupid. They eat rocks.",
                                            /*german*/ "Goronen sind doof. Sie essen Felsen.",
                                            /*french*/ "Les Gorons sont des vraies têtes dures."));

    hintTextTable[RHT_JUNK14] = HintText(CustomMessage("They say that Lon Lon Ranch prospered under Ingo.",
                                            /*german*/ "Man erzählt sich, daß Basil der Lon Lon-Farm zu neuem Glanz verholfen hat.",
                                            /*french*/ "Selon moi, le Ranch Lon Lon était plus prospère sous Ingo."));

    hintTextTable[RHT_JUNK15] = HintText(CustomMessage("They say without the Lens of Truth, the Treasure Chest Mini-Game is a 1 out of 32 chance.^Good luck!",
                                            /*german*/ "Man erzählt sich, daß Du ohne das Auge der Wahrheit bei der Truhenlotterie nur eine 1-zu-32-Chance hast.^Na dann, viel Glück.",
                                            /*french*/ "Selon moi, les chances de gagner la Chasse-aux-Trésors sans Monocle de Vérité est de 1 chance sur 32.^Bonne chance!"));

    hintTextTable[RHT_JUNK16] = HintText(CustomMessage("Use bombs wisely.",
                                            /*german*/ "Setze Bomben mit Bedacht ein.",
                                            /*french*/ "Utilise les bombes avec précaution."));

    hintTextTable[RHT_JUNK17] = HintText(CustomMessage("They say that players who select the \"ON\" option for \"MOTION CONTROL\" are the real \"Zelda players!\"",
                                            /*german*/ "Man erzählt sich, daß Spieler, die die Option \"BEWEGUNGSSTEUERUNG\" auf \"EIN\" stellen, die wahren \"Zelda-Spieler\" sind.",
                                            /*french*/ "Selon moi, ceux qui utilisent les contrôles gyroscopiques sont les VRAIS joueurs."));

    hintTextTable[RHT_JUNK18] = HintText(CustomMessage("L2P @.",
                                            /*german*/ "Lern zu spielen, @.",
                                            /*french*/ "Arrête de lire les indices et joue comme un grand, @."));

    hintTextTable[RHT_JUNK19] = HintText(CustomMessage("I bet you'd like to have more bombs.",
                                            /*german*/ "Möchtest Du zur Strafe mehr Bomben tragen können?",
                                            /*french*/ "Je parie que tu veux plus de bombes."));

    hintTextTable[RHT_JUNK20] = HintText(CustomMessage("When all else fails, use Fire.",
                                            /*german*/ "Wenn alles andere scheitert, benutze Feuer.",
                                            /*french*/ "Quand rien ne marche, utilise le feu."));

    hintTextTable[RHT_JUNK21] = HintText(CustomMessage("Here's a hint, @. Don't be bad.",
                                            /*german*/ "Hier ein Tipp, @: Sei nicht so schlecht.",
                                            /*french*/ "Selon moi, la #Triforce# n'est pas dans le jeu... Duh!"));

    hintTextTable[RHT_JUNK22] = HintText(CustomMessage("Game Over. Return of Ganon.",
                                            /*german*/ "Game Over. Ganons Rückkehr.",
                                            /*french*/ "Partie terminée. RETour de Ganon."));

    hintTextTable[RHT_JUNK23] = HintText(CustomMessage("May the way of the Hero lead to the Triforce.",
                                            /*german*/ "Möge der Weg des Helden zum Triforce führen.",
                                            /*french*/ "Que le chemin du héros te mène à la Triforce."));

    hintTextTable[RHT_JUNK24] = HintText(CustomMessage("Can't find an item? Scan an Amiibo.",
                                            /*german*/ "Du kannst einen Gegenstand nicht finden? Scanne ein Amiibo.",
                                            /*french*/ "Tu cherches de quoi? Utilise un Amiibo!"));

    hintTextTable[RHT_JUNK25] = HintText(CustomMessage("They say this game has just a few glitches.",
                                            /*german*/ "Man erzählt sich, daß dieses Spiel nur ein paar Glitches hat.",
                                            /*french*/ "Selon moi, ce jeu est complètement exempt de glitchs."));

    hintTextTable[RHT_JUNK26] = HintText(CustomMessage("BRRING BRRING This is Ulrira. Wrong number?",
                                            /*german*/ "TUUUUT! TUUUUT! Ja? Hier Ulrira! Muss sich wohl verwählt haben.",
                                            /*french*/ "DRING DRING!! Pépé le Ramollo à l'appareil... Quoi? Faux numéro?"));

    hintTextTable[RHT_JUNK27] = HintText(CustomMessage("Tingle Tingle Kooloo Limpah!",
                                            /*german*/ "Tingle Tingle Kuuluu-Limpah!",
                                            /*french*/ "Tingle! Tingle! Kooloolin... Pah!"));

    hintTextTable[RHT_JUNK28] = HintText(CustomMessage("L is real 2401",
                                            /*german*/ "L is real 2401",
                                            /*french*/ "L is real 2401"));

    hintTextTable[RHT_JUNK29] = HintText(CustomMessage("They say that Ganondorf will appear in the next Mario Tennis.",
                                            /*german*/ "Man erzählt sich, daß Ganondorf im nächsten Mario Tennis erscheinen wird.",
                                            /*french*/ "Selon moi, Ganondorf sera la nouvelle recrue dans Mario Tennis."));

    hintTextTable[RHT_JUNK30] = HintText(CustomMessage("They say Medigoron sells the earliest Breath of the Wild demo.",
                                            /*german*/ "Man erzählt sich, daß Medigoron die früheste Breath of the Wild-Demo verkauft.",
                                            /*french*/ "Selon moi, Medigoron vend une démo de #Breath of the Wild#."));

    hintTextTable[RHT_JUNK31] = HintText(CustomMessage("Can you move me? I don't get great service here.",
                                            /*german*/ "Kannst Du mich bewegen? Ich habe schlechten Empfang.",
                                            /*french*/ "Peux-tu me déplacer? J'ai pas une bonne réception ici."));

    hintTextTable[RHT_JUNK32] = HintText(CustomMessage("They say if you use Strength on the truck, you can find Mew.",
                                            /*german*/ "Man erzählt sich, daß man mit Stärke, unter dem Truck Mew finden kann.",
                                            /*french*/ "Selon moi, #Mew# se trouve dessous le camion... Duh!"));

    hintTextTable[RHT_JUNK33] = HintText(CustomMessage("I'm a helpful hint Gossip Stone!^See, I'm helping.",
                                            /*german*/ "Ich bin ein hilfreicher Mythenstein!^Siehst du? Ich helfe.",
                                            /*french*/ "Salut! Je suis une pierre de bons conseils!^Tiens, tu vois? J'aide bien, hein?"));

    hintTextTable[RHT_JUNK34] = HintText(CustomMessage("Dear @, please come to the castle. I've baked a cake for you.&Yours truly, Princess Zelda.",
                                            /*german*/ "Lieber @!&Komm mich doch einmal im Schloß besuchen! Der Kuchen steht bereit!&In Freundschaft, Prinzessin Zelda.",
                                            /*french*/ "Mon très cher @:&Viens vite au château, je t'ai préparé&un délicieux gâteau...^À bientôt, Princesse Zelda"));

    hintTextTable[RHT_JUNK35] = HintText(CustomMessage("They say all toasters toast toast.",
                                            /*german*/ "Man erzählt sich, daß alle Toaster Toast toasten.",
                                            /*french*/ "Selon moi, les grille-pains grillent du pain."));

    hintTextTable[RHT_JUNK36] = HintText(CustomMessage("You thought it would be a useful hint, but it was me, junk hint!",
                                            /*german*/ "Du dachtest dies wäre ein nützlicher Hinweis. Doch hier bin ich, der Müll-Hinweis!",
                                            /*french*/ "Tu t'attendais à un bon indice... Mais c'était moi, un mauvais indice!"));

    hintTextTable[RHT_JUNK37] = HintText(CustomMessage("They say that quest guidance can be found at a talking rock.",
                                            /*german*/ "Man erzählt sich, daß man Quest-Hinweise bei einem sprechenden Stein finden kann.",
                                            /*french*/ "Selon moi, des #indices# se trouvent auprès d'une pierre parlante... Duh!"));

    hintTextTable[RHT_JUNK38] = HintText(CustomMessage("They say that the final item you're looking for can be found somewhere in Hyrule.",
                                            /*german*/ "Man erzählt sich, daß der letzte Gegenstand nachdem Du gerade suchst, irgendwo in Hyrule zu finden sei.",
                                            /*french*/ "Selon moi, le #dernier objet# se trouve quelque part dans Hyrule... Duh!"));

    hintTextTable[RHT_JUNK39] = HintText(CustomMessage("Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.",
                                            /*german*/ "Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.^Mweep.",
                                            /*french*/ "Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip.^Mwip."));

    hintTextTable[RHT_JUNK40] = HintText(CustomMessage("They say that Barinade fears Deku Nuts.",
                                            /*german*/ "Man erzählt sich, daß Barinade Angst vor Deku-Nüssen hat.",
                                            /*french*/ "Selon moi, Barinade a la frousse des noix Mojo."));

    hintTextTable[RHT_JUNK41] = HintText(CustomMessage("They say that Flare Dancers do not fear Goron-crafted blades.",
                                            /*german*/ "Man erzählt sich, daß Flammenderwische keine Angst vor goronengeschmiedeten Klingen haben.",
                                            /*french*/ "Selon moi, le danse-flamme n'a pas peur des armes de Goron."));

    hintTextTable[RHT_JUNK42] = HintText(CustomMessage("They say that Morpha is easily trapped in a corner.",
                                            /*german*/ "Man erzählt sich, daß man Morpha ganz leicht in die Ecke drängen kann.",
                                            /*french*/ "Selon moi, Morpha est facilement coincé."));

    hintTextTable[RHT_JUNK43] = HintText(CustomMessage("They say that Bongo Bongo really hates the cold.",
                                            /*german*/ "Man erzählt sich, daß Bongo Bongo die Kälte wirklich hasst.",
                                            /*french*/ "Selon moi, Bongo Bongo a facilement froid aux doigts."));

    hintTextTable[RHT_JUNK44] = HintText(CustomMessage("They say that your sword is most powerful when you put it away.",
                                            /*german*/ "Man erzählt sich, daß Dein Schwert am mächtigsten ist, wenn Du es wegsteckst.",
                                            /*french*/ "Selon moi, ton épée est à pleine puissance quand tu la rengaines."));

    hintTextTable[RHT_JUNK45] = HintText(CustomMessage("They say that bombing the hole Volvagia last flew into can be rewarding.",
                                            /*german*/ "Man erzählt sich, daß es sich lohnt das Loch zu bombardieren, in das Volvagia zuletzt geflogen ist.",
                                            /*french*/ "Selon moi, le trou où se creuse Volvagia est vulnérable aux bombes."));

    hintTextTable[RHT_JUNK46] = HintText(CustomMessage("They say that invisible ghosts can be exposed with Deku Nuts.",
                                            /*german*/ "Man erzählt sich, daß unsichtbare Geister mit Deku-Nüssen sichtbar gemacht werden können.",
                                            /*french*/ "Selon moi, des fantômes invisibles apparaissent avec des noix Mojo."));

    hintTextTable[RHT_JUNK47] = HintText(CustomMessage("They say that the real Phantom Ganon is bright and loud.",
                                            /*german*/ "Man erzählt sich, daß der wahre Phantom-Ganon durch Helligkeit und Lärm auffällt.",
                                            /*french*/ "Selon moi, le vrai spectre de Ganon est clair et bruyant."));

    hintTextTable[RHT_JUNK48] = HintText(CustomMessage("They say that walking backwards is very fast.",
                                            /*german*/ "Man erzählt sich, daß rückwärts laufen sehr schnell ist.",
                                            /*french*/ "Selon moi, tu fais marche arrière très rapidement pour un héros."));

    hintTextTable[RHT_JUNK49] = HintText(CustomMessage("They say Ingo is not very good at planning ahead.",
                                            /*german*/ "Man erzählt sich, daß Basil nicht besonders gut darin ist vorauszuplanen.",
                                            /*french*/ "Selon moi, Ingo ne fait pas un très bon geôlier."));

    hintTextTable[RHT_JUNK50] = HintText(CustomMessage("You found a spiritual Stone! By which I mean, I worship Nayru.",
                                            /*german*/ "Du hast einen Heiligen Stein gefunden!^Also... eigentlich heißt das nur, daß ich Nayru verehre.",
                                            /*french*/ "Vous avez trouvé une Pierre Ancestrale! En effet, je vénère la déesse Hylia."));

    hintTextTable[RHT_JUNK51] = HintText(CustomMessage("Open your eyes.^Open your eyes.^Wake up, @.",
                                            /*german*/ "Öffne die Augen...^Öffne die Augen...^Wach auf, @.",
                                            /*french*/ "Réveille-toi...^Réveille-toi.^Ouvre les yeux, @."));

    hintTextTable[RHT_JUNK52] = HintText(CustomMessage("They say that the Nocturne of Shadow can bring you very close to Ganon.",
                                            /*german*/ "Man erzählt sich, daß die Nocturne des Schattens dich Ganon sehr nahe bringen kann.",
                                            /*french*/ "Selon moi, le Nocturne de l'Ombre peut t'amener très près de Ganon."));

    hintTextTable[RHT_JUNK53] = HintText(CustomMessage("They say that Twinrova always casts the same spell the first three times.",
                                            /*german*/ "Man erzählt sich, daß Twinrova die ersten drei Male immer denselben Zauber benutzt.",
                                            /*french*/ "Selon moi, le Duo Maléfique lance toujours les mêmes trois premiers sorts."));

    hintTextTable[RHT_JUNK54] = HintText(CustomMessage("They say that the nightly builds may be unstable.",
                                            /*german*/ "Man erzählt sich, daß die \"Nightly Builds\" instabil sein könnten.",
                                            /*french*/ "Selon moi, les \"nightly builds\" peuvent être instables."));

    hintTextTable[RHT_JUNK55] = HintText(CustomMessage("You're playing a Randomizer. I'm randomized!^Here's a random number:  #4#.&Enjoy your Randomizer!",
                                            /*german*/ "Du spielst einen Randomizer. Ich wurde zufällig generiert!^Hier ist eine zufällige Zahl:  #4#.&Viel Spaß mit dem Randomizer!",
                                            /*french*/ "Tu joues à un randomizer. Je suis aléatoire!^Voici un nombre aléatoire: #4#.&Bonne partie!"));

    hintTextTable[RHT_JUNK56] = HintText(CustomMessage("They say Ganondorf's bolts can be reflected with glass or steel.",
                                            /*german*/ "Man erzählt sich, daß man Ganondorfs Blitze mit Glas oder Stahl reflektieren kann.",
                                            /*french*/ "Selon moi, les éclairs de Ganon se reflètent sur l'acier et le verre."));

    hintTextTable[RHT_JUNK57] = HintText(CustomMessage("They say Ganon's tail is vulnerable to nuts, arrows, swords, explosives, hammers...^...sticks, seeds, "
                                                       "boomerangs...^...rods, shovels, iron balls, angry bees...",
                                            /*german*/ "Man erzählt sich, Ganons Schwanz sei verwundbar durch Nüsse, Pfeile, Schwerter...^...Explosives, Hämmer, "
													   "Stöcke...^...Kerne, Bumerangs, Schaufeln, Eisenkugeln, wütende Bienen...",
                                            /*french*/ "Selon moi, la queue de Ganon est vulnérable aux noix, flèches, épées, bombes, marteaux...^...bâtons, "
                                                       "graines, boomerangs...^...baguettes, pelles, boulets de fer, abeilles enragées..."));

    hintTextTable[RHT_JUNK58] = HintText(CustomMessage("They say that you're wasting time reading this hint, but I disagree. Talk to me again!",
                                            /*german*/ "Man erzählt sich, daß Du deine Zeit mit dem Lesen dieses Hinweises verschwendest, aber ich bin anderer Meinung. Sprich noch einmal mit mir.",
                                            /*french*/ "Selon moi... tu sais quoi? Parle-moi encore, et je te le dirai!"));

    hintTextTable[RHT_JUNK59] = HintText(CustomMessage("They say Ganondorf knows where to find the instrument of his doom.",
                                            /*german*/ "Man erzählt sich, daß Ganondorf weiß, wo man das Instrument seines Untergangs finden kann.",
                                            /*french*/ "Selon moi, Ganondorf sait où il a caché son point faible."));

    hintTextTable[RHT_JUNK60] = HintText(CustomMessage("I heard @ is pretty good at Zelda.",
                                            /*german*/ "Ich habe gehört, daß @ ziemlich gut in Zelda Spielen ist.",
                                            /*french*/ "Apparemment, @ est super bon à Zelda."));

    hintTextTable[RHT_JUNK61] = HintText(CustomMessage("Hi @, we've been trying to reach you about your car's extended warranty. ",
                                            /*german*/ "Hallo @, wir haben versucht, Dich wegen der erweiterten Garantie für Dein Auto zu erreichen.",
                                            /*french*/ "Bonjour, @. Vous avez une voiture? Vous savez, nous offrons des assurances abordables..."));

    hintTextTable[RHT_JUNK62] = HintText(CustomMessage("They say that it's actually possible to beat the running man.",
                                            /*german*/ "Man erzählt sich, daß es tatsächlich möglich sei, den Marathon-Läufer zu besiegen.",
                                            /*french*/ "Selon moi, il est possible de battre le coureur.&Donc, tu prends ton arc, et..."));

    hintTextTable[RHT_JUNK63] = HintText(CustomMessage("They say this hint makes more sense in other languages.",
                                            /*german*/ "Man erzählt sich, daß dieser Hinweis in anderen Sprachen mehr Sinn macht.",
                                            /*french*/ "Selon moi, ces indices auraient pu être mieux traduits... Duh!"));

    // ^ Junk hints above are from 3drando
    // v Junk hints below are new to soh rando

    // Please keep hints to stuff related to ship directly, or to Nintendo/Zelda related stuff.
    // And nothing that's super obscure that no one's going to understand.

    hintTextTable[RHT_JUNK64] = HintText(CustomMessage("They say Greg is special.",
                                            /*german*/ "Man erzählt sich, daß Greg etwas Besonderes ist.",
                                            /*french*/ "Selon moi, Greg est spécial."));

    hintTextTable[RHT_JUNK65] = HintText(CustomMessage("They say the longer the Goron's neck, the wiser they are.",
                                            /*german*/ "Man erzählt sich, daß ein Gorone umso weiser wird, je länger sein Hals ist.",
                                            /*french*/ "Selon moi, plus le cou des Gorons est long, plus ils sont sage." ));

    hintTextTable[RHT_JUNK66] = HintText(CustomMessage("They say this ship is what all true gamers strive for.",
                                            /*german*/ "Man erzählt sich, daß dieses \"Ship\" das ist, wonach alle echten Gamer streben.",
                                            /*french*/ "Selon moi, cette version du port est ce pour quoi luttent tous les vrais gamers."));

    hintTextTable[RHT_JUNK67] = HintText(CustomMessage("They say that Glowsticks can be found in the Raveyard.",
                                            /*german*/ "Man erzählt sich, daß Leuchtstäbe auf dem Partyfriedhof zu finden wären.",
                                            /*french*/ "Selon moi, on peut trouver des Bâtons Lumineux sur le dancefloor du cimetière."));

    hintTextTable[RHT_JUNK68] = HintText(CustomMessage("They say @'s uncle works for Nintendo.",
                                            /*german*/ "Man erzählt sich, daß @'s Onkel bei Nintendo arbeitet.",
                                            /*french*/ "Selon moi, l'oncle de @ travaille chez Nintendo."));

    hintTextTable[RHT_JUNK69] = HintText(CustomMessage("They say pulling all gravestones in the graveyard leads to something magical.",
                                            /*german*/ "Man erzählt sich, daß das Ziehen aller Grabsteine auf dem Friedhof zu etwas Magischem führe.",
                                            /*french*/ "Selon moi, tirer toutes les tombes du Cimetière déclanche un truc magique."));

    hintTextTable[RHT_JUNK70] = HintText(CustomMessage("They say holding L while pausing makes you win the game.",
                                            /*german*/ "Man erzählt sich, daß man das Spiel gewinnt, wenn man L gedrückt hält, während man pausiert.",
                                            /*french*/ "Selon moi, maintenir L pendant que vous appuyez sur START vous permet de terminer le jeu."));

    hintTextTable[RHT_JUNK71] = HintText(CustomMessage("They say @'s body is ready.",
                                            /*german*/ "Man erzählt sich... nein weißt Du was, ich habe keine Lust mehr.",
                                            /*french*/ "Selon moi, ce junk hint ne se traduirait pas bien en français."));

    /*--------------------------
    |     DUNGEON HINT TEXT    |
    ---------------------------*/


    hintTextTable[RHT_DEKU_TREE] = HintText(CustomMessage("Deku Tree",
                                               /*german*/ "im Deku-Baum",
                                               /*french*/ "l'Arbre Mojo"),
                                            // /*spanish*/el Gran Árbol Deku
                                            {},
                                            {CustomMessage("an ancient tree",
                                                /*german*/ "in einem antiken Baum",
                                                /*french*/ "le vieil arbre")});
                                             // /*spanish*/un ancestral árbol

    hintTextTable[RHT_DODONGOS_CAVERN] = HintText(CustomMessage("Dodongo's Cavern",
                                                     /*german*/ "in Dodongos Höhle",
                                                     /*french*/ "la Caverne Dodongo"),
                                                  // /*spanish*/la Cueva de los Dodongos
                                                  {},
                                                  {CustomMessage("an immense cavern",
                                                      /*german*/ "in einer riesigen Kaverne",
                                                      /*french*/ "l'immense caverne")});
                                                   // /*spanish*/una descomunal cueva

    hintTextTable[RHT_JABU_JABUS_BELLY] = HintText(CustomMessage("Jabu-Jabu's Belly",
                                                      /*german*/ "in Jabu-Jabus Bauch",
                                                      /*french*/ "le Ventre de Jabu-Jabu"),
                                                   // /*spanish*/tripa de Jabu-Jabu
                                                   {},
                                                   {CustomMessage("the belly of a deity",
                                                       /*german*/ "im Bauche einer Gottheit",
                                                       /*french*/ "le ventre d'un gardien")});
                                                    // /*spanish*/la tripa de cierta deidad

    hintTextTable[RHT_FOREST_TEMPLE] = HintText(CustomMessage("Forest Temple",
                                                   /*german*/ "im Waldtempel",
                                                   /*french*/ "le Temple de la Forêt"),
                                                // /*spanish*/el Templo del Bosque
                                                {},
                                                {CustomMessage("a deep forest",
                                                    /*german*/ "in dunklen Wäldern",
                                                    /*french*/ "la profonde forêt")});
                                                 // /*spanish*/las profundidades del bosque

    hintTextTable[RHT_FIRE_TEMPLE] = HintText(CustomMessage("Fire Temple",
                                                 /*german*/ "im Feuertempel",
                                                 /*french*/ "le Temple du Feu"),
                                              // /*spanish*/el Templo del Fuego
                                              {},
                                              {CustomMessage("a high mountain",
                                                  /*german*/ "auf hohem Berge",
                                                  /*french*/ "la grande montagne")});
                                               // /*spanish*/una alta montaña

    hintTextTable[RHT_WATER_TEMPLE] = HintText(CustomMessage("Water Temple",
                                                  /*german*/ "im Wassertempel",
                                                  /*french*/ "le Temple de l'Eau"),
                                               // /*spanish*/el Templo del Agua
                                               {},
                                               {CustomMessage("a vast lake",
                                                   /*german*/ "in einem riesigen See",
                                                   /*french*/ "le vaste lac")});
                                                // /*spanish*/un lago inmenso

    hintTextTable[RHT_SPIRIT_TEMPLE] = HintText(CustomMessage("Spirit Temple",
                                                   /*german*/ "im Geistertempel",
                                                   /*french*/ "le Temple de l'Esprit"),
                                                // /*spanish*/el Templo del Espíritu
                                                {},
                                                {CustomMessage("the goddess of the sand",
                                                    /*german*/ "innerhalb der Göttin des Sandes",
                                                    /*french*/ "la déesse des sables")});
                                                 // /*spanish*/la diosa de las arenas

    hintTextTable[RHT_SHADOW_TEMPLE] = HintText(CustomMessage("Shadow Temple",
                                                   /*german*/ "im Schattentempel",
                                                   /*french*/ "le Temple de l'Ombre"),
                                                // /*spanish*/el Templo de las Sombras
                                                {},
                                                {CustomMessage("the house of the dead",
                                                    /*german*/ "an der Stätte der Toten",
                                                    /*french*/ "la maison des morts")});
                                                 // /*spanish*/la casa de la muerte

    hintTextTable[RHT_ICE_CAVERN] = HintText(CustomMessage("Ice Cavern",
                                                /*german*/ "in der Eishöhle",
                                                /*french*/ "la caverne de glace"),
                                             // /*spanish*/la caverna de hielo
                                             {},
                                             {CustomMessage("a frozen maze",
                                                 /*german*/ "in einem gefrorenen Labyrinth",
                                                 /*french*/ "le dédale glacé")});
                                              // /*spanish*/un gélido laberinto

    hintTextTable[RHT_BOTTOM_OF_THE_WELL] = HintText(CustomMessage("Bottom of the Well",
                                                        /*german*/ "auf dem Grund des Brunnens",
                                                        /*french*/ "le fonds du Puits"),
                                                     // /*spanish*/el fondo del pozo
                                                     {},
                                                     {CustomMessage("a shadow\'s prison",
                                                         /*german*/ "im Gefängnis eines Dämons",
                                                         /*french*/ "la prison d'une ombre")});
                                                      // /*spanish*/la prisión de las sombras

    hintTextTable[RHT_GERUDO_TRAINING_GROUND] = HintText(CustomMessage("Gerudo Training Ground",
                                                            /*german*/ "in der Gerudo-Trainingsarena",
                                                            /*french*/ "le Gymnase Gerudo"),
                                                         // /*spanish*/el Centro de Instrucción Gerudo
                                                         {},
                                                         {CustomMessage("the test of thieves",
                                                             /*german*/ "bei der Prüfung der Diebe",
                                                             /*french*/ "l'épreuve des voleurs")});
                                                          // /*spanish*/la prueba de las bandidas

    hintTextTable[RHT_GANONS_CASTLE] = HintText(CustomMessage("Inside Ganon's Castle",
                                                   /*german*/ "in Ganons Schloß",
                                                   /*french*/ "l'intérieur du Château de Ganon"),
                                                // /*spanish*/el interior del Castillo de Ganon
                                                {},
                                                {CustomMessage("a conquered citadel",
                                                    /*german*/ "in einer gefallenen Zitadelle",
                                                    /*french*/ "la citadelle assiégée")});
                                                 // /*spanish*/una conquistada ciudadela

    /*--------------------------
    |      BOSS HINT TEXT      |
    ---------------------------*/
//RANDOTODO check the beginning and end on the french translations
    hintTextTable[RHT_QUEEN_GOHMA] = HintText(CustomMessage("They say that #Queen Gohma# holds #[[1]]#.",
                                                 /*german*/ "Man erzählt sich, daß #Königin Gohma# #[[1]]# hielte.",
                                                 /*french*/ "Selon moi, la #Reine Gohma# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                              // /*spanish*/la #Reina Goma# porta #[[1]]#.
                                              {},
                                              {CustomMessage("They say that the #Parasitic Armored Arachnid# holds #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß der #gepanzerte Spinnenparasit# #[[1]]# hielte.",
                                                  /*french*/ "Selon moi, le #monstre insectoïde géant# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                               // /*spanish*/el #arácnido parasitario acorazado# porta #[[1]]#.

    hintTextTable[RHT_KING_DODONGO] = HintText(CustomMessage("They say that #King Dodongo# holds #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß #König Dodongo# #[[1]]# hielte.",
                                                  /*french*/ "Selon moi, le #Roi Dodongo# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                               // /*spanish*/el #Rey Dodongo# porta #[[1]]#.
                                               {},
                                               {CustomMessage("They say that the #Infernal Dinosaur# holds #[[1]]#.",
                                                   /*german*/ "Man erzählt sich, daß der #Infernosaurus# #[[1]]# hielte.",
                                                   /*french*/ "Selon moi, le #dinosaure infernal# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                                // /*spanish*/el #dinosaurio infernal# porta #[[1]]#.

    hintTextTable[RHT_BARINADE] = HintText(CustomMessage("They say that #Barinade# holds #[[1]]#.",
                                              /*german*/ "Man erzählt sich, daß #Barinade# #[[1]]# hielte.",
                                              /*french*/ "Selon moi, #Barinade# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                           // /*spanish*/#Barinade# porta #[[1]]#.
                                           {},
                                           {CustomMessage("They say that the #Bio-Electric Anemone# holds #[[1]]#.",
                                               /*german*/ "Man erzählt sich, daß der #elektroterrestrische Biotentakel# #[[1]]# hielte.",
                                               /*french*/ "Selon moi, l'#anémone bioélectrique# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                            // /*spanish*/la #anémona bioeléctrica# porta #[[1]]#.

    hintTextTable[RHT_PHANTOM_GANON] = HintText(CustomMessage("They say that #Phantom Ganon# holds #[[1]]#.",
                                                   /*german*/ "Man erzählt sich, daß #Phantom-Ganon# #[[1]]# hielte.",
                                                   /*french*/ "Selon moi, #Ganon Spectral# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                                // /*spanish*/#Ganon Fantasma# porta #[[1]]#.
                                                {},
                                                {CustomMessage("They say that the #Evil Spirit from Beyond# holds #[[1]]#.",
                                                    /*german*/ "Man erzählt sich, daß das #reitende Unheil# #[[1]]# hielte.",
                                                    /*french*/ "Selon moi, l'#esprit maléfique de l'au-delà# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                                 // /*spanish*/el #espíritu maligno de ultratumba# porta #[[1]]#.

    hintTextTable[RHT_VOLVAGIA] = HintText(CustomMessage("They say that #Volvagia# holds #[[1]]#.",
                                              /*german*/ "Man erzählt sich, daß #Volvagia# #[[1]]# hielte.",
                                              /*french*/ "Selon moi, #Volvagia# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                           // /*spanish*/#Volvagia# porta #[[1]]#.
                                           {},
                                           {CustomMessage("They say that the #Subterranean Lava Dragon# holds #[[1]]#.",
                                               /*german*/ "Man erzählt sich, daß der #subterrane Lavadrachoid# #[[1]]# hielte.",
                                               /*french*/ "Selon moi, le #dragon des profondeurs# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                            // /*spanish*/el #dragón de lava subterráneo# porta #[[1]]#.

    hintTextTable[RHT_MORPHA] = HintText(CustomMessage("They say that #Morpha# holds #[[1]]#.",
                                            /*german*/ "Man erzählt sich, daß #Morpha# #[[1]]# hielte.",
                                            /*french*/ "Selon moi, #Morpha# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                         // /*spanish*/#Morpha# porta #[[1]]#.
                                         {},
                                         {CustomMessage("They say that the #Giant Aquatic Amoeba# holds #[[1]]#.",
                                             /*german*/ "Man erzählt sich, daß der #aquamöbe Wassertentakel# #[[1]]# hielte.",
                                             /*french*/ "Selon moi, l'#amibe aquatique géante# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                          // /*spanish*/la #ameba acuática gigante# porta #[[1]]#.

    hintTextTable[RHT_BONGO_BONGO] = HintText(CustomMessage("They say that #Bongo Bongo# holds #[[1]]#.",
                                                 /*german*/ "Man erzählt sich, daß #Bongo Bongo# #[[1]]# hielte.",
                                                 /*french*/ "Selon moi, #Bongo Bongo# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                              // /*spanish*/#Bongo Bongo# porta #[[1]]#.
                                              {},
                                              {CustomMessage("They say that the #Phantom Shadow Beast# holds #[[1]]#.",
                                                  /*german*/ "Man erzählt sich, daß die #bestialische Schattenmonstrosität# #[[1]]# hielte.",
                                                  /*french*/ "Selon moi, le #monstre de l'ombre# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                               // /*spanish*/la #alimaña oscura espectral# porta #[[1]]#.

    hintTextTable[RHT_TWINROVA] = HintText(CustomMessage("They say that #Twinrova# holds #[[1]]#.",
                                              /*german*/ "Man erzählt sich, daß #Twinrova# #[[1]]# hielte.",
                                              /*french*/ "Selon moi, #Twinrova# possède #[[1]]#.", {QM_RED, QM_GREEN}),
                                           // /*spanish*/#Birova# porta #[[1]]#.
                                           {},
                                           {CustomMessage("They say that the #Sorceress Sisters# hold #[[1]]#.",
                                               /*german*/ "Man erzählt sich, daß die #höllische Hexenarmada# #[[1]]# hielte.",
                                               /*french*/ "Selon moi, #les sorcières jumelles# possède #[[1]]#.", {QM_RED, QM_GREEN})});
                                            // /*spanish*/las #hermanas hechiceras# portan #[[1]]#.

    hintTextTable[RHT_GIFT_FROM_RAURU] = HintText(CustomMessage("They say that the #Sage of Light# gifts @ #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß der #Weise des Lichts# #[[1]]# schenke.",
                                                     /*french*/ "Selon moi, #le Sage de la Lumière# donne #[[1]]#.", {QM_RED, QM_GREEN}),
                                                 {},
                                                 {CustomMessage("They say that #a former owl# gifts @ #[[1]]#.",
                                                     /*german*/ "Man erzählt sich, daß #eine einstige Eule# #[[1]]# schenke.",
                                                     /*french*/ "Selon moi, #un ancien hibou# donne #[[1]]#.", {QM_RED, QM_GREEN})});

    /*--------------------------
    |     BRIDGE HINT TEXT     |
    ---------------------------*/

    hintTextTable[RHT_BRIDGE_OPEN_HINT] = HintText(CustomMessage("$lThe awakened ones have #already created a bridge# to the castle where the evil dwells.^",
                                                      /*german*/ "$lDie Weisen haben #bereits&eine Brücke zum Portal von&Ganons Schloß gelegt#...^",
                                                      /*french*/ "$lLes êtres de sagesse ont#déjà créé un pont# vers le repaire du mal.^",
                                                                 {QM_LBLUE}, {}, TEXTBOX_TYPE_BLUE));
                                                   // /*spanish*/$lLos sabios #ya habrán creado un puente#&al castillo, de donde emana el mal.^


    hintTextTable[RHT_BRIDGE_VANILLA_HINT] = HintText(CustomMessage("$6The awakened ones require the #Shadow and Spirit Medallions# as well as the #Light Arrows#.^",
                                                        /*german*/ "$6Die Weisen, die darauf warten,&daß der Held das #Amulett des Schattens, Amulett der Geister#&und die #Licht-Pfeile# sammelt.^",
                                                        /*french*/ "$6Les êtres de sagesse attendront le héros muni des #Médaillons de l'Ombre et l'Esprit# et des #Flèches de Lumière#.^",
                                                                   {QM_RED, QM_YELLOW}, {}, TEXTBOX_TYPE_BLUE));
                                                     // /*spanish*/$6Los sabios aguardarán a que el héroe obtenga tanto el #Medallón de las Sombras y el del Espíritu# junto
                                                                // a la #flecha de luz#.^

    hintTextTable[RHT_BRIDGE_STONES_HINT] = HintText(CustomMessage("$0The awakened ones will await for the Hero to collect #[[d]] Spiritual Stone||s|#.^",
                                                        /*german*/ "$0Die Weisen werden darauf&warten, daß der Held&#[[d]] |Heiligen Stein|Heilige Steine|# sammelt.^",
                                                        /*french*/ "$0Les êtres de sagesse attendront le héros muni de #[[d]] |Pierre Ancestrale|Pierres Ancestrales|#.^",
                                                                   {QM_BLUE}, {}, TEXTBOX_TYPE_BLUE));
                                                     // /*spanish*/$0Los sabios aguardarán a que el héroe&obtenga #[[d]] |piedra espiritual|piedras espirituales|#.^

    hintTextTable[RHT_BRIDGE_MEDALLIONS_HINT] = HintText(CustomMessage("$8The awakened ones will await for the Hero to collect #[[d]] Medallion||s|#.^",
                                                            /*german*/ "$8Die Weisen werden darauf&warten, daß der Held&#[[d]] Amulett||e|# sammelt.^",
                                                            /*french*/ "$8Les êtres de sagesse attendront le héros muni de #[[d]] Médaillon||s|#.^",
                                                                       {QM_RED}, {}, TEXTBOX_TYPE_BLUE));
                                                         // /*spanish*/$8Los sabios aguardarán a que el héroe&obtenga #[[d]] |medallón|medallones|#.^

    hintTextTable[RHT_BRIDGE_REWARDS_HINT] = HintText(CustomMessage("$CThe awakened ones will await for the Hero to collect #[[d]]# |#Spiritual Stone# or #Medallion#|"
                                                                     "#Spiritual Stones# and #Medallions#|.^",
                                                         /*german*/ "$CDie Weisen werden darauf&warten, daß der Held&#[[d]]# |#Heiligen Stein# oder #Amulett#|"
                                                                     "#Heilige Steine# oder #Amulette#| sammelt.^",
                                                         /*french*/ "$CLes êtres de sagesse attendront le héros muni de #[[d]]# |#Pierre Ancestrale# ou #Médaillon#"
                                                                     "|#Pierres Ancestrales# ou #Médaillons#|.^",
                                                                     {QM_YELLOW, QM_BLUE, QM_RED}, {}, TEXTBOX_TYPE_BLUE));
                                                      // /*spanish*/$CLos sabios aguardarán a que el héroe obtenga #[[d]]# |#piedra espiritual# o #medallón#|
                                                                    //#piedras espirtuales# y #medallones#|.^

    hintTextTable[RHT_BRIDGE_DUNGEONS_HINT] = HintText(CustomMessage("$mThe awakened ones will await for the Hero to conquer #[[d]] Dungeon||s|#.^",
                                                          /*german*/ "$mDie Weisen werden darauf&warten, daß der Held&#[[d]] Labyrinth||e|# abschließt.^",
                                                          /*french*/ "$mLes êtres de sagesse attendront la conquête de #[[d]] Donjon||s|#.^",
                                                                     {QM_PINK}, {}, TEXTBOX_TYPE_BLUE));
                                                       // /*spanish*/$mLos sabios aguardarán a que el héroe complete #[[d]] mazmorra||s|#.^

    hintTextTable[RHT_BRIDGE_TOKENS_HINT] = HintText(CustomMessage("$sThe awakened ones will await for the Hero to collect #[[d]] Gold Skulltula Token||s|#.^",
                                                        /*german*/ "$sDie Weisen werden darauf&warten, daß der Held&#[[d]] Skulltula-Symbol||e|# sammelt.^",
                                                        /*french*/ "$sLes êtres de sagesse attendront le héros muni de #[[d]] Symbole||s| de Skulltula d'or#.^",
                                                                   {QM_YELLOW}, {}, TEXTBOX_TYPE_BLUE));
                                                     // /*spanish*/$sLos sabios aguardarán a que el héroe obtenga #[[d]] símbolo||s| de skulltula dorada#.^

    hintTextTable[RHT_BRIDGE_GREG_HINT] = HintText(CustomMessage("$gThe awakened ones will await for the Hero to find #Greg#.^",
	                                                  /*german*/ "$gDie Weisen werden darauf&warten, daß der Held&#Greg# findet.^",
                                                      /*french*/ "$gLes êtres de sagesse attendront le héros muni de #Greg#.^",
                                                                 {QM_GREEN}, {}, TEXTBOX_TYPE_BLUE));


    /*--------------------------
    | GANON BOSS KEY HINT TEXT |
    ---------------------------*/

    hintTextTable[RHT_GANON_BK_START_WITH_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be #given from the start#.^",
                                                              /*german*/ "$bUnd der #Schlüssel des Bösen#, wird #von Anfang an, im Besitz des Helden# sein.^",
                                                              /*french*/ "$bAussi, la #clé du Malin# sera #possession même du héros#.^",
                                                                         {QM_PINK, QM_LBLUE}));
                                                           // /*spanish*/$bY obtendrás la llave del #señor del mal# desde el #inicio#.^

    hintTextTable[RHT_GANON_BK_VANILLA_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be kept in a big chest #inside its tower#.^",
                                                           /*german*/ "$bUnd der #Schlüssel des Bösen#,&wird in der großen Truhe&#im Teufelsturm# zu finden sein.^",
                                                           /*french*/ "$bAussi, la #clé du Malin# sera encoffrée #dans sa tour#.^",
                                                                      {QM_PINK, QM_LBLUE}));
                                                        // /*spanish*/$bY la llave del #señor del mal# aguardará en un gran cofre de #su torre#.^

    hintTextTable[RHT_GANON_BK_OWN_DUNGEON_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be hidden somewhere #inside its castle#.^",
                                                               /*german*/ "$bUnd der #Schlüssel des Bösen#, wird irgendwo #in Ganons Schloß# zu finden sein.^",
                                                               /*french*/ "$bAussi, la #clé du Malin# sera cachée #dans son vaste château#.^",
                                                                          {QM_PINK, QM_PINK}));
                                                            // /*spanish*/$bY la llave del #señor del mal# aguardará en #algún lugar de su castillo#.^

    hintTextTable[RHT_GANON_BK_OVERWORLD_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be hidden #outside of dungeons# in Hyrule.^",
                                                             /*german*/ "$bUnd der #Schlüssel des Bösen#, wird irgendwo #in Hyrule, außerhalb von Labyrinthen# zu finden sein.^",
                                                             /*french*/ "$bAussi, la #clé du Malin# se trouve #hors des donjons# d'Hyrule.^",
                                                                        {QM_PINK, QM_GREEN}));
                                                          // /*spanish*/$bY la llave del #señor del mal# aguardará #fuera de las mazmorras# de Hyrule.^

    hintTextTable[RHT_GANON_BK_ANY_DUNGEON_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be hidden #inside a dungeon# in Hyrule.^",
                                                               /*german*/ "$bUnd der #Schlüssel des Bösen#, wird irgendwo #in Hyrule, innerhalb eines Labyrinths# zu finden sein.^",
                                                               /*french*/ "$bAussi, la #clé du Malin# se trouve #dans un donjon# d'Hyrule.^",
                                                                          {QM_PINK, QM_PINK}));
                                                            // /*spanish*/$bY la llave del #señor del mal# aguardará #en una mazmorra# de Hyrule.^

    hintTextTable[RHT_GANON_BK_ANYWHERE_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be hidden somewhere&#in Hyrule#.^",
                                                            /*german*/ "$bUnd der #Schlüssel des Bösen#, wird irgendwo #in Hyrule# zu finden sein.^",
                                                            /*french*/ "$bAussi, la #clé du Malin# se trouve quelque part #dans Hyrule#.^",
                                                                       {QM_PINK, QM_BLUE}));
                                                         // /*spanish*/$bY la llave del #señor del mal# aguardará en #cualquier lugar de Hyrule#.^

    hintTextTable[RHT_GANON_BK_TRIFORCE_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be given to the Hero once the #Triforce## is completed.^",
                                                            /*german*/ "$bUnd der #Schlüssel des Bösen# wird verliehen, sobald das #Triforce# vervollständigt wurde.^",
                                                            /*french*/ "$bAussi, la #clé du Malin# se&révèlera une fois la #Triforce#&assemblée.^",
                                                                       {QM_PINK, QM_YELLOW}));
                                                         // /*spanish*/$bY el héroe recibirá la llave del #señor del mal# cuando haya completado la #Trifuerza#.^

    hintTextTable[RHT_GANON_BK_SKULLTULA_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by the cursed rich man once #100 Gold Skulltula Tokens# are retrieved.^",
                                                             /*german*/ "$bUnd der #Schlüssel des Bösen# wird von einem verfluchten reichen Mann verliehen, sobald #100 Skulltula-Symbole# gesammelt wurden.^",
                                                             /*french*/ "$bAussi, la #clé du Malin# sera&donnée par l'homme maudit une fois que #100 Symboles de Skulltula d'or# auront été trouvés.^",
                                                                        {QM_PINK, QM_YELLOW}));
                                                          // /*spanish*/$bY el rico maldito entregará la llave&del #señor de mal# tras obtener&100 símbolos de skulltula dorada#.^

    hintTextTable[RHT_LACS_VANILLA_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once the #Shadow and Spirit Medallions# are retrieved.^",
                                                       /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #die Amulette des Schattens und der Geister# geborgen wurden.^",
                                                       /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois que les #Médaillons de l'Ombre et de l'Esprit# seront récupérés.^",
                                                                  {QM_PINK, QM_YELLOW, QM_RED}));
                                                    // /*spanish*/$bY #Zelda# entregará la llave del #señor del mal# tras obtener #el medallón de las sombras y del espíritu#.^

    hintTextTable[RHT_LACS_MEDALLIONS_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once #[[d]] Medallion|# is|s# are| retrieved.^",
                                                          /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #[[d]] Amulett|# geborgen wurde|e# geborgen wurden|.^",
                                                          /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois |qu' #[[d]] Médaillon# aura été récupéré|que #[[d]] Médaillons# auront été récupérés|.^",
                                                                     {QM_PINK, QM_YELLOW, QM_RED}));
                                                       // /*spanish*/$bY #Zelda# entregará la llave&del #señor del mal# tras obtener #[[d]] |medallón|medallones|#.^

    hintTextTable[RHT_LACS_STONES_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once #[[d]] Spiritual Stone|# is|s# are| retrieved.^",
                                                      /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #[[d]] Heilige|r Stein# geborgen wurde| Steine# geborgen wurden|.^",
                                                      /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois |qu' #[[d]] Pierre Ancestrale# aura été&récupérée|que #[[d]] Pierres Ancestrales# auront été récupérées|.^",
                                                                 {QM_PINK, QM_YELLOW, QM_BLUE}));
                                                   // /*spanish*/$bY #Zelda# entregará la llave del #señor del mal# tras obtener #[[d]] piedra| espiritual|s espirituales|#.^

    hintTextTable[RHT_LACS_REWARDS_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once #[[d]]# #Spiritual Stone|# or #Medallion# is|s# and #Medallions# are| retrieved.^",
                                                       /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #[[d]]# #Heilige|r Stein# oder #Amulett#&geborgen wurde| Steine# oder #Amulette#&geborgen wurden|.^",
                                                       /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois qu|' #[[d]]# #Pierre Ancestrale# ou #[[d]] Médaillon# sera récupéré|e&#[[d]]# #Pierres Ancestrales# et&#Médaillons# seront récupérés|.^",
                                                                  {QM_PINK, QM_YELLOW, QM_YELLOW, QM_BLUE, QM_RED}));
                                                    // /*spanish*/$bY #Zelda# entregará la llave del #señor del mal# tras obtener #[[d]]# piedra| espiritual o medallón|s espirituales o medallones|#.^

    hintTextTable[RHT_LACS_DUNGEONS_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once #[[d]] Dungeon|# is|s# are| conquered.^",
                                                        /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #[[d]] Labyrinth|# abgeschloßen wurde|e# abgeschloßen wurden|.^",
                                                        /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois qu|' #[[d]] donjon #sera conquis|e #[[d]] donjons# seront conquis|.^",
                                                                  {QM_PINK, QM_YELLOW, QM_PINK}));
                                                     // /*spanish*/$bY #Zelda# entregará la llave del #señor del mal# tras completar #[[d]] mazmorra||s|#.^

    hintTextTable[RHT_LACS_TOKENS_HINT] = HintText(CustomMessage("$bAnd the #evil one#'s key will be provided by #Zelda# once #[[d]] Gold Skulltula Token|# is|s# are| retrieved.^",
                                                      /*german*/ "$bUnd der #Schlüssel des Bösen# wird von #Zelda# verliehen, sobald #[[d]] Skulltula-Symbol|# gesammelt wurde|e# gesammelt wurden|.^",
                                                      /*french*/ "$bAussi, la #clé du Malin# sera fournie par #Zelda# une fois |qu' #[[d]] symbole de Skulltula d'or #sera récupuéré"
                                                                 "|que &#[[d]] symboles de Skulltula d'or&#seront recupérés|.^",
                                                                  {QM_PINK, QM_YELLOW, QM_YELLOW}));
                                                    // /*spanish*/$bY #Zelda# entregará la llave del #señor del mal# tras obtener #[[d]] símbolo
                                                              // ||s| de skulltula dorada#.^

    /*--------------------------
    |     TRIAL HINT TEXT      |
    ---------------------------*/

    hintTextTable[RHT_SIX_TRIALS] = HintText(CustomMessage("They say that #Ganon's Tower# is protected by a powerful barrier.",
                                                /*german*/ "Man erzählt sich, daß der #Teufelsturm# von einer mächtigen Barriere geschützt sei.",
                                                /*french*/ "Selon moi, #la Tour de Ganon# est protégée par une puissante barrière", {QM_PINK}));
                                             // /*spanish*/Según dicen, la #torre de Ganon# está protegida por una poderosa barrera

    hintTextTable[RHT_ZERO_TRIALS] = HintText(CustomMessage("They say that Sheik dispelled the barrier around #Ganon's Tower#.",
                                                 /*german*/ "Man erzählt sich, Shiek habe die Barriere um den #Teufelsturm# aufgelöst.",
                                                 /*french*/ "Selon moi, Sheik a dissipé la barrière autour de #la Tour de Ganon#", {QM_YELLOW}));
                                              // /*spanish*/Según dicen, Sheik disipó la barrera alrededor de la #torre de Ganon#.

    hintTextTable[RHT_TRIAL_OFF] = HintText(CustomMessage("They say that #[[1]]# was dispelled by Sheik.",
                                               /*german*/ "Man erzählt sich, daß #[[1]]# von Shiek aufgelöst worden sei.",
                                               /*french*/ "Selon moi, #[[1]]# a été dissipée par Sheik.", {QM_YELLOW}));
                                            // /*spanish*/Según dicen, #[[1]]# se disipó gracias a Sheik.

    hintTextTable[RHT_TRIAL_ON] = HintText(CustomMessage("They say that #[[1]]# protects Ganons Tower.",
                                              /*german*/ "Man erzählt sich, daß #[[1]]# den Teufelsturm schütze.",
                                              /*french*/ "Selon moi, #[[1]]# protège la Tour de Ganon.", {QM_PINK}));
                                           // /*spanish*/Según dicen, #[[1]]# protege la torre de Ganon

    hintTextTable[RHT_LIGHT_TRIAL] = HintText(CustomMessage("the Light Trial",
                                                 /*german*/ "das Licht-Siegel",
                                                 /*french*/ "l'épreuve de la Lumière"));
                                              // /*spanish*/la prueba de la luz

    hintTextTable[RHT_FOREST_TRIAL] = HintText(CustomMessage("the Forest Trial",
                                                  /*german*/ "das Wald-Siegel",
                                                  /*french*/ "l'épreuve de la Forêt"));
                                               // /*spanish*/la prueba del bosque

    hintTextTable[RHT_FIRE_TRIAL] = HintText(CustomMessage("the Fire Trial",
                                                /*german*/ "das Feuer-Siegel",
                                                /*french*/ "l'épreuve du Feu"));
                                             // /*spanish*/la prueba del fuego

    hintTextTable[RHT_WATER_TRIAL] = HintText(CustomMessage("the Water Trial",
                                                 /*german*/ "das Wasser-Siegel",
                                                 /*french*/ "l'épreuve de l'Eau"));
                                              // /*spanish*/la prueba del agua

    hintTextTable[RHT_SPIRIT_TRIAL] = HintText(CustomMessage("the Spirit Trial",
                                                  /*german*/ "das Geister-Siegel",
                                                  /*french*/ "l'épreuve de l'Esprit"));
                                               // /*spanish*/la prueba del espíritu

    hintTextTable[RHT_SHADOW_TRIAL] = HintText(CustomMessage("the Shadow Trial",
                                                  /*german*/ "das Schatten-Siegel",
                                                  /*french*/ "l'épreuve de l'Ombre"));
                                               // /*spanish*/la prueba de las sombras

    /*--------------------------
    |        ALTAR TEXT        |
    ---------------------------*/

    hintTextTable[RHT_CHILD_ALTAR_STONES] = HintText(CustomMessage("3 Spiritual Stones found in Hyrule...^$0#[[1]]#...^$1#[[2]]#...^$2#[[3]]#...^",
                                                        /*german*/ "Drei Heilige Steine, zu finden in Hyrule...^$0#[[1]]#...^$1#[[2]]#...^$2#[[3]]#...^",
                                                        /*french*/ "Les trois Pierres Ancestrales cachées&dans Hyrule...$0#[[1]]#...^$1#[[2]]#...^$2#[[3]]#...^",
                                                        {QM_GREEN, QM_RED, QM_BLUE}, {true, true, true}, TEXTBOX_TYPE_BLUE));
                                                    // /*spanish*/ Tres piedras espirituales halladas por Hyrule...$0#[[1]]#...^$1#[[2]]#...^$2#[[3]]#...^

    hintTextTable[RHT_CHILD_ALTAR_TEXT_END_DOTOPEN] = HintText(CustomMessage("$oYe who may become a Hero...&The path to the future is open...",
                                                                  /*german*/ "$oJener auf dem Weg des Helden...&Der Pfad zur Zukunft sei geöffnet...",
                                                                  /*french*/ "$oÀ celui qui a quête de devenir&héros...&Le futur vous accueille béant...", TEXTBOX_TYPE_BLUE));
                                                               // /*spanish*/$oPara aquel que se convierta en el héroe...&La puerta al futuro está a su disposición...

    hintTextTable[RHT_CHILD_ALTAR_TEXT_END_DOTSONGONLY] = HintText(CustomMessage("$cYe who may become a Hero...&Stand with the Ocarina and&play the Song of Time.",
                                                                      /*german*/ "$cJener auf dem Weg des Helden...&Nehme er seine Okarina zur Hand und spiele hier die Hymne der Zeit.",
                                                                      /*french*/ "$cÀ celui qui a quête de devenir&héros...&Portez l'Ocarina et jouez&le chant du temps.", TEXTBOX_TYPE_BLUE));
                                                                   // /*spanish*/$cPara aquel que se convierta en el héroe...&Tome la ocarina y&entone la Canción del Tiempo.

    hintTextTable[RHT_CHILD_ALTAR_TEXT_END_DOTCLOSED] = HintText(CustomMessage("$iYe who may become a Hero...&Offer the spiritual stones and&play the Song of Time.",
                                                                    /*german*/ "$iJener mit den drei Heiligen Steinen nehme er seine Okarina zur Hand und spiele hier die Hymne der Zeit.",
                                                                    /*french*/ "$iÀ celui qui a quête de devenir&héros... Présentez les Pierres&Ancestrales et jouez&le chant du temps.", TEXTBOX_TYPE_BLUE));
                                                                 // /*spanish*/$iPara aquel que se convierta en el héroe...&Tome las piedras espirituales y&entone la Canción del Tiempo.

    hintTextTable[RHT_ADULT_ALTAR_MEDALLIONS] = HintText(CustomMessage("An awakening voice from the Sacred Realm will call those destined to be Sages, who dwell in the #five temples#.^"
                                                                       "$8#[[1]]#...^$3#[[2]]#...^$4#[[3]]#...^$5#[[4]]#...^$6#[[5]]#...^$7#[[6]]#...^" ,
                                                            /*german*/ "Beherrscht das Böse die Welt, verweilen die Weisen, erweckt von der Stimme des Heiligen Reiches, noch in den #fünf Tempeln#.^"
                                                                       "$8#[[1]]#...^$3#[[2]]#...^$4#[[3]]#...^$5#[[4]]#...^$6#[[5]]#...^$7#[[6]]#...^",
                                                            /*french*/ "Quand le mal aura triomphé, une voix du Saint Royaume appellera ceux cachés dans les #cinq temples#, destinés^à être Sages.^"
                                                                       "$8#[[1]]#...^$3#[[2]]#...^$4#[[3]]#...^$5#[[4]]#...^$6#[[5]]#...^$7#[[6]]#...^",
                                                                       { QM_RED, QM_YELLOW, QM_GREEN, QM_RED, QM_BLUE, QM_YELLOW, QM_PINK}, {true, true, true, true, true, true}, TEXTBOX_TYPE_BLUE));
                                                         // /*spanish*/Cuando el mal lo impregne todo, desde el Reino Sagrado surgirá una voz que hará despertar a los sabios que moran en los #cinco templos#.^
                                                                    // $8#[[1]]#...^$3#[[2]]#...^$4#[[3]]#...^$5#[[4]]#...^$6#[[5]]#...^$7#[[6]]#...^

    hintTextTable[RHT_ADULT_ALTAR_TEXT_END] = HintText(CustomMessage("$kTogether with the Hero of Time, the awakened ones will return the light of peace to the world...",
                                                        /*german*/ "$kZusammen mit dem Auserwählten werden diese ihre Kräfte&einsetzen, um der Welt den&Frieden wiederzugeben.",
                                                        /*french*/ "$kEnsemble avec le Héros du Temps, ces Sages emprisonneront le mal et réinstaureront la lumière de paix dans le monde..."));
                                                        // /*spanish*/Con el Héroe del Tiempo, aquellos&que despierten detendrán el mal y&volverán al mundo de luz la paz...

    /*--------------------------
    |     Static Item Hints     |
    ---------------------------*/

    hintTextTable[RHT_GANONDORF_HINT_LA_ONLY] = HintText(CustomMessage("Ha ha ha... You'll never beat me by reflecting my lightning bolts and unleashing the arrows from #[[1]]#!",
                                                            /*german*/ "Ha ha ha... Du wirst mich niemals besiegen, indem Du meine Blitze zurückschlägst...^und die Pfeile entfesselst, die #[[1]]# zu finden sind!",
                                                            /*french*/ "Ha ha ha... Pauvre fou! Tu ne pourras jamais me vaincre sans les flèches que j'ai cachées dans #[[1]]#!",
                                                                       {QM_RED}));
                                                         // /*spanish*/Ja, ja, ja... Nunca me derrotarás reflejando mis esferas de energía y desplegando la flecha de luz de #[[1]]#!

    hintTextTable[RHT_GANONDORF_HINT_MS_ONLY] = HintText(CustomMessage("Ha ha ha... You'll never defeat me, drop a castle on me and finish me off with the sacred blade from #[[2]]#!",
                                                            /*german*/ "Ha ha ha... Du wirst mich niemals besiegen, ein Schloß auf mich werfen...^und mich mit der legendären Klinge niederstrecken, die #[[2]]# versteckt ist!",
                                                            /*french*/ "Ha ha ha... Pauvre fou! Tu ne pourras jamais me vaincre sans l'Épée de Légende cachée dans #[[2]]#!",
                                                                       {QM_RED}));

    hintTextTable[RHT_GANONDORF_HINT_LA_AND_MS] = HintText(CustomMessage("Ha ha ha... You'll never beat me by reflecting my lightning bolts and unleashing the arrows from #[[1]]#!"
                                                                         "^And even if you do, you'll never find the legendary blade hidden in #[[2]]#!",
                                                              /*german*/ "Ha ha ha... Du wirst mich niemals besiegen, indem Du meine Blitze zurückschlägst...^und die Pfeile entfesselst, die #[[1]]# zu finden sind!"
                                                                         "^Und selbst wenn Du es tust,&wirst Du niemals die legendäre&Klinge finden, die #[[2]]# versteckt ist!",
                                                              /*french*/ "Ha ha ha... Pauvre fou! Tu ne pourras jamais me vaincre sans les flèches que j'ai cachées dans #[[1]]#!"
                                                                         "^Et même si tu les trouves, tu ne touveras jamais l'Épée de Légende cachée dans #[[2]]#!",
                                                                         {QM_RED, QM_RED}));
                                                           // /*spanish*/Ja, ja, ja... Nunca me derrotarás reflejando mis esferas de energía y desplegando la flecha de luz de #[[1]]#!
                                                                      // ^E incluso si lo haces, nunca encontrarás la espada legendaria escondida en #[[2]]#!

    hintTextTable[RHT_SHEIK_HINT_LA_ONLY] = HintText(CustomMessage("I overheard Ganondorf say that he misplaced the #Light Arrows# in #[[1]]#.",
                                                        /*german*/ "Ich habe Ganondorf sagen hören, daß er die #Licht-Pfeile# #[[1]]# verlegt hat.",
                                                        /*french*/ "J'ai entendu dire que Ganondorf aurait caché les #Flèches de Lumière# dans #[[1]]#.",
                                                                   {QM_YELLOW, QM_RED}));

    hintTextTable[RHT_DAMPE_DIARY] = HintText(CustomMessage("Whoever reads this, please enter #[[1]]#. I will let you have my #stretching, shrinking keepsake#.^I'm waiting for you.&--Dampé",
                                                 /*german*/ "Wer immer dies liest, der möge #[[1]]# nach meinem #langen, kurzen Schatz# suchen.^Ich warte!&Boris",
                                                 /*french*/ "Toi qui lit ce journal, rends-toi dans #[[1]]#. Et peut-être auras-tu droit à mon précieux #trésor#.^Je t'attends...&--Igor",
                                                            {QM_RED, QM_RED}));

    hintTextTable[RHT_GREG_HINT] = HintText(CustomMessage("By the way, if you're interested, I saw the shiniest #Green Rupee# somewhere in #[[1]]#.^It's said to have #mysterious powers#...^But then, it could just be another regular rupee.&Oh well.",
                                               /*german*/ "Übrigens, falls es Dich interessiert, ich habe #[[1]]# den glänzendsten #Grünen Rubin# gesehen.^Er soll #mysteriöse Kräfte# haben...^Aber naja, es könnte auch einfach nur ein normaler Rubin sein.",
                                               /*french*/ "Au fait, si ça t'intéresse, j'ai aperçu le plus éclatant des #Rubis Verts# quelque part à #[[1]]#.^On dit qu'il possède des #pouvoirs mystérieux#...^Mais bon, ça pourrait juste être un autre rubis ordinaire.",
                                                          {QM_GREEN, QM_RED, QM_RED}));

    hintTextTable[RHT_SARIA_TALK_HINT] = HintText(CustomMessage("Did you feel the #surge of magic# recently? A mysterious bird told me it came from #[[1]]#.^You should check that place out, @!",
                                                     /*german*/ "Hast Du kürzlich den #Magieschub# gespürt? Ein geheimnisvoller Vogel meinte, daß er #[[1]]# am stärksten zu spüren war.^Du solltest Dir diesen Ort mal ansehen, @!",
                                                     /*french*/ "As-tu récemment ressenti une vague de #puissance magique#? Un mystérieux hibou m'a dit qu'elle provenait du #[[1]]#.^Tu devrais aller y jeter un coup d'oeil, @!",
                                                               {QM_GREEN, QM_RED}));

    hintTextTable[RHT_SARIA_SONG_HINT] = HintText(CustomMessage("Did you feel the #surge of magic# recently? A mysterious bird told me it came from #[[1]]#.^You should check that place out, @!\x0B",
                                                     /*german*/ "Hast Du kürzlich den #Magieschub# gespürt? Ein geheimnisvoller Vogel meinte, daß er #[[1]]# am stärksten zu spüren war.^Du solltest Dir diesen Ort mal ansehen, @!\x0B",
                                                     /*french*/ "As-tu récemment ressenti une vague de #puissance magique#? Un mystérieux hibou m'a dit qu'elle provenait du #[[1]]#.^Tu devrais aller y jeter un coup d'oeil, @!\x0B",
                                                                {QM_GREEN, QM_RED}, {}, TEXTBOX_TYPE_BLUE));

    hintTextTable[RHT_LOACH_HINT] = HintText(CustomMessage("What?^You wanna know about the&%rHyrule Loach%w?^It's a big fish, but it's so rare that I'll give my %g[[1]]%w to anyone who catches it. Seriously!",
	                                            /*german*/ "Was?^Du willst etwas über die&%rhylianische Forelle%w wissen?&Es ist ein riesiger Fisch,&der unfassbar selten ist!^Wenn Du mir eine bringst, |springt|springen| für Dich&%g[[1]]%w dabei raus.&Ganz im Ernst!",
                                                /*french*/ "Quoi?&Tu veux en savoir plus sur le&%rBrochet d'Hyrule%w?^C'est un gros poisson, mais il&est si rare que je donne&%g[[1]]%w&à celui qui l'attrape.^Ouais, j'suis sérieux!",
                                                           {QM_RED}));

    hintTextTable[RHT_FISHING_POLE_HINT] = HintText(CustomMessage("^If I remember correctly, I lost it somewhere in #[[1]]#...&Let me know if you find it!",
	                                                    /*german*/ "Wenn ich mich recht erinnere,&habe ich sie irgendwo&#[[1]]#&verloren...&Sag mir Bescheid, wenn Du sie findest!",
													    /*french*/ "Si je me souviens bien, il me&semble que je l'ai perdue&quelque part dans&#[[1]]#...^Fais-moi signe si jamais&tu la trouves!",
                                                                  {QM_RED}));

    /*--------------------------
    |    Static Entrance Hint   |
    ---------------------------*/

    hintTextTable[RHT_WARP_SONG] = HintText(CustomMessage("Warp to&#[[1]]#?&" + TWO_WAY_CHOICE() + "#OK&No#",
                                               /*german*/ "Das Ziel liegt&#[[1]]#!&" + TWO_WAY_CHOICE() + "#Ja!&Nein!#",
                                               /*french*/ "Se téléporter vers&#[[1]]#?&" + TWO_WAY_CHOICE() + "#OK!&Non#",
                                                          {QM_RED, QM_GREEN}));

   /*--------------------------
    |   STATIC LOCATION HINTS   |
    ---------------------------*/

    hintTextTable[RHT_HBA_HINT_SIGN] = HintText(CustomMessage("#Horseback Archery# Range Prizes:&1000: #[[1]]#&1500: #[[2]]#^@'s Record: #" + CustomMessage::POINTS(HS_HORSE_ARCHERY) + "#",
												   /*german*/ "#Bogenschießen zu Pferde#&Schießstandpreise:&1000: #[[1]]#^1500: #[[2]]#^@'s Rekord: #" + CustomMessage::POINTS(HS_HORSE_ARCHERY) + "#",
                                                   /*french*/ "Récompenses de l'#Archerie Montée#:&1000: #[[1]]#&1500: #[[2]]#^Record de @: #" + CustomMessage::POINTS(HS_HORSE_ARCHERY) + "#",
                                                              {QM_RED, QM_GREEN, QM_GREEN, QM_GREEN}, {}, TEXTBOX_TYPE_WOODEN));

    hintTextTable[RHT_HBA_HINT_NOT_ON_HORSE] = HintText(CustomMessage("Hey, rookie!&Come back on your #horse# and take on the #Horseback Archery# challenge!^"
                                                                      "Impress me with a high score of 1000 to win a #[[1]]# or score 1500 for #[[2]]#!",
                                                           /*german*/ "Hey, Kleiner! Das ist die %rArena für Bogenschießen zu Pferde%w.^"
                                                                      "Wenn Du reiten kannst, beeindrucke mich mit #1.000 Punkten# und gewinne #[[1]]#!^Knackst Du #1.500 Punkte# gibt es #[[2]]#!",
                                                           /*french*/ "Hé, l'nouveau!&Reviens avec ton #cheval# et essaie notre #Terrain d'Archerie Montée#^"
                                                                      "Impressionne-moi avec un score de 1000 pour gagner #[[1]]# ou atteins 1500 pour #[[2]]#!",
                                                                      {QM_RED, QM_RED, QM_GREEN, QM_GREEN}));

    hintTextTable[RHT_HBA_HINT_INITIAL] = HintText(CustomMessage("Hey, rookie!&Want to take on the #Horseback Archery# challenge?^"
                                                                 "Impress me with a high score of 1000 to win a #[[1]]# or score 1500 for #[[2]]#!\x0B",
                                                      /*german*/ "Hey, Kleiner! Wie wäre es mit einer Runde %rBogenschießen zu Pferde%w!?^"
                                                                 "Wenn Du mich mit #1.000 Punkten# beeindruckst, erhältst Du #[[1]]#!^Knackst Du #1.500 Punkte# gibt es #[[2]]#!\x0B",
                                                      /*french*/ "Hé, l'nouveau!&Tu veux essayer notre #Terrain d'Archerie Montée#^"
                                                                 "Impressionne-moi avec un score de 1000 pour gagner #[[1]]# ou atteins 1500 pour #[[2]]#!\x0B",
                                                                 {QM_RED, QM_GREEN, QM_GREEN}));

    hintTextTable[RHT_HBA_HINT_HAVE_1000] = HintText(CustomMessage("Hey, newcomer!&Want to take on the #Horseback Archery# challenge?^"
                                                                   "Prove yourself to be a horsemaster by scoring 1500 points to win #[[2]]#!\x0B",
                                                        /*german*/ "Hey, Kleiner! Wie wäre es mit einer Runde %rBogenschießen zu Pferde%w!?^"
                                                                   "Versuche doch jetzt, #1.500 Punkte# zu erreichen für #[[2]]#!\x0B",
                                                        /*french*/ "Hé, l'nouveau!&Tu veux essayer notre #Terrain d'Archerie Montée#^"
                                                                   "Prouve que tu es un véritable cavalier en obtenant 1500 points pour gagner #[[2]]#!\x0B",
                                                                   {QM_RED, QM_GREEN}));

    hintTextTable[RHT_MALON_HINT_HOW_IS_EPONA] = HintText(CustomMessage("@! You should come back with Epona and try to beat my time on the #Obstacle Course#!^"
                                                                        "If you beat my time, I'll give you my favourite #cow# Elsie and her toy #[[1]]#!",
                                                             /*german*/ "@! Warum kommst Du nicht mit Epona zurück und versuchst Dich an dem #Hindernisparcours#?^"
                                                                        "Gelingt es Dir den Rekord zu brechen, bekommst Du meine #Lieblingskuh# Elsie^und ihr Lieblingsspielzeug, #[[1]]#!",
                                                             /*french*/ "@! Tu devrais revenir avec Epona et essayer de battre mon temps sur le #Parcours d'Obstacles#!^"
                                                                        "Si tu bats mon temps, je te donnerai ma vache préférée, Elsie, ainsi que son jouet #[[1]]#!",
                                                                        {QM_RED, QM_BLUE, QM_GREEN}));

    hintTextTable[RHT_MALON_HINT_OBSTICLE_COURSE] = HintText(CustomMessage("How about trying the #Obstacle Course?# If you beat my time I'll let you keep my favourite #cow# Elsie and her toy #[[1]]#!^"
                                                                           "Challenge the #Obstacle Course?#&\x1B&#Let's go&No thanks#",
                                                                /*german*/ "Warum versuchst Du Dich nicht mit Epona an dem #Hindernisparcours#?^"
                                                                           "Gelingt es Dir den Rekord zu brechen, bekommst Du meine #Lieblingskuh# Elsie^und ihr Lieblingsspielzeug, #[[1]]#!^"
                                                                           "Wie sieht's aus?&Möchtest Du es versuchen?\x1B&#Ja!&Nein!#",
                                                                /*french*/ "Que dirais-tu d'essayer le #Parcours d'Obstacles#? Si tu bats mon temps, je te donnerai ma vache préférée, Elsie, et son jouet #[[1]]#!^"
                                                                           "Tenter le #Parcours d'Obstacles#?&\x1B&#Allons-y&Non merci#",
                                                                           {QM_RED, QM_BLUE, QM_GREEN, QM_RED, QM_GREEN}));

    hintTextTable[RHT_MALON_HINT_TURNING_EVIL] = HintText(CustomMessage("@? Is that you? ^If I ran the ranch, I'd build an #Obstacle Course#, and whoever gets the best time would win a #cow#!^"
                                                                        "Elsie loves sharing her #[[1]]# with new people, It'll be fun!^...But Ingo won't let me...",
                                                             /*german*/ "@? Bist Du das?^Wenn ich die Farm leiten würde, würde ich einen #Hindernisparcours# bauen, und wer die beste Zeit schafft, gewinnt eine #Kuh#!^"
                                                                        "Elsie würde es lieben, #[[1]]# mit neuen Leuten zu teilen, es würde Spaß machen!^...Aber Basil läßt mich nicht...",
                                                             /*french*/ "@? C'est toi?^Si je dirigeais le ranch, je construirais un #Parcours d'Obstacles#, et celui qui obtiendrait le meilleur temps gagnerait une #vache#!^"
                                                                        "Elsie adore partager son #[[1]]# avec de nouvelles personnes, ce serait amusant!^... Mais Ingo ne me laisse pas faire...",
                                                                        {QM_RED, QM_BLUE, QM_GREEN}));

    hintTextTable[RHT_MALON_HINT_INGO_TEMPTED] = HintText(CustomMessage("@! You should come back in the morning and try to beat my time on the #Obstacle Course#!^"
                                                                        "If you beat my time, I'll give you my favourite #cow# Elsie and her toy #[[1]]#!",
                                                             /*german*/ "@! Warum kommst Du nicht morgens wieder und versuchst Dich an dem #Hindernisparcours#?^"
                                                                        "Gelingt es Dir den Rekord zu brechen, bekommst Du meine #Lieblingskuh# Elsie^und ihr Lieblingsspielzeug, #[[1]]#!",
                                                             /*french*/ "@! Tu devrais revenir le matin et essayer de battre mon temps sur le #Parcours d'Obstacles#!^"
                                                                        "Si tu bats mon temps, je te donnerai ma vache préférée, Elsie, ainsi que son jouet #[[1]]#!",
                                                                        {QM_RED, QM_BLUE, QM_GREEN}));

    hintTextTable[RHT_CHICKENS_HINT] = HintText(CustomMessage("You! Please!&Bring my Cucco's back to my pen!&I'll give you my #[[1]]#!",
                                                   /*german*/ "Was soll ich nur tun?! Bitte!&Hilf mir, die Hühner wieder einzusammeln!^Ich gebe Dir auch #[[1]]#, wenn du mir hilfst!",
                                                   /*french*/ "Mes Cocottes sont perdues!&Dépose toutes les cocottes dans cet enclos!&Je te donnerai #[[1]]#!",
                                                              {QM_GREEN}));

    hintTextTable[RHT_BIG_POES_HINT] = HintText(CustomMessage("You have #\x1E\x01 Poe Points#! Reach 1000 and you'll get a #[[1]]#!",
                                                   /*german*/ "Du hast nun #\x1E\x01 Punkte# auf dem Nachtschwärmer-Konto gut!^"
                                                              "Hast Du %y1.000 Punkte%w gesammelt dann |wartet|warten| #[[1]]# auf Dich! Hehehe...",
                                                   /*french*/ "Tu as #\x1E\x01 Points d'Âme#! Atteins 1000 et tu recevras #[[1]]#!",
                                                              {QM_YELLOW, QM_GREEN}));

    hintTextTable[RHT_BIGGORON_HINT] = HintText(CustomMessage("Arrrrrre you here to claim my finest #[[1]]#? Shoooooow me your #Claim Check#.",
                                                   /*german*/ "Arrrrr, bist Du hier, um etwas von mir schmieden zu lassen? Zeig mir das #Zertifikat# und #[[1]]# |gehört|gehören| Dir!",
                                                   /*french*/ "Eeeeeees-tu ici pour réclaaaaaamer mon plus beau #[[1]]#? Moooooontre-moi ton #Certificat#.",
                                                              {QM_GREEN, QM_RED}));

    hintTextTable[RHT_FROGS_HINT] = HintText(CustomMessage("Some frogs holding #[[1]]# are looking at you from underwater...",
                                                /*german*/ "Unter Wasser gibt es Frösche, die #[[1]]# bei sich haben und Dich neugierig beobachten...",
                                                /*french*/ "Des grenouilles se trouvant sous l'eau vous fixent attentivement, tenant fermement #[[1]]#.",
                                                           {QM_GREEN}));

    hintTextTable[RHT_OOT_HINT] = HintText(CustomMessage("Bring the #Spiritual Stones# to the past so you can receive #[[1]]# from Zelda and learn #[[2]]#!",
                                              /*german*/ "Bringe die #Heiligen Steine# in die Vergangenheit, damit Du #[[1]]# von...^"
                                                         "Prinzessin Zelda erhältst und #[[2]]# lernst, um Hyrule vor dem Untergang zu bewahren!",
                                              /*french*/ "Amène les #Pierres Spirituelles# dans le passé et Zelda te donnera #[[1]]# et t'apprendra #[[2]]# !",
                                                         {QM_BLUE, QM_GREEN, QM_GREEN}));

    hintTextTable[RHT_SKULLS_HINT] = HintText(CustomMessage("Yeaaarrgh! I'm cursed!!^Please save me by destroying #[[d]] Spiders of the Curse# and I will give you my #[[1]]#!",
                                                 /*german*/ "Aarrrgh! Ich bin verflucht!^Bitte rette mich, indem Du&#[[d]] Skulltulas# zerstörst und ich&werde Dir dafür #[[1]]# geben!",
                                                 /*french*/ "Yeaaarrgh! Je suis maudit!^Détruit encore #[[d]] Araignées de la Malédiction# et j'aurai quelque chose à te donner! #([[1]])#",
                                                            {QM_YELLOW, QM_GREEN}));

    hintTextTable[RHT_MASK_SHOP_HINT] = HintText(CustomMessage("Some young scrubs in the #Deku Theatre# love seeing Masks!^"
                                                               "They'll give you #[[1]]# if you show them the #Skull Mask#, and #[[2]]# if you show them the #Mask of Truth#!",
                                                    /*german*/ "Ein paar junge Dekus in der #Waldbühne# lieben es, Masken anzuschauen!^"
                                                               "Sie geben Dir #[[1]]#, wenn Du ihnen die #Schädel-Maske# zeigst,^und #[[2]]#, wenn Du ihnen die #Maske des Wissens# zeigst!",
                                                    /*french*/ "De jeunes Pestes Mojo qui se trouvent dans le #Théâtre Mojo# adorent voir des masques !^"
                                                               "Elles te donneront #[[1]]# si tu leur montres le #Masque de Mort#, et #[[2]]# si tu leur montres le #Masque de Vérité#.",
                                                               {QM_GREEN, QM_GREEN, QM_RED, QM_GREEN, QM_RED}));

    /*--------------------------
    |      GANON LINE TEXT     |
    ---------------------------*/

    hintTextTable[RHT_GANON_JOKE01] = HintText(CustomMessage("Oh! It's @.&I was expecting someone called Sheik.&Do you know what happened to them?",
                                                  /*german*/ "Oh! Du bist es @.&Ich habe eigentlich jemanden namens Shiek erwartet. Weißt Du, was mit ihm passiert ist?",
                                                  /*french*/ "Ah, c'est @.&J'attendais un certain Sheik.&Tu sais ce qui lui est arrivé?"));
                                               // /*spanish*/¡Oh! Pero si es @.&Estaba esperando a alguien llamado Sheik. ¿Sabes qué puede haberle pasado?

    hintTextTable[RHT_GANON_JOKE02] = HintText(CustomMessage("I knew I shouldn't have put the key on the other side of my door.",
                                                  /*german*/ "Ich wußte, ich hätte den Schlüssel nicht draußen liegen lassen sollen.",
                                                  /*french*/ "J'aurais dû garder la clé ici. Hélas..."));
                                               // /*spanish*/Sabía que no tendría que haber dejado la llave al otro lado de la puerta.

    hintTextTable[RHT_GANON_JOKE03] = HintText(CustomMessage("Looks like it's time for a round of tennis.",
                                                  /*german*/ "Sieht so aus, als wäre es Zeit für eine Runde Tennis.",
                                                  /*french*/ "C'est l'heure de jouer au tennis."));
                                               // /*spanish*/Parece que es hora de una pachanga de tenis.

    hintTextTable[RHT_GANON_JOKE04] = HintText(CustomMessage("You'll never deflect my bolts of energy with your sword, then shoot me with those Light Arrows you happen to have.",
                                                  /*german*/ "Du wirst mich niemals besiegen, indem Du meine Blitze zurückschlägst und mich dann mit Licht-Pfeilen beschießt!",
                                                  /*french*/ "Ne perds pas ton temps à frapper mes éclairs d'énergie avec ton épée et me tirer avec tes flèches de Lumière!"));
                                               // /*spanish*/Nunca reflejarás mis esferas de energía con tu espada, para después dispararme con las flechas de luz que tendrás.

    hintTextTable[RHT_GANON_JOKE05] = HintText(CustomMessage("Why did I leave my trident back in the desert?",
                                                  /*german*/ "Verdammt... Warum habe ich meinen Dreizack in der Wüste gelassen?",
                                                  /*french*/ "Sale bêtise... Et j'ai oublié mon trident dans le désert!"));
                                               // /*spanish*/Santa Hylia... ¿Por qué me habré dejado el tridente en el desierto?

    hintTextTable[RHT_GANON_JOKE06] = HintText(CustomMessage("Zelda is probably going to do something stupid, like send you back to your own timeline.^So this is "
                                                             "quite meaningless. Do you really want to save this moron?",
                                                  /*german*/ "Zelda wird wahrscheinlich wieder etwas Dummes tun, wie Dich in Deine eigene Zeit zurück zu schicken."
												             "^Ziemlich sinnlos wenn Du mich fragst. Willst Du diesen Schwachkopf wirklich retten?",
                                                  /*french*/ "Même si je suis vaincu... Zelda te renverra dans ton ère, et je reviendrai conquérir!^Telle est la "
                                                             "prophécie d'Hyrule Historia!"));
                                               // /*spanish*/Seguro que Zelda trata de hacer alguna tontería, como enviarte de vuelta a tu línea temporal.^No tiene
                                                         // ningún sentido alguno. ¿De verdad quieres salvar a esa tonta?

    hintTextTable[RHT_GANON_JOKE07] = HintText(CustomMessage("What about Zelda makes you think&she'd be a better ruler than I?^I saved Lon Lon Ranch,&fed the "
                                                             "hungry,&and my castle floats.",
                                                  /*german*/ "Was an Zelda lässt Dich glauben, sie wäre eine bessere Herrscherin als ich?^"
                                                             "^Ich habe die Lon Lon-Farm gerettet, den Hungrigen etwas zu Essen gegeben, und mein Schloß schwebt!",
                                                  /*french*/ "Zelda ne sera jamais un meilleur monarque que moi!^J'ai un château volant, mes sujets sont des belles "
                                                             "amazones... et mes Moblins sont clairement plus puissants que jamais!"));
                                               // /*spanish*/¿Qué te hace pensar que Zelda gobierna mejor que yo?^Yo he salvado el Rancho Lon Lon,&he alimentado a
                                                          // los hambrientos&y hasta hago que mi castillo flote.

    hintTextTable[RHT_GANON_JOKE08] = HintText(CustomMessage("I've learned this spell,&it's really neat,&I'll keep it later&for your treat!",
                                                  /*german*/ "Zelda weint, das arme Kind, ihr Retter läuft herum wie blind!",
                                                  /*french*/ "Gamin, ton destin achève,&sous mon sort tu périras!&Cette partie ne fut pas brève,&et cette mort, tu subiras!"));
                                               // /*spanish*/Veamos ahora que harás,&la batalla ha de comenzar,&te enviaré de una vez al más allá,&¿listo para
                                                          // afrontar la verdad?

    hintTextTable[RHT_GANON_JOKE09] = HintText(CustomMessage("Many tricks are up my sleeve,&to save yourself&you'd better leave!",
                                                  /*german*/ "Da hilft kein Flehen und kein Flennen, bald wird dieses Weltlein brennen!",
                                                  /*french*/ "Sale petit garnement,&tu fais erreur!&C'est maintenant que marque&ta dernière heure!"));
                                               // /*spanish*/¿No osarás a mí enfrentarte?&Rimas aparte,&¡voy a matarte!

    hintTextTable[RHT_GANON_JOKE10] = HintText(CustomMessage("After what you did to Koholint Island, how can you call me the bad guy?",
                                                  /*german*/ "Nach allem was Du auf Kokolint angerichtet hast, wie kannst Du mich den Bösewicht nennen?",
                                                  /*french*/ "J'admire ce que tu as fait à l'Île Koholint... Toi et moi, nous devrions faire équipe!"));
                                               // /*spanish*/Después de lo que le hiciste a la Isla Koholint, ¿cómo te atreves a llamarme malvado?

    hintTextTable[RHT_GANON_JOKE11] = HintText(CustomMessage("Today, let's begin down&'The Hero is Defeated' timeline.",
                                                  /*german*/ "Heute beginnen wir die&'Der Held wurde besiegt'-Zeitlinie.",
                                                  /*french*/ "Si tu me vaincs, Hyrule sera englouti... mais si tu meurs, on aura A Link to the Past, le meilleur opus "
                                                             "de la série!"));
                                               // /*spanish*/Hoy daremos lugar a la línea temporal del Héroe Derrotado.&¡Prepárate para el culmen de esta saga!

   /*--------------------------
   |     Misc utilities       |
   ---------------------------*/

    hintTextTable[RHT_YOUR_POCKET] = HintText(CustomMessage("your pocket",
                                                 /*german*/ "in deiner Tasche",
                                                 /*french*/ "tes poches"));
                                              // /*spanish*/tu bolsillo

    hintTextTable[RHT_ISOLATED_PLACE] = HintText(CustomMessage("an Isolated Place",
                                                    /*german*/ "an einem abgelegenen Ort",
                                                    /*french*/ "un lieu isolé"));

    hintTextTable[RHT_DUNGEON_ORDINARY] = HintText(CustomMessage("&It's %gordinary%w.",
                                                      /*german*/ "&Man kann darauf die Worte&%gOcarina of Time%w entziffern...",
                                                      /*french*/ "&Elle vous semble %gordinaire%w."));

    hintTextTable[RHT_DUNGEON_MASTERFUL] = HintText(CustomMessage("&It's %rmasterful%w!",
                                                       /*german*/ "&Man kann darauf die Worte&%rMaster Quest%w entziffern...",
                                                       /*french*/ "&Étrange... les mots %r\"Master_Quest\"%w sont gravés dessus."));

    // clang-format on
}
} // namespace Rando
