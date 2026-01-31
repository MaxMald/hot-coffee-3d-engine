#pragma once

#include "hc/editor/hcIView.h"

namespace hc::editor
{
  class EditorLoggerWindow;
  class PluginManagerWindow;
  class SceneGraphWindow;
  class ProjectManager;
  class LightManagerWindow;
  class CameraManagerWindow;
  class AssetManagerWindow;

  /**
   * @brief Main menu bar view for the Hot Coffee Editor.
   *
   * Implements the main menu bar using ImGui, providing access to
   * various editor functionalities.
   */
  class  MainMenuToolbar :
    public IView,
    public IDependencyResolvable
  {
  public:
    MainMenuToolbar();
    virtual ~MainMenuToolbar();

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
    SharedPtr<ProjectManager> m_projectManager;
    SharedPtr<LightManagerWindow> m_lightManagerWindow;
    SharedPtr<CameraManagerWindow> m_cameraManagerWindow;
    SharedPtr<AssetManagerWindow> m_assetManagerWindow;

    void displayOpenProjectDialog();
  };
}
