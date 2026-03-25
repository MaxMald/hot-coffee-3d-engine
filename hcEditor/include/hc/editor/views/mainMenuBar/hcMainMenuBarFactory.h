#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class MainMenuBar;
  class EditorViewsManager;
  class EditorServiceManager;

  namespace mainMenuBarFactory
  {
    UniquePtr<MainMenuBar> create(
      EditorViewsManager& editorViewsManager,
      EditorServiceManager& editorServiceManager
    );
  }
}
