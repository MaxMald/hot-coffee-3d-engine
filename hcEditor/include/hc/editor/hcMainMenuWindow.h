#pragma once

#include "hc/editor/hcIView.h"
#include "hc/editor/hcPluginManagerWindow.h"
#include "hc/editor/hcEditorLoggerWindow.h"
#include "hc/editor/hcSceneGraphWindow.h"

namespace hc
{
  class HotCoffeeEngine;
}

namespace hc::editor
{
  class EditorLogger;

  /**
   * @brief Main menu bar view for the Hot Coffee Editor.
   *
   * Implements the main menu bar using ImGui, providing access to
   * various editor functionalities.
   */
  class  MainMenuWindow : public IView
  {
  public:
    /**
     * @brief Constructs the MainMenu view.
     *
     * @param engine Reference to the HotCoffeeEngine instance.
     */
    MainMenuWindow(HotCoffeeEngine& engine, EditorLogger& editorLogger);

    /**
     * @brief Virtual destructor.
     */
    virtual ~MainMenuWindow();

    /**
     * @brief Draws the main menu bar contents.
     *
     * Overrides the base class method to implement the main menu UI.
     */
    virtual void draw() override;

  protected:
    PluginManagerWindow m_pluginManagerWindow;
    EditorLoggerWindow m_editorLoggerWindow;
    SceneGraphWindow m_sceneGraphWindow;
  };
}
