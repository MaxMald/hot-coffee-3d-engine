#include "hc/graphics/resource/shaderProgram/hcBuiltInShaderProgramType.h"

namespace hc
{
  namespace builtInShaderProgramType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Unlit: return "Unlit";
      case Type::BlinnPhongForward: return "BlinnPhongForward";
      case Type::BlinnPhongDeferredGeometry: return "BlinnPhongDeferredGeometry";
      case Type::DeferredLighting: return "DeferredLighting";
      case Type::Skybox: return "Skybox";
      case Type::FinalPass: return "FinalPass";
      case Type::ShadowMap: return "ShadowMap";
      case Type::HairDeferredGeometry: return "HairDeferredGeometry";
      case Type::HairForwardSpecular: return "HairForwardSpecular";
      case Type::HairForwardTransparent: return "HairForwardTransparent";
      default:
        throw InvalidArgumentException(
          String::Format("Invalid shader program type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Unlit") return Type::Unlit;
      else if (str == "BlinnPhongForward") return Type::BlinnPhongForward;
      else if (str == "BlinnPhongDeferredGeometry") return Type::BlinnPhongDeferredGeometry;
      else if (str == "DeferredLighting") return Type::DeferredLighting;
      else if (str == "Skybox") return Type::Skybox;
      else if (str == "FinalPass") return Type::FinalPass;
      else if (str == "ShadowMap") return Type::ShadowMap;
      else if (str == "HairDeferredGeometry") return Type::HairDeferredGeometry;
      else if (str == "HairForwardSpecular") return Type::HairForwardSpecular;
      else if (str == "HairForwardTransparent") return Type::HairForwardTransparent;
      throw InvalidArgumentException(
        String::Format("Invalid shader program type string: %s", str.c_str())
      );
    }
  }
}
