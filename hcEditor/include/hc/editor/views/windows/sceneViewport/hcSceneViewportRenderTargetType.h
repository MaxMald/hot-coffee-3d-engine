#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  namespace sceneViewportRenderTargetType
  {
    enum Type : UInt8
    {
      FinalColor = 0,
      GBufferPositionAndDepth,
      GBufferNormal,
      GBufferAlbedoAlpha,
      GBufferORMIOR,
      Count
    };

    String ToString(Type type);
    Type FromString(const String& str);
  }
}
