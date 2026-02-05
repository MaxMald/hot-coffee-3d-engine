#pragma once

namespace hc::editor
{
  class EditorViewsManager;
  class GameObjectSelectionService;
  class ProjectManager;

  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      EditorViewsManager& manager,
      GameObjectSelectionService& gameObjectSelectionService,
      ProjectManager& projectManager
    );
  }
}
