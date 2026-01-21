#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"
#include "hc/hcIJsonProperty.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class JsonObjectBuilder;
  struct Color;

  /**
   * @brief Utility class for building and serializing JSON arrays.
   *
   * JsonArrayBuilder provides a fluent interface to construct JSON arrays by
   * code.
   */
  class HC_UTILITY_EXPORT JsonArrayBuilder : 
    public IJsonProperty,
    public NonCopyable
  {
  public:
    JsonArrayBuilder();

    /**
     * @brief Constructs a JSON array with the specified property name.
     *
     * @param name The property name for this array.
     */
    JsonArrayBuilder(const String& name);
    virtual ~JsonArrayBuilder();

    /**
     * @copydoc IJsonProperty::toString
     */
    virtual String toString() const override;

    /**
     * @brief Sets the property name for this array.
     * 
     * @param name The property name to set.
     */
    void setPropertyName(const String& name);

    /**
     * @brief Adds a float value to the array.
     * 
     * @param value The float value to add.
     */
    void setFloat(float value);

    /**
     * @brief Adds a signed integer value to the array.
     * 
     * @param value The Int32 value to add.
     */
    void setNumber(Int32 value);

    /**
     * @brief Adds an unsigned integer value to the array.
     * 
     * @param value The UInt32 value to add.
     */
    void setNumber(UInt32 value);

    /**
     * @brief Adds a string value to the array.
     * @param value The string value to add.
     */
    void setString(const String& value);

    /**
     * @brief Adds a boolean value to the array.
     * 
     * @param value The boolean value to add.
     */
    void setBool(bool value);

    /**
     * @brief Adds a color value to the array.
     * 
     * @param color The Color value to add.
     */
    void setColor(const Color& color);

    /**
     * @brief Creates and adds a new nested array to this array.
     * 
     * @return Pointer to the newly created JsonArray.
     */
    JsonArrayBuilder* createArray();

    /**
     * @brief Creates and adds a new nested object to this array.
     * 
     * @return Pointer to the newly created JsonObject.
     */
    JsonObjectBuilder* createObject();

  private:
    String m_propertyName;
    Vector<UniquePtr<IJsonProperty>> m_properties;
  };
}
