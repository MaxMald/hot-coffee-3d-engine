#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class EditorViewsManager;
  class GameObjectSelectionService;
  class ProjectManager;
  class EditorLogHistory;

  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      HotCoffeeEngine& hotCoffeeEngine,
      EditorViewsManager& manager,
      GameObjectSelectionService& gameObjectSelectionService,
      ProjectManager& projectManager,
      EditorLogHistory& editorLogHistory
    );
  }
}
