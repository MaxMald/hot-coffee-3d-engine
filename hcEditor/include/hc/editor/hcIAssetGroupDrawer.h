#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class IAssetGroupDrawer : public NonCopyable
  {
  public:
    virtual ~IAssetGroupDrawer() = default;

    virtual void draw() = 0;
  };
}
