#ifndef SOH_GFX_DEBUGGER_H
#define SOH_GFX_DEBUGGER_H

#include "window/gui/GuiWindow.h"
#include "window/gui/GfxDebuggerWindow.h"

class SohGfxDebuggerWindow : public LUS::GfxDebuggerWindow {
  public:
    using GfxDebuggerWindow::GfxDebuggerWindow;

  protected:
    void InitElement() override;
    void UpdateElement() override;
    void DrawElement() override;
};

#endif // SOH_GFX_DEBUGGER_H
