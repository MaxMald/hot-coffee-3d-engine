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
      case  GBufferSpecularColorAndShininess: return
        "GBuffer Specular Color + Shininess";
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
      else if (str == "GBuffer Position + Depth")
        return GBufferPositionAndDepth;
      else if (str == "GBuffer Normal + Roughness")
        return GBufferNormalRoughness;
      else if (str == "GBuffer Albedo + Alpha")
        return GBufferAlbedoAlpha;
      else if (str == "GBuffer Material Parameters")
        return GBufferMaterialParameters;
      else if (str == "GBuffer Specular Color + Shininess")
        return GBufferSpecularColorAndShininess;
      throw InvalidArgumentException(
        String::Format("Unknown render target type string: %s", str.c_str())
      );
    }
  }
}
