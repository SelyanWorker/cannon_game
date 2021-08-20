#pragma once

#include "WindowsWindow.h"
#include "core/input.h"

namespace selyan
{
    class WindowsInput : public Input
    {
    protected:
        bool isKeyPressedImpl(uint32_t KeyCode) override;
        bool isKeyReleaseImpl(uint32_t KeyCode) override;
        bool isMouseKeyPressedImpl(uint32_t mouseKeyCode) override;
        bool isMouseKeyReleasedImpl(uint32_t mouseKeyCode) override;
        std::pair<float, float> getCursorPositionImpl() override;
        // std::pair<float, float> GetMouseScrollImpl()  override;
    };

}   // namespace selyan
