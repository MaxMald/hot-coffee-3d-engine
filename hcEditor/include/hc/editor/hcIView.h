#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class IView
  {
  public:
    virtual ~IView() = default;

    virtual void draw() = 0;
  };
}
