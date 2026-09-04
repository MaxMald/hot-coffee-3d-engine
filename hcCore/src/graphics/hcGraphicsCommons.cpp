#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  namespace graphicsBackendType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case OPENGL:
        return "OpenGL";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented graphics backend type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "OpenGL")
        return OPENGL;
      else
        throw InvalidArgumentException("Invalid graphics backend type string: " + str);
    }
  }

  namespace renderPipelineType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::DeferredHybrid:
        return "DeferredHybrid";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented render pipeline type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "DeferredHybrid")
        return Type::DeferredHybrid;
      else
        throw InvalidArgumentException(
          String::Format("Invalid render pipeline type string: %s", str.c_str())
        );
    }
  }

  namespace renderPassType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Forward:
        return "Forward";
      case Type::DeferredGeometry:
        return "DeferredGeometry";
      case Type::DeferredLighting:
        return "DeferredLighting";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented render pass type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Forward")
        return Type::Forward;
      else if (str == "DeferredGeometry")
        return Type::DeferredGeometry;
      else if (str == "DeferredLighting")
        return Type::DeferredLighting;
      else
        throw InvalidArgumentException(
          String::Format("Invalid render pass type string: %s", str.c_str())
        );
    }
  }

  namespace drawType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Triangles:
        return "Triangles";
      case Type::Lines:
        return "Lines";
      case Type::LineStrip:
        return "LineStrip";
      case Type::LineLoop:
        return "LineLoop";
      case Type::Points:
        return "Points";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented draw type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Triangles")
        return Type::Triangles;
      else if (str == "Lines")
        return Type::Lines;
      else if (str == "LineStrip")
        return Type::LineStrip;
      else if (str == "LineLoop")
        return Type::LineLoop;
      else if (str == "Points")
        return Type::Points;
      else
        throw InvalidArgumentException("Invalid draw type: " + str);
    }
  }

  namespace polygonFillType
  {
    String ToString(Type type)
    {
      switch (type)
      {
      case Type::Undefined:
        return "Undefined";
      case Type::Solid:
        return "Solid";
      case Type::Wireframe:
        return "Wireframe";
      case Type::Point:
        return "Point";
      default:
        throw RuntimeErrorException(
          String::Format("Not implemented polygon fill type: %d", static_cast<Int32>(type))
        );
      }
    }

    Type FromString(const String& str)
    {
      if (str == "Undefined")
        return Type::Undefined;
      else if (str == "Solid")
        return Type::Solid;
      else if (str == "Wireframe")
        return Type::Wireframe;
      else if (str == "Point")
        return Type::Point;
      else
        throw InvalidArgumentException("Invalid polygon fill type string: " + str);
    }
  }

  namespace shaderStageType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Vertex:
        return "Vertex";
      case Fragment:
        return "Fragment";
      default:
        return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Vertex")
        return Vertex;
      else if (str == "Fragment")
        return Fragment;
      else
        return Unknown;
    }
  }

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

  namespace shadingType
  {
    String toString(Type type)
    {
      switch (type)
      {
      case Unlit:
        return "Unlit";
      case BlinnPhong:
        return "BlinnPhong";
      case Hair:
        return "Hair";
      default:
        throw RuntimeErrorException("Unknown shading type.");
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Unlit")
        return Unlit;
      else if (str == "BlinnPhong")
        return BlinnPhong;
      else if (str == "Hair")
        return Hair;
      else
        throw RuntimeErrorException("Unknown shading type string: " + str);
    }
  }

  namespace materialRenderMode
  {
    String toString(Type renderMode)
    {
      switch (renderMode)
      {
      case Type::Background:
        return "Background";
      case Type::Opaque:
        return "Opaque";
      case Type::AlphaCutout:
        return "AlphaCutout";
      case Type::Transparent:
        return "Transparent";
      default:
        return "Unknown";
      }
    }

    Type fromString(const String& str)
    {
      if (str == "Background")
        return Type::Background;
      else if (str == "Opaque")
        return Type::Opaque;
      else if (str == "AlphaCutout")
        return Type::AlphaCutout;
      else if (str == "Transparent")
        return Type::Transparent;
      else
        throw RuntimeErrorException(
          String::Format("Invalid material render mode string: '%s'", str.c_str())
        );
    }
  }
}
