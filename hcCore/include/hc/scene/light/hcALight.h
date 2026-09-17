#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"

namespace hc
{
  /**
   * @brief Represents a light source in the engine.
   */
  class HC_CORE_EXPORT ALight : public io::ISerializable
  {
  public:
    virtual ~ALight() = default;

    /**
     * @copydoc ISerializable::serialize
     */
    virtual void serialize(io::BinaryWriter& writer) const override;
    
    /**
     * @copydoc ISerializable::deserialize
     */
    virtual void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Gets the type of the light.
     * 
     * @return The current light type.
     */
    lightType::Type getType() const;

    /**
     * @brief Sets the color of the light.
     * 
     * @param color The new color.
     */
    void setColor(const Color& color);

    /**
     * @brief Sets the color of the light using RGBA components.
     * 
     * @param r Red component.
     * @param g Green component.
     * @param b Blue component.
     */
    void setColor(float r, float g, float b);

    /**
     * @brief Gets the color of the light.
     * 
     * @return The current color.
     */
    const Color& getColor() const;

    /**
     * @brief Sets the intensity of the light.
     * 
     * @param intensity The new intensity value.
     */
    void setIntensity(float intensity);

    /**
     * @brief Gets the intensity of the light.
     * 
     * @return The current intensity value.
     */
    float getIntensity() const;

    /**
     * @brief Sets the range of the light.
     *
     * @param range The new range value.
     */
    void setRange(float range);

    /**
     * @brief Gets the range of the light.
     *
     * @return The current range value.
     */
    float getRange() const;

    /**
     * @brief Sets the position of the light.
     * 
     * @param position The new position vector.
     */
    void setPosition(const Vector3f& position);

    /**
     * @brief Gets the position of the light.
     * 
     * @return The current position vector.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Enables or disables the light.
     */
    void setEnabled(bool isEnabled);

    /**
     * @brief Checks if the light is enabled.
     * 
     * @return True if the light is enabled, false otherwise.
     */
    bool isEnabled() const;

    /**
     * @brief Enables or disables shadow casting for the light.
     * 
     * @param isEnabled True to enable shadows, false to disable.
     */
    void setShadowsEnabled(bool isEnabled);

    /**
     * @brief Checks if shadow casting is enabled for the light.
     * 
     * @return True if shadow casting is enabled, false otherwise.
     */
    bool isShadowsEnabled() const;

    /**
     * @brief Sets the shadow bias for the light.
     *
     * @param bias The new shadow bias value.
     */
    void setShadowBias(float bias);

    /**
     * @brief Gets the shadow bias for the light.
     *
     * @return The current shadow bias value.
     */
    float getShadowBias() const;

    /**
     * @brief Sets the shadow strength for the light.
     *
     * @param strength The new shadow strength value.
     */
    void setShadowStrength(float strength);

    /**
     * @brief Gets the shadow strength for the light.
     *
     * @return The current shadow strength value.
     */
    float getShadowStrength() const;

  protected:
    Color m_color;
    Vector3f m_position;
    float m_intensity;
    float m_range;
    float m_shadowBias;
    float m_shadowStrength;
    lightType::Type m_type;
    bool m_enabled;
    bool m_shadowsEnabled;

    ALight(lightType::Type type);

    /**
     * @brief Serializes the derived light properties to binary format.
     *
     * This method should be implemented by derived classes to serialize their
     * specific properties. It is called by the base class's serialize method.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void onSerialize(io::BinaryWriter& writer) const = 0;

    /**
     * @brief Deserializes the derived light properties from binary format.
     *
     * This method should be implemented by derived classes to deserialize their
     * specific properties. It is called by the base class's deserialize method.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void onDeserialize(io::BinaryReader& reader) = 0;

    /**
     * @brief Gets the derived version of the light for serialization purposes.
     *
     * @return The derived version as a UInt16.
     */
    virtual UInt16 getDerivedVersion() const = 0;
  };
}
