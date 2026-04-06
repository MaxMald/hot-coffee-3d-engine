#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistory.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/views/hcEditorViewsManager.h"

namespace hc::editor
{
  class HotCoffeeEditor : public IGameLoopListener
  {
  public:
    HotCoffeeEditor();
    ~HotCoffeeEditor() = default;

    ProcessResult initialize();
    void run();
    void destroy();

  protected:
    bool onEvent(const Event& event) override;
    void onBeforeSceneUpdate(const Time& elapsedTime) override;
    void onBeforeSceneRender() override;
    void onAfterSceneRender() override;

  private:
    HotCoffeeEngine m_engine;
    EditorLogHistory m_editorLogHistory;
    EditorServiceManager m_serviceManager;
    EditorViewsManager m_viewsManager;
    bool m_initialized;

    void prepareEditorScene();
    void prepareEditorServices();
    void prepareEditorViews();
  };
}
