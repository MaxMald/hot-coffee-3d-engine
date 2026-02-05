#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogHistory.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcProjectManager.h"

namespace hc::editor
{
  class EditorViewsManager;

  class HotCoffeeEditor : public AModule<HotCoffeeEditor>
  {
  public:
    static void Initialize();
    static void Run();

    HotCoffeeEditor();
    ~HotCoffeeEditor() override = default;

  private:
    EditorLogHistory m_editorLogHistory;
    EditorViewsManager m_viewsManager;
    GameObjectSelectionService m_gameObjectSelectionService;
    ProjectManager m_projectManager;
    bool m_initialized;

    void onPrepare() override;
    void onShutdown() override;

    void initialize();
    void run();
    void initEngine();
    void prepareEditorScene();
  };
}
