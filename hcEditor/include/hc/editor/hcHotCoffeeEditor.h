#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogHistory.h"

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
    EditorLogHistory m_editorLogHistory;
    bool m_initialized;

    void prepareEditorScene();
  };
}
