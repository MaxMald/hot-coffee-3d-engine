#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class EditorServiceManager;
  class EditorViewsManager;
  class EditorLogHistory;

  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      HotCoffeeEngine& hotCoffeeEngine,
      EditorViewsManager& viewsManager,
      EditorServiceManager& serviceManager,
      EditorLogHistory& editorLogHistory
    );
  }
}
