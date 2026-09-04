#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  static constexpr UInt32 SAMPLER_2D_BINDING_POINT_DIRECTIONAL_SHADOW_MAP = 5;
  static constexpr UInt32 SAMPLER_2D_BINDING_POINT_SPOT_LIGHT_SHADOW_MAP = 6;

  namespace graphicsBackendType
  {
    /**
     * @brief Enumeration for different graphics backend types.
     */
    enum Type : UInt8
    {
      OPENGL = 0
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }

  namespace renderPipelineType
  {
    /**
     * @brief Enumeration for different render pipeline types.
     */
    enum Type : UInt8
    {
      DeferredHybrid = 0,
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }

  namespace renderPassType
  {
    /**
     * @brief Enumeration for different render pass types.
     */
    enum Type : UInt8
    {
      Forward = 0,
      ForwardTransparent,
      DeferredGeometry,
      DeferredLighting,
      HairForwardSpecular,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }

  namespace drawType
  {
    /**
     * @brief Enumeration for different primitive types. These types determine how
     * primitives are rendered in the graphics pipeline.
     */
    enum Type : UInt8
    {
      Triangles,
      Lines,
      LineStrip,
      LineLoop,
      Points
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }

  namespace polygonFillType
  {
    /**
     * @brief Enumeration for different polygon fill types. These types determine how
     * polygons are rendered in the graphics pipeline.
     */
    enum Type : UInt8
    {
      Undefined = 0,
      Solid,
      Wireframe,
      Point
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  }

  namespace shaderStageType
  {
    /**
     * @brief Enumeration for different shader stage types. These types represent
     * the stages in the graphics pipeline where shaders are executed.
     */
    enum Type : UInt8
    {
      Vertex = 0,
      Fragment = 1,
      Unknown = 255
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }

  namespace builtInShaderType
  {
    /**
     * @brief Enumeration for different built-in shader types. These types represent
     * pre-defined shaders used in the graphics pipeline.
     */
    enum HC_CORE_EXPORT Type : UInt8
    {
      UnlitVertex = 0,
      UnlitFragment,
      LitVertex,
      BlinnPhongForwardFragment,
      BlinnPhongDeferredFragment,
      FullScreenTriangleVertex,
      DeferredLightingFragment,
      SkyboxVertex,
      SkyboxFragment,
      FinalPassFragment,
      ShadowMapVertex,
      ShadowMapFragment,
      HairDeferredGeometryFragment,
      HairForwardSpecularFragment,
      HairForwardTransparentFragment,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
    HC_CORE_EXPORT shaderStageType::Type GetShaderStageType(Type type);
  }

  namespace builtInShaderProgramType
  {
    /**
     * @brief Enumeration for different built-in shader program types. These types
     * represent pre-defined shader programs used in the graphics pipeline.
     */
    enum HC_CORE_EXPORT Type : UInt8
    {
      Unlit = 0,
      BlinnPhongForward,
      BlinnPhongDeferredGeometry,
      DeferredLighting,
      Skybox,
      FinalPass,
      ShadowMap,
      HairDeferredGeometry,
      HairForwardSpecular,
      HairForwardTransparent,
      Count
    };

    HC_CORE_EXPORT String ToString(Type type);
    HC_CORE_EXPORT Type FromString(const String& str);
  };

  namespace shadingType
  {
    /**
     * @brief Enumeration for different shading types. These types represent the
     * shading models used in the graphics pipeline.
     */
    enum Type : UInt8
    {
      Unlit = 0,
      BlinnPhong = 1,
      Hair = 2,
      Unknown = 255
    };

    HC_CORE_EXPORT String toString(Type type);
    HC_CORE_EXPORT Type fromString(const String& str);
  }

  namespace materialRenderMode
  {
    /**
     * @brief Enumeration of material render modes.
     *
     * Defines the rendering mode for materials, which can affect how they are
     * rendered in the graphics pipeline.
     */
    enum class Type : UInt8
    {
      Background = 0,
      Opaque,
      AlphaCutout,
      Transparent
    };

    String HC_CORE_EXPORT toString(Type renderMode);
    Type HC_CORE_EXPORT fromString(const String& str);
  }

  namespace dataBlockType
  {
    /**
     * @brief Enumeration of data block types used in the rendering engine.
     *
     * This enum defines the various types of data blocks that can be used for rendering.
     */
    enum Type : UInt8
    {
      Camera = 0,
      Lights = 1,
      LightShadows = 2,
      Object = 3,
      LightViewProjection = 4,
      MaterialUnlit = 5,
      MaterialBlinnPhong = 6,
      MaterialHair = 7,
      Count
    };
  }

  namespace textureType
  {
    /**
     * @brief Enumeration of texture types used in the rendering engine.
     *
     * This enum defines the various types of textures that can be used for materials and
     * rendering.
     */
    enum Type : UInt8
    {
      None = 0,
      Diffuse = 1,
      Specular = 2,
      Ambient = 3,
      Emissive = 4,
      Height = 5,
      Normals = 6,
      Shininess = 7,
      Opacity = 8,
      Displacement = 9,
      Lightmap = 10,
      Reflection = 11,

      // PBR
      BaseColor = 12,
      NormalCamera = 13,
      EmissionColor = 14,
      Metalness = 15,
      DiffuseRoughness = 16,
      AmbientOcclusion = 17,

      Sheen = 19,
      Clearcoat = 20,
      Transmission = 21,

      // Maya material declarations

      MayaBase = 22,
      MayaSpecular = 23,
      MayaSpecularColor = 24,
      MayaSpecularRoughness = 25,

      Anisotropy = 26,
      GltfMetallicRoughness = 27,

      Unknown = 255
    };
  }
}
