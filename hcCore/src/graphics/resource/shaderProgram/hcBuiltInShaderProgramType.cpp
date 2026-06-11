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
      default:
        throw InvalidArgumentException(
          String::Format("Invalid shader program type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Unlit") return Type::Unlit;
      if (str == "BlinnPhongForward") return Type::BlinnPhongForward;
      if (str == "BlinnPhongDeferredGeometry") return Type::BlinnPhongDeferredGeometry;
      if (str == "DeferredLighting") return Type::DeferredLighting;
      if (str == "Skybox") return Type::Skybox;
      throw InvalidArgumentException(
        String::Format("Invalid shader program type string: %s", str.c_str())
      );
    }
  }
}
