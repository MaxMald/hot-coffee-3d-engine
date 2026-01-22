#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"
#include "hc/hcIJsonProperty.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class JsonArrayBuilder;
  struct Color;

  /**
   * @brief Utility class for building and serializing JSON objects.
   *
   * JsonObjectBuilder provides a fluent interface to construct JSON objects by
   * code, allowing the addition of various property types, nested objects, and
   * arrays.
   */
  class HC_UTILITY_EXPORT JsonObjectBuilder :
    public IJsonProperty,
    public NonCopyable
  {
  public:
    JsonObjectBuilder();

    /**
     * @brief Constructs a JSON object with the specified property name.
     *
     * @param name The property name for this object.
     */
    JsonObjectBuilder(const String& name);
    virtual ~JsonObjectBuilder();

    /**
     * @brief Converts the object and its contents to a JSON string
     * representation.
     *
     * @return The JSON-formatted string.
     */
    virtual String toString() const override;

    /**
     * @brief Sets the property name for this object.
     *
     * @param name The property name to set.
     */
    void setPropertyName(const String& name);

    /**
     * @brief Adds a float value to the object with the specified property name.
     *
     * @param name The property name for the value.
     * @param value The float value to add.
     */
    void setFloat(const String& name, float value);

    /**
     * @brief Adds a signed integer value to the object with the specified
     * property name.
     *
     * @param name The property name for the value.
     * @param value The Int32 value to add.
     */
    void setNumber(const String& name, Int32 value);

    /**
     * @brief Adds an unsigned integer value to the object with the specified
     * property name.
     *
     * @param name The property name for the value.
     * @param value The UInt32 value to add.
     */
    void setNumber(const String& name, UInt32 value);

    /**
     * @brief Adds a string value to the object with the specified property name.
     *
     * @param name The property name for the value.
     * @param value The string value to add.
     */
    void setString(const String& name, const String& value);

    /**
     * @brief Adds a file path value to the object with the specified property name.
     *
     * @param name The property name for the value.
     * @param value The file path value to add.
     */
    void setPath(const String& name, const Path& value);

    /**
     * @brief Adds a boolean value to the object with the specified property
     * name.
     *
     * @param name The property name for the value.
     * @param value The boolean value to add.
     */
    void setBool(const String& name, bool value);

    /**
     * @brief Adds a color value to the object with the specified property name.
     *
     * @param name The property name for the value.
     * @param color The Color value to add.
     */
    void setColor(const String& name, const Color& color);

    /**
     * @brief Creates and adds a new nested array to this object.
     *
     * @param name The property name for the new array.
     * @return Pointer to the newly created JsonArray.
     */
    JsonArrayBuilder* createArray(const String& name);

    /**
     * @brief Creates and adds a new nested object to this object.
     *
     * @param name The property name for the new object.
     * @return Pointer to the newly created JsonObject.
     */
    JsonObjectBuilder* createObject(const String& name);

  private:
    String m_propertyName;
    Vector<UniquePtr<IJsonProperty>> m_properties;
  };
}
