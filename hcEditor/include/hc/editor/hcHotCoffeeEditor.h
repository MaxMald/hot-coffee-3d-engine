#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogHistory.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcEditorServiceManager.h"

namespace hc::editor
{
  class HotCoffeeEditor : public AModule<HotCoffeeEditor>
  {
  public:
    static void Initialize();
    static void Run();

    HotCoffeeEditor();
    ~HotCoffeeEditor() override = default;

  private:
    EditorServiceManager m_serviceManager;
    EditorLogHistory m_editorLogHistory;
    EditorViewsManager m_viewsManager;
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
