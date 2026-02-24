#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/services/hcEditorServiceManager.h"
#include "hc/editor/views/hcEditorViewsManager.h"

namespace hc::editor
{
  class EditorLogHistory;

  class EditorScene : public Scene
  {
  public:
    EditorScene(HotCoffeeEngine& engine, EditorLogHistory& editorLogHistory);
    virtual ~EditorScene();

  private:
    HotCoffeeEngine& m_engine;
    EditorLogHistory& m_editorLogHistory;
    EditorServiceManager m_serviceManager;
    EditorViewsManager m_viewsManager;

    void onInitialized() override;
    void onAfterDraw(const RenderContext& renderContext) override;
    void onBeforeUpdate(const Time& elapsedTime) override;
    void onDestroy() override;
  };
}
