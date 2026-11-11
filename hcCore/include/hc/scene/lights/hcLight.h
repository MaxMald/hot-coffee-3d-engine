#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/lights/hcLightType.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc::scene::lights
{
  /**
   * @brief Represents an omni-directional light source in the engine.
   *
   * @note Use the setter methods to safely modify the properties of the OmniLight.
   */
  struct HC_CORE_EXPORT OmniLight : hc::io::ISerializable
  {
    float range = 10.0f;  ///< The range of the light's influence

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Sets the range of the OmniLight.
     * @param newRange The new range value to set. Must be non-negative.
     */
    inline void setRange(float newRange)
    {
      range = Math::Max(newRange, 0.0f);
    }
  };

  /**
   * @brief Represents a directional light source in the engine.
   *
   * @note Use the setter methods to safely modify the properties of the DirectionalLight.
   */
  struct HC_CORE_EXPORT DirectionalLight : hc::io::ISerializable
  {
    Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);         ///< The direction vector of the directional light
    Vector3f shadowViewTarget = Vector3f(0.0f, 0.0f, 0.0f);   ///< The target point for the shadow view
    float shadowViewDistance = 0.0f;                          ///< The distance for the shadow view
    float shadowProjectionSize = 0.0f;                        ///< The size of the shadow projection
    float shadowProjectionNearPlane = 0.0f;                   ///< The near plane distance for shadow projection
    float shadowProjectionFarPlane = 0.0f;                    ///< The far plane distance for shadow projection

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Sets the direction of the DirectionalLight.
     * @param newDirection The new direction vector to set. It will be normalized.
     */
    inline void setDirection(const Vector3f& newDirection)
    {
      direction = newDirection.normalized();
    }

    /**
     * @brief Sets the shadow view target of the DirectionalLight.
     * @param newTarget The new shadow view target position to set.
     */
    inline void setShadowViewDistance(float newDistance)
    {
      shadowViewDistance = Math::Max(newDistance, 0.0f);
    }

    /**
     * @brief Sets the size of the shadow projection for the DirectionalLight.
     * @param newSize The new size of the shadow projection to set. Must be non-negative.
     */
    inline void setShadowProjectionSize(float newSize)
    {
      shadowProjectionSize = Math::Max(newSize, 0.001f);
    }

    /**
     * @brief Sets the near plane distance for the shadow projection of the
     * DirectionalLight.
     * @param newNearPlane The new near plane distance to set. Must be non-negative.
     */
    inline void setShadowProjectionNearPlane(float newNearPlane)
    {
      shadowProjectionNearPlane = Math::Min(
        Math::Max(newNearPlane, 0.0f),
        shadowProjectionFarPlane - 0.001f
      );
    }

    /**
     * @brief Sets the far plane distance for the shadow projection of the DirectionalLight.
     * @param newFarPlane The new far plane distance to set. Must be non-negative.
     */
    inline void setShadowProjectionFarPlane(float newFarPlane)
    {
      shadowProjectionFarPlane = Math::Max(
        newFarPlane,
        shadowProjectionNearPlane + 0.001f
      );
    }
  };

  /**
   * @brief Represents a spotlight source in the engine.
   *
   * @note Use the setter methods to safely modify the properties of the SpotLight.
   */
  struct HC_CORE_EXPORT SpotLight : hc::io::ISerializable
  {
    Vector3f direction = Vector3f(0.0f, 0.0f, -1.0f);   ///< The direction vector of the spotlight
    Angle innerConeAngle = Angle::FromDegrees(15.0f);   ///< The inner cone angle of the spotlight, defining the area of full illumination
    Angle outerConeAngle = Angle::FromDegrees(30.0f);   ///< The outer cone angle of the spotlight, defining the edge of the light cone
    float shadowProjectionNearPlane = 0.1f;             ///< The near plane distance for shadow projection
    float shadowProjectionFarPlane = 100.0f;            ///< The far plane distance for shadow projection
    float range = 10.0f;                                ///< The range of the light's influence

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Sets the direction of the SpotLight.
     * @param newDirection The new direction vector to set. It will be normalized.
     */
    inline void setInnerConeAngle(const Angle& newInnerConeAngle)
    {
      innerConeAngle = newInnerConeAngle < outerConeAngle ? newInnerConeAngle : outerConeAngle;
      innerConeAngle = Math::Clamp(innerConeAngle, Angle::FromDegrees(0.0f), Angle::FromDegrees(90.0f));
    }

    /**
     * @brief Sets the outer cone angle of the SpotLight.
     * @param newOuterConeAngle The new outer cone angle to set. It must be greater than
     * or equal to the inner cone angle.
     */
    inline void setOuterConeAngle(const Angle& newOuterConeAngle)
    {
      outerConeAngle = newOuterConeAngle > innerConeAngle ? newOuterConeAngle : innerConeAngle;
      outerConeAngle = Math::Clamp(outerConeAngle, Angle::FromDegrees(0.0f), Angle::FromDegrees(90.0f));
    }

    /**
     * @brief Sets the near plane distance for the shadow projection of the
     * SpotLight.
     * @param newNearPlane The new near plane distance to set. Must be non-negative.
     */
    inline void setShadowProjectionNearPlane(float newNearPlane)
    {
      shadowProjectionNearPlane = Math::Max(newNearPlane, 0.001f);
    }

    /**
     * @brief Sets the far plane distance for the shadow projection of the SpotLight.
     * @param newFarPlane The new far plane distance to set. Must be non-negative.
     */
    inline void setShadowProjectionFarPlane(float newFarPlane)
    {
      shadowProjectionFarPlane = Math::Max(newFarPlane, 0.002f);
    }

    /**
     * @brief Sets the range of the SpotLight.
     * @param newRange The new range value to set. Must be non-negative.
     */
    inline void setRange(float newRange)
    {
      range = Math::Max(newRange, 0.0f);
    }
  };

  using LightVariant = std::variant<
    OmniLight,
    DirectionalLight,
    SpotLight
  >;

  /**
   * @brief Represents a light source in the engine.
   *
   * @note Use the setter methods to safely modify the properties of the Light.
   */
  class HC_CORE_EXPORT Light : public io::ISerializable
  {
  private:
    LightVariant m_variantData; ///< Variant data holding specific light properties based on the light type

  public:
    Color color;          ///< The color of the light
    Vector3f position;    ///< The position of the light in 3D space
    float intensity;      ///< The intensity of the light
    float shadowBias;     ///< The bias applied to shadow mapping to reduce artifacts
    float shadowStrength; ///< The strength of the shadows cast by the light
    bool enabled;         ///< Whether the light is enabled
    bool shadowsEnabled;  ///< Whether shadows are enabled for the light

  private:
    lightType::Type m_type; ///< The type of the light (e.g., Omni, Directional, Spot)

  public:
    /**
     * @brief Creates a light source with default properties.
     */
    Light();

    /**
     * @brief Destroys the light source.
     */
    ~Light();

    /**
     * @copydoc io::ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc io::ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Sets a new light type for this light source. This will reset the variant
     * data to the default state for the new light type.
     * @param lightType The new light type to set (e.g., Omni, Directional, Spot)
     */
    void setType(lightType::Type lightType);

    /**
     * @brief Gets the current light type of this light source.
     * @return The current light type (e.g., Omni, Directional, Spot)
     */
    inline lightType::Type getType() const
    {
      return m_type;
    }
    
    /**
     * @brief Sets the intensity of the light.
     * @param newIntensity The new intensity value to set. Must be non-negative.
     */
    inline void setIntensity(float newIntensity)
    {
      intensity = Math::Max(newIntensity, 0.0f);
    }

    /**
     * @brief Sets the shadow bias of the light.
     * @param newShadowBias The new shadow bias value to set. Must be non-negative.
     */
    inline void setShadowBias(float newShadowBias)
    {
      shadowBias = Math::Max(newShadowBias, 0.0f);
    }

    /**
     * @brief Sets the shadow strength of the light.
     * @param newShadowStrength The new shadow strength value to set. Must be between
     * 0.0 and 1.0.
     */
    inline void setShadowStrength(float newShadowStrength)
    {
      shadowStrength = Math::Clamp(newShadowStrength, 0.0f, 1.0f);
    }

    /**
     * @brief Clears the light's state, resetting it to default values. The type of light
     * remains unchanged, but its properties are reset to their defaults.
     */
    void clear();

    /**
     * @brief Retrieves a pointer to the OmniLight data if the light type is Omni.
     * @return A pointer to the OmniLight data if the light type is Omni; otherwise,
     * nullptr.
     */
    inline const OmniLight* getIfOmniLight() const
    {
      return std::get_if<OmniLight>(&m_variantData);
    }

    /**
     * @brief Retrieves a pointer to the OmniLight data if the light type is Omni.
     * @return A pointer to the OmniLight data if the light type is Omni; otherwise,
     * nullptr.
     */
    inline OmniLight* getIfOmniLight()
    {
      return std::get_if<OmniLight>(&m_variantData);
    }

    /**
     * @brief Retrieves a pointer to the DirectionalLight data if the light type is
     * Directional.
     * @return A pointer to the DirectionalLight data if the light type is Directional;
     * otherwise, nullptr.
     */
    inline const DirectionalLight* getIfDirectionalLight() const
    {
      return std::get_if<DirectionalLight>(&m_variantData);
    }

    /**
     * @brief Retrieves a pointer to the DirectionalLight data if the light type is
     * Directional.
     * @return A pointer to the DirectionalLight data if the light type is Directional;
     * otherwise, nullptr.
     */
    inline DirectionalLight* getIfDirectionalLight()
    {
      return std::get_if<DirectionalLight>(&m_variantData);
    }

    /**
     * @brief Retrieves a pointer to the SpotLight data if the light type is Spot.
     * @return A pointer to the SpotLight data if the light type is Spot; otherwise,
     * nullptr.
     */
    inline const SpotLight* getIfSpotLight() const
    {
      return std::get_if<SpotLight>(&m_variantData);
    }

    /**
     * @brief Retrieves a pointer to the SpotLight data if the light type is Spot.
     * @return A pointer to the SpotLight data if the light type is Spot; otherwise,
     * nullptr.
     */
    inline SpotLight* getIfSpotLight()
    {
      return std::get_if<SpotLight>(&m_variantData);
    }

    /**
     * @brief Gets the GPU-aligned data structure representing this light as an OmniLight.
     * @return A Light data structure containing the light's properties.
     * @throws RuntimeErrorException if the current light type is not an OmniLight.
     */
    dataBlockStructure::OmniLight getOmniLightDataBlockStructure() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this light as a
     * DirectionalLight.
     * @return A Light data structure containing the light's properties.
     * @throws RuntimeErrorException if the current light type is not a DirectionalLight.
     */
    dataBlockStructure::DirectionalLight getDirectionalLightDataBlockStructure() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this light as a SpotLight.
     * @return A Light data structure containing the light's properties.
     * @throws RuntimeErrorException if the current light type is not a SpotLight.
     */
    dataBlockStructure::SpotLight getSpotLightDataBlockStructure() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this light's shadow
     * properties as a DirectionalLightShadow.
     * @return A DirectionalLightShadow data structure containing the light's shadow
     * properties.
     * @throws RuntimeErrorException if the current light type is not a DirectionalLight.
     */
    dataBlockStructure::DirectionalLightShadow
      getDirectionalLightShadowDataBlockStructure() const;

    /**
     * @brief Gets the GPU-aligned data structure representing this light's shadow
     * properties as a SpotLightShadow.
     * @return A SpotLightShadow data structure containing the light's shadow
     * properties.
     * @throws RuntimeErrorException if the current light type is not a SpotLight.
     */
    dataBlockStructure::SpotLightShadow getSpotLightShadowDataBlockStructure() const;
  };
}
