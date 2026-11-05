#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"

namespace hc::scene::lights
{
  /**
   * @brief Represents an omni-directional light source in the engine.
   */
  struct HC_CORE_EXPORT OmniLight : hc::io::ISerializable
  {
    float range = 10.0f;  ///< The range of the light's influence

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;
  };

  /**
   * @brief Represents a directional light source in the engine.
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
  };

  /**
   * @brief Represents a spotlight source in the engine.
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
  };

  using LightVariant = std::variant<
    OmniLight,
    DirectionalLight,
    SpotLight
  >;

  /**
   * @brief Represents a light source in the engine.
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
  };
}
