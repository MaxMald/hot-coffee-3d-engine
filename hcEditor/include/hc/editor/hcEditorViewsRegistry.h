#pragma once

namespace hc::editor
{
  class EditorViewsManager;
  class GameObjectSelectionService;

  namespace editorViewsRegistry
  {
    void registerDefaultViews(
      EditorViewsManager& manager,
      GameObjectSelectionService& gameObjectSelectionService
    );
  }
}
