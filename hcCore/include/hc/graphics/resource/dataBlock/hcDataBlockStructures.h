#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  namespace dataBlockStructure
  {
    /**
   * @brief GPU-aligned data for a single camera.
   */
    struct alignas(16) HC_CORE_EXPORT Camera
    {
      Matrix4 projectionMatrix = Matrix4::Identity(); ///< Projection matrix transforming camera space to clip space.
      Matrix4 viewMatrix = Matrix4::Identity();       ///< View matrix transforming world space to camera space.

      Vector3f cameraWorldPosition;                   ///< World-space position of the camera.
      float padding0 = 0.0f;
    };

    static_assert(sizeof(Camera) % 16 == 0, "Camera must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single directional light.
     */
    struct alignas(16) HC_CORE_EXPORT DirectionalLight
    {
      Vector4f directionAndIntensity;   ///< Normalized light direction. W component stores intensity.
      Color color;                      ///< Linear RGB emission color.

      Int32 shadowFrameDataIndex = -1;  ///< Index of the shadow map data for this light, or -1 if shadows are disabled.
      Int32 padding0 = 0;
      Int32 padding1 = 0;
      Int32 padding2 = 0;
    };

    static_assert(sizeof(DirectionalLight) % 16 == 0, "DirectionalLight must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single omni light.
     */
    struct alignas(16) HC_CORE_EXPORT OmniLight
    {
      Vector4f position;      ///< World-space position (w unused).
      Color color;            ///< Linear RGB emission color.

      float range = 0.0f;     ///< Maximum influence radius in world units.
      float intensity = 0.0f; ///< Scalar multiplier applied to color.
      float padding0 = 0.0f;
      float padding1 = 0.0f;
    };

    static_assert(sizeof(OmniLight) % 16 == 0, "OmniLight must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single spot light.
     */
    struct alignas(16) HC_CORE_EXPORT SpotLight
    {
      Vector4f position;                ///< World-space position (w unused).
      Vector4f direction;               ///< Normalized cone direction (w unused).
      Color color;                      ///< Linear RGB emission color.

      float range = 0.0f;               ///< Maximum influence radius in world units.
      float innerConeAngle = 0.0f;      ///< Cosine of the inner cone half-angle (radians).
      float intensity = 0.0f;           ///< Scalar multiplier applied to color.
      float outerConeAngle = 0.0f;      ///< Cosine of the outer cone half-angle (radians).

      Int32 shadowFrameDataIndex = -1;  ///< Index of the shadow map data for this light, or -1 if shadows are disabled.
      Int32 padding0 = 0;
      Int32 padding1 = 0;
      Int32 padding2 = 0;
    };

    static_assert(sizeof(SpotLight) % 16 == 0, "SpotLight must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for all lights in a single frame, containing arrays of
     * directional, omni, and spot lights.
     */
    struct alignas(16) HC_CORE_EXPORT Lights
    {
      static constexpr UInt32 MAX_OMNI_LIGHTS = 16;
      static constexpr UInt32 MAX_SPOT_LIGHTS = 8;
      static constexpr UInt32 MAX_DIRECTIONAL_LIGHTS = 4;

      DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
      OmniLight omniLights[MAX_OMNI_LIGHTS];
      SpotLight spotLights[MAX_SPOT_LIGHTS];

      Int32 numDirectionalLights = 0; //< Number of active directional lights in the frame.
      Int32 numOmniLights = 0;        //< Number of active omni lights in the frame.
      Int32 numSpotLights = 0;        //< Number of active spot lights in the frame.
      Int32 padding = 0;
    };

    static_assert(sizeof(Lights) % 16 == 0, "Lights must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single directional light's shadow metadata.
     */
    struct alignas(16) HC_CORE_EXPORT DirectionalLightShadow
    {
      Matrix4 LightViewProjectionMatrix;  ///< Light's view-projection matrix for shadow mapping.

      float shadowBias = 0.005f;          ///< Bias to reduce shadow acne.
      float shadowStrength = 1.0f;        ///< Strength of the shadow (0.0 to 1.0).
      Int32 shadowMapIndex = -1;          ///< Index of the shadow map texture in the array.
      Int32 padding = 0;
    };

    static_assert(sizeof(DirectionalLightShadow) % 16 == 0, "DirectionalLightShadow must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single spot light's shadow metadata.
     */
    struct alignas(16) HC_CORE_EXPORT SpotLightShadow
    {
      Matrix4 lightViewProjectionMatrix;  ///< Light's view-projection matrix for shadow mapping.

      float shadowBias = 0.005f;          ///< Bias to reduce shadow acne.
      float shadowStrength = 1.0f;        ///< Strength of the shadow (0.0 to 1.0).
      float projectionNearPlane = 0.1f;   ///< Near plane distance for the light's projection.
      float projectionFarPlane = 100.0f;  ///< Far plane distance for the light's projection.

      Int32 shadowMapIndex = -1;          ///< Index of the shadow map texture in the array.
      Int32 padding0 = 0;
      Int32 padding1 = 0;
      Int32 padding2 = 0;
    };

    static_assert(sizeof(SpotLightShadow) % 16 == 0, "SpotLightShadow must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for all light shadows in a single frame, containing arrays of
     * directional and spot light shadow data.
     */
    struct alignas(16) HC_CORE_EXPORT LightShadows
    {
      static constexpr UInt32 MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA = 4;
      static constexpr UInt32 MAX_SPOT_LIGHTS_SHADOW_DATA = 8;

      DirectionalLightShadow directionalLightShadowData[MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA];
      SpotLightShadow spotLightShadowData[MAX_SPOT_LIGHTS_SHADOW_DATA];
    };

    static_assert(sizeof(LightShadows) % 16 == 0, "LightShadows must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a single object.
     */
    struct alignas(16) HC_CORE_EXPORT ObjectData
    {
      Matrix4 modelMatrix; ///< Model matrix transforming object space to world space.
    };

    static_assert(sizeof(ObjectData) % 16 == 0, "ObjectData must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a light's view-projection matrix.
     */
    struct alignas(16) HC_CORE_EXPORT LightViewProjection
    {
      Matrix4 lightViewProjectionMatrix; ///< Light's view-projection matrix for shadow mapping.
    };

    static_assert(sizeof(LightViewProjection) % 16 == 0, "LightViewProjection must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for an unlit material.
     */
    struct alignas(16) HC_CORE_EXPORT MaterialUnlit
    {
      Color color;              ///< Base color of the unlit material.

      float alphaCutoff = 0.0f; ///< Alpha cutoff value for transparency.
      float padding0 = 0.0f;
      float padding1 = 0.0f;
      float padding2 = 0.0f;
    };

    static_assert(sizeof(MaterialUnlit) % 16 == 0, "MaterialUnlit must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a Blinn-Phong material.
     */
    struct alignas(16) HC_CORE_EXPORT MaterialBlinnPhong
    {
      Color color;              ///< Base color of the Blinn-Phong material.

      float alphaCutoff = 0.0f; ///< Alpha cutoff value for transparency.
      float shininess = 0.0f;   ///< Shininess factor for specular highlights.
      float padding0 = 0.0f;
      float padding1 = 0.0f;
    };

    static_assert(sizeof(MaterialBlinnPhong) % 16 == 0, "MaterialBlinnPhong must be 16-byte aligned");

    /**
     * @brief GPU-aligned data for a hair material.
     */
    struct alignas(16) HC_CORE_EXPORT MaterialHair
    {
      Vector4f color;                       ///< Base color of the hair material.
      Vector4f primarySpecularColor;        ///< Colors for primary and secondary specular highlights.
      Vector4f secondarySpecularColor;      ///< Colors for primary and secondary specular highlights.

      float alphaCutoff = 0.0f;             ///< Alpha cutoff value for transparency.
      float shininess = 0.0f;               ///< Shininess factor for specular highlights.
      float primarySpecularShift = 0.0f;    ///< Specular shift values for primary and secondary highlights.
      float secondarySpecularShift = 0.0f;  ///< Specular shift values for primary and secondary highlights.

      float specularWidth = 0.0f;           ///< Width of the specular highlight.
      float specularStrength = 0.0f;        ///< Strength of the specular highlight.
      float padding0 = 0.0f;
      float padding1 = 0.0f;
    };

    static_assert(sizeof(MaterialHair) % 16 == 0, "MaterialHair must be 16-byte aligned");
  }
}
