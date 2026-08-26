#include "hc/graphics/resource/shader/hcBuiltInShaderType.h"

namespace hc
{
  namespace builtInShaderType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::UnlitVertex: return "UnlitVertex";
      case Type::UnlitFragment: return "UnlitFragment";
      case Type::LitVertex: return "LitVertex";
      case Type::BlinnPhongForwardFragment: return "BlinnPhongForwardFragment";
      case Type::BlinnPhongDeferredFragment: return "BlinnPhongDeferredFragment";
      case Type::FullScreenTriangleVertex: return "FullScreenTriangleVertex";
      case Type::DeferredLightingFragment: return "DeferredLightingFragment";
      case Type::SkyboxVertex: return "SkyboxVertex";
      case Type::SkyboxFragment: return "SkyboxFragment";
      case Type::FinalPassFragment: return "FinalPassFragment";
      case Type::ShadowMapVertex: return "ShadowMapVertex";
      case Type::ShadowMapFragment: return "ShadowMapFragment";
      case Type::HairDeferredGeometryFragment: return "HairDeferredGeometryFragment";
      case Type::HairForwardSpecularFragment: return "HairForwardSpecularFragment";
      case Type::HairForwardTransparentFragment: return "HairForwardTransparentFragment";

      default:
        throw InvalidArgumentException(
          String::Format("builtInShaderType::ToString : Invalid built-in shader type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "UnlitVertex") return Type::UnlitVertex;
      else if (str == "UnlitFragment") return Type::UnlitFragment;
      else if (str == "LitVertex") return Type::LitVertex;
      else if (str == "BlinnPhongForwardFragment") return Type::BlinnPhongForwardFragment;
      else if (str == "BlinnPhongDeferredFragment") return Type::BlinnPhongDeferredFragment;
      else if (str == "FullScreenTriangleVertex") return Type::FullScreenTriangleVertex;
      else if (str == "DeferredLightingFragment") return Type::DeferredLightingFragment;
      else if (str == "SkyboxVertex") return Type::SkyboxVertex;
      else if (str == "SkyboxFragment") return Type::SkyboxFragment;
      else if (str == "FinalPassFragment") return Type::FinalPassFragment;
      else if (str == "ShadowMapVertex") return Type::ShadowMapVertex;
      else if (str == "ShadowMapFragment") return Type::ShadowMapFragment;
      else if (str == "HairDeferredGeometryFragment") return Type::HairDeferredGeometryFragment;
      else if (str == "HairForwardSpecularFragment") return Type::HairForwardSpecularFragment;
      else if (str == "HairForwardTransparentFragment") return Type::HairForwardTransparentFragment;

      throw InvalidArgumentException(
        String::Format("builtInShaderType::FromString : Invalid built-in shader type string: %s", str.c_str())
      );
    }

    shaderStageType::Type GetShaderStageType(Type type)
    {
      switch (type)
      {
      case Type::UnlitVertex:
      case Type::LitVertex:
      case Type::FullScreenTriangleVertex:
      case Type::SkyboxVertex:
      case Type::ShadowMapVertex:
        return shaderStageType::Vertex;

      case Type::UnlitFragment:
      case Type::BlinnPhongForwardFragment:
      case Type::BlinnPhongDeferredFragment:
      case Type::DeferredLightingFragment:
      case Type::SkyboxFragment:
      case Type::FinalPassFragment:
      case Type::ShadowMapFragment:
      case Type::HairDeferredGeometryFragment:
      case Type::HairForwardSpecularFragment:
      case Type::HairForwardTransparentFragment:
        return shaderStageType::Fragment;

      default:
        throw InvalidArgumentException(
          String::Format("builtInShaderType::GetShaderStageType : Invalid built-in shader type: %d", static_cast<Int32>(type))
        );
      }
    }
  }
}
