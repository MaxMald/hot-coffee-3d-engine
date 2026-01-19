#pragma once

#include <hc/hcDependencyContainer.h>

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLogger.h"

namespace hc::editor
{
  class EditorViewsManager;

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
    DependencyContainer m_dependencyContainer;
    SharedPtr<EditorLogger> m_editorLogger;
    SharedPtr<EditorViewsManager> m_editorViewsManager;

    HotCoffeeEditor();
    ~HotCoffeeEditor();

    void onPrepare();
    void onShutdown();

    void prepareEditorLogger();
    void registerDependencies();
    void resolveDependencies();
    void initEngine();
    void prepareEditorScene();
    void runMainLoop();
    void unsubscribeEditorLogger();
  };
}
