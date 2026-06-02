#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class GraphicsWindow : public AWindowView
  {
  public:
    GraphicsWindow(IGraphicsManager& graphicsManager);
    ~GraphicsWindow();

    void destroy() override;

  private:
    IGraphicsManager& m_graphicsManager;

    void onDraw() override;
  };
}
