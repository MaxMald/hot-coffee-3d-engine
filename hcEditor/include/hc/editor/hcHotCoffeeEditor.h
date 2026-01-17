#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogger.h"

namespace hc::editor
{
  class HotCoffeeEditor
  {
  public:
    static HotCoffeeEditor& Instance();
    static void Prepare();
    static void Shutdown();

    void start();

  private:
    static HotCoffeeEditor* _Instance;

    bool m_started;
    EditorLogger m_editorLogger;

    HotCoffeeEditor();
    ~HotCoffeeEditor();

    void onPrepare();
    void onShutdown();
  };
}
