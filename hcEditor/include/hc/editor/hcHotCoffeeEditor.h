#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogger.h"

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
    bool m_initialized;

    void onPrepare() override;
    void onShutdown() override;

    void initialize();
    void run();
    void initEngine();
    void prepareEditorScene();
  };
}
