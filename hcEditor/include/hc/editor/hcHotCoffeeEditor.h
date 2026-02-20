#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogHistory.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcEditorServiceManager.h"

namespace hc::editor
{
  class HotCoffeeEditor
  {
  public:
    HotCoffeeEditor();
    ~HotCoffeeEditor() = default;

    ProcessResult initialize();
    void run();
    void destroy();

  private:
    HotCoffeeEngine m_engine;
    EditorServiceManager m_serviceManager;
    EditorLogHistory m_editorLogHistory;
    EditorViewsManager m_viewsManager;
    bool m_initialized;

    void prepareEditorScene();
  };
}
