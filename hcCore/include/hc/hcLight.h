#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcColor.h"
#include "hc/hcVector3.h"
#include "hc/hcLightType.h"

namespace hc
{
  /**
   * @brief Represents a light source in the engine.
   */
  class HC_CORE_EXPORT Light
  {
  public:
    /**
     * @brief Constructs a Light with the specified type.
     * 
     * @param type The type of the light (default is Point).
     */
    Light(lightType::Type type = lightType::Type::Point);

    /**
     * @brief Sets the type of the light.
     * 
     * @param type The new light type.
     */
    void setType(lightType::Type type);

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
     * @brief Sets the direction of the light.
     * 
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the direction of the light.
     * 
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

  private:
    lightType::Type m_type;
    Color m_color;
    float m_intensity;
    Vector3f m_position;
    Vector3f m_direction;
  };
}
