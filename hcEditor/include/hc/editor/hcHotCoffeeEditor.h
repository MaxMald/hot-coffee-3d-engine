#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogger.h"
#include "hc/editor/hcEditorViewsManager.h"

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
    EditorViewsManager m_viewsManager;
    bool m_initialized;

    void onPrepare() override;
    void onShutdown() override;

    void initialize();
    void run();
    void initEngine();
    void prepareEditorScene();
  };
}
