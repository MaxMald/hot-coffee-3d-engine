#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class IWindow;
  class Event;
}

namespace hc::editor
{
  namespace hcImguiHandler
  {
    /**
     * @brief Initializes ImGui with the specified window.
     * 
     * @param window Reference to the window to attach ImGui to.
     */
    void init(IWindow& window);

    /**
     * @brief Processes an input event for ImGui.
     * 
     * @param event The event to process.
     * 
     * @return true if the event was handled by ImGui, false otherwise.
     */
    bool processEvent(const Event& event);

    /**
     * @brief Begins a new ImGui frame.
     */
    void beginFrame();

    /**
     * @brief Ends the current ImGui frame and renders the UI.
     */
    void endFrame();

    /**
     * @brief Cleans up ImGui resources.
     */
    void destroy();
  }
}
