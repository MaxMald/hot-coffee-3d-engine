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
    EditorViewsManager();
    ~EditorViewsManager();

    void initialize();
    bool processEvent(const Event& event);
    void draw();
    void shutdown();
    void registerView(IView* view);
    void clearViews();

  private:
    Vector<IView*> m_views;
  };
}
