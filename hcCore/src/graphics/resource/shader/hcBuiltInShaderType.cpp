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
      default:
        throw InvalidArgumentException(
          String::Format("Invalid built-in shader type: %d", static_cast<Int32>(type))
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
      throw InvalidArgumentException(
        String::Format("Invalid built-in shader type string: %s", str.c_str())
      );
    }
  }
}
