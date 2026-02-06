#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class MainMenuBar;
  class EditorViewsManager;
  class ProjectManager;

  namespace mainMenuBarFactory
  {
    UniquePtr<MainMenuBar> create(
      EditorViewsManager& editorViewsManager,
      ProjectManager& projectManager
    );
  }
}
