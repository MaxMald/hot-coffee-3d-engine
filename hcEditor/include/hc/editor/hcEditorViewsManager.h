#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class Event;
}

namespace hc::editor
{
  class IView;

  class EditorViewsManager
  {
  public:
    static EditorViewsManager& Instance();
    static void Prepare();
    static void Shutdown();

    void initialize();
    bool processEvent(const Event& event);
    void draw();
    void registerView(IView* view);
    void unregisterView(IView* view);
    void clearViews();

  private:
    static EditorViewsManager* s_instance;

    Vector<IView*> m_views;

    EditorViewsManager();
    ~EditorViewsManager();

    void destroy();
  };
}
