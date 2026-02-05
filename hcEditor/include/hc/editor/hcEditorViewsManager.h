#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class Event;
}

namespace hc::editor
{
  class IView;

  class EditorViewsManager : public AModule<EditorViewsManager>
  {
  public:
    EditorViewsManager() = default;
    ~EditorViewsManager() override = default;

    void initialize();
    bool processEvent(const Event& event);
    void draw();
    void registerView(IView* view);
    void unregisterView(IView* view);

  private:
    Vector<IView*> m_views;

    void onPrepare() override;
    void onShutdown() override;

    void destroy();
  };
}
