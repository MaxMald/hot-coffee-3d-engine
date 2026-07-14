#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"

namespace hc
{
  /**
   * @brief Represents a light source in the engine.
   */
  class HC_CORE_EXPORT ALight : public ISerializable
  {
  public:
    virtual ~ALight() = default;

    /**
     * @copydoc ISerializable::serialize
     */
    virtual void serialize(BinaryWriter& writer) const override;
    
    /**
     * @copydoc ISerializable::deserialize
     */
    virtual void deserialize(BinaryReader& reader) override;

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

  protected:
    Color m_color;
    Vector3f m_position;
    float m_intensity;
    float m_range;
    lightType::Type m_type;
    bool m_enabled;
    bool m_shadowsEnabled;

    ALight(lightType::Type type);
  };
}
