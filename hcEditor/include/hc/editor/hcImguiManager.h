#pragma once

#include "hc/hcNonCopyable.h"
#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class IWindow;
  class Event;
}

namespace hc::editor
{
  class ImguiManager : public NonCopyable
  {
  public:
    ImguiManager();
    ~ImguiManager();

    void init(IWindow& window);
    bool processEvent(const Event&);
    void beginFrame();
    void endFrame();
    void destroy();
  };
}
