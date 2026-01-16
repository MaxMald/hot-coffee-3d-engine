#pragma once

#include "hc/hcNonCopyable.h"
#include "hc/hcIDrawable.h"
#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class EditorInterfaceManager :
    public NonCopyable,
    public IDrawable
  {
  public:
    EditorInterfaceManager();
    ~EditorInterfaceManager() override;

    void draw(IGraphicsManager& graphicsManager) override;

  private:
  };
}
