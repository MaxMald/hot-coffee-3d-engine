#pragma once

#include "hc/editor/hcABaseView.h"

namespace hc::editor
{
  /**
   * @brief Main menu bar view for the Hot Coffee Editor.
   *
   * Implements the main menu bar using ImGui, providing access to
   * various editor functionalities.
   */
  class  MainMenuToolbar : public ABaseView
  {
  public:
    MainMenuToolbar();
    virtual ~MainMenuToolbar();

    void draw() override;

  protected:
    void displayOpenProjectDialog();
  };
}
