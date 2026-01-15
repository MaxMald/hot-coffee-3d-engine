#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

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

    HotCoffeeEditor();
    ~HotCoffeeEditor();

    void onPrepare();
    void onShutdown();
  };
}
