#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcIView.h"

namespace hc
{
  class HotCoffeeEngine;
}

namespace hc::editor
{
  class EditorLoggerWindow;
  class PluginManagerWindow;
  class SceneGraphWindow;

  /**
   * @brief Main menu bar view for the Hot Coffee Editor.
   *
   * Implements the main menu bar using ImGui, providing access to
   * various editor functionalities.
   */
  class  MainMenuWindow :
    public IView,
    public IDependencyResolvable
  {
  public:
    MainMenuWindow();
    virtual ~MainMenuWindow();

    /**
     * @brief Draws the main menu bar contents.
     *
     * Overrides the base class method to implement the main menu UI.
     */
    void draw() override;

    /**
     * @brief Resolves dependencies for the MainMenuWindow.
     *
     * @param container Reference to the dependency container.
     */
    void resolveDependencies(DependencyContainer& container) override;

  protected:
    SharedPtr<PluginManagerWindow> m_pluginManagerWindow;
    SharedPtr<EditorLoggerWindow> m_editorLoggerWindow;
    SharedPtr<SceneGraphWindow> m_sceneGraphWindow;
  };
}
