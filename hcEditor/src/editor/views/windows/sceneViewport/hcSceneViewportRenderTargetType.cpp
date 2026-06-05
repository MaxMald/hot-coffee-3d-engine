#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderTargetType.h"

namespace hc::editor
{
  namespace sceneViewportRenderTargetType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case FinalColor:
        return "Final Color";
      case GBufferPositionAndDepth: return
        "GBuffer Position + Depth";
      case GBufferNormalRoughness: return
        "GBuffer Normal + Roughness";
      case GBufferAlbedoAlpha: return
        "GBuffer Albedo + Alpha";
      case GBufferMaterialParameters: return
        "GBuffer Material Parameters";
      default:
        throw RuntimeErrorException(
          String::Format("Not Implemented render target type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Final Color")
        return FinalColor;
      if (str == "GBuffer Position + Depth")
        return GBufferPositionAndDepth;
      if (str == "GBuffer Normal + Roughness")
        return GBufferNormalRoughness;
      if (str == "GBuffer Albedo + Alpha")
        return GBufferAlbedoAlpha;
      if (str == "GBuffer Material Parameters")
        return GBufferMaterialParameters;
      throw InvalidArgumentException(
        String::Format("Unknown render target type string: %s", str.c_str())
      );
    }
  }
}
