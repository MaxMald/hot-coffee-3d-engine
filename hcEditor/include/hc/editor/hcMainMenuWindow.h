#pragma once

#include "hc/editor/hcIView.h"
#include "hc/editor/hcPluginManagerWindow.h"

namespace hc
{
  class HotCoffeeEngine;
}

namespace hc::editor
{
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
    MainMenuWindow(HotCoffeeEngine& engine);

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
  };
}
