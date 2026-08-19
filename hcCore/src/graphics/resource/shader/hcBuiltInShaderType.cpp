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

      default:
        throw InvalidArgumentException(
          String::Format("builtInShaderType::ToString : Invalid built-in shader type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "UnlitVertex") return Type::UnlitVertex;
      if (str == "UnlitFragment") return Type::UnlitFragment;
      if (str == "LitVertex") return Type::LitVertex;
      if (str == "BlinnPhongForwardFragment") return Type::BlinnPhongForwardFragment;
      if (str == "BlinnPhongDeferredFragment") return Type::BlinnPhongDeferredFragment;
      if (str == "FullScreenTriangleVertex") return Type::FullScreenTriangleVertex;
      if (str == "DeferredLightingFragment") return Type::DeferredLightingFragment;
      if (str == "SkyboxVertex") return Type::SkyboxVertex;
      if (str == "SkyboxFragment") return Type::SkyboxFragment;
      if (str == "FinalPassFragment") return Type::FinalPassFragment;
      if (str == "ShadowMapVertex") return Type::ShadowMapVertex;
      if (str == "ShadowMapFragment") return Type::ShadowMapFragment;

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
        return shaderStageType::Fragment;

      default:
        throw InvalidArgumentException(
          String::Format("builtInShaderType::GetShaderStageType : Invalid built-in shader type: %d", static_cast<Int32>(type))
        );
      }
    }
  }
}
