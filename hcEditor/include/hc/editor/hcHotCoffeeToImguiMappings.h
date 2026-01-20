#pragma once

#include <hc/hcMouseButtonKey.h>
#include <hc/hcKeyboardKey.h>
#include <hc/hcVector3.h>

#include "hc/editor/hcEditorPrerequisites.h"
#include "imgui.h"

namespace hc::editor
{
  namespace hotCoffeeToImgui
  {
    /**
     * @brief Maps a HotCoffee mouse button code to an ImGui mouse button code.
     *
     * @param button The HotCoffee mouse button code.
     * 
     * @return The corresponding ImGui mouse button code.
     */
    ImGuiMouseButton_ mapMouseButton(const mouseButtonKey::Type& button);

    /**
     * @brief Maps a HotCoffee keyboard key code to an ImGui key code.
     *
     * @param key The HotCoffee keyboard key code.
     * 
     * @return The corresponding ImGui key code.
     */
    ImGuiKey mapKeyboardCode(const keyboardKey::Type& key);

    
  }
}
