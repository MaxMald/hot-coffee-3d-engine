#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class LightManagerWindow : public AWindowView
  {
  public:
    LightManagerWindow();
    ~LightManagerWindow();

  protected:
    void onDraw() override;
    void drawLightController(Light* light);
  };
}
