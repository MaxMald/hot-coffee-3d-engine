#pragma once

#include "hc/editor/hcABaseView.h"

namespace hc::editor
{
  class EditorViewsManager;
  class ProjectManager;

  /**
   * @brief Main menu bar view for the Hot Coffee Editor.
   *
   * Implements the main menu bar using ImGui, providing access to
   * various editor functionalities.
   */
  class  MainMenuToolbar : public ABaseView
  {
  public:
    MainMenuToolbar(
      EditorViewsManager& editorViewsManager,
      ProjectManager& projectManager
    );
    virtual ~MainMenuToolbar();

    void draw() override;

  protected:
    EditorViewsManager& m_editorViewsManager;
    ProjectManager& m_projectManager;
    void displayOpenProjectDialog();
  };
}
