#include "hc/hcJsonArrayBuilder.h"
#include "hc/hcJsonObjectBuilder.h"
#include "hc/hcJsonFromattedValue.h"
#include "hc/hcColor.h"

namespace hc
{
  JsonArrayBuilder::JsonArrayBuilder() :
    m_propertyName("")
  {
  }

  JsonArrayBuilder::JsonArrayBuilder(const String& name) :
    m_propertyName(name)
  {
  }

  JsonArrayBuilder::~JsonArrayBuilder()
  {
  }

  String JsonArrayBuilder::toString() const
  {
    String result = "";
    if (!m_propertyName.empty())
      result = String::Format("\"%s\":", m_propertyName.c_str());

    result += "[";
    bool first = true;
    for (const auto& property : m_properties)
    {
      if (!first)
        result += ",";

      result += property->toString();
      first = false;
    }
    result += "]";
    return result;
  }

  void JsonArrayBuilder::setPropertyName(const String& name)
  {
    m_propertyName = name;
  }

  void JsonArrayBuilder::setFloat(float value)
  {
    String formattedValue = String::Format("%f", value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonArrayBuilder::setNumber(Int32 value)
  {
    String formattedValue = String::Format("%d", value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonArrayBuilder::setNumber(UInt32 value)
  {
    String formattedValue = String::Format("%u", value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonArrayBuilder::setString(const String& value)
  {
    String formattedValue = String::Format("\"%s\"", value.c_str());
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonArrayBuilder::setBool(bool value)
  {
    String formattedValue = value ? "true" : "false";
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

    void JsonArrayBuilder::setColor(const Color& color)
  {
    String formattedValue = String::Format(
      "{ \"r\": %f, \"g\": %f, \"b\": %f, \"a\": %f }",
      color.r,
      color.g,
      color.b,
      color.a
    );

    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  JsonArrayBuilder* JsonArrayBuilder::createArray()
  {
    UniquePtr<JsonArrayBuilder> newArray = MakeUnique<JsonArrayBuilder>();
    JsonArrayBuilder* newArrayPtr = newArray.get();
    m_properties.push_back(std::move(newArray));
    return newArrayPtr;
  }

  JsonObjectBuilder* JsonArrayBuilder::createObject()
  {
    UniquePtr<JsonObjectBuilder> newObject = MakeUnique<JsonObjectBuilder>();
    JsonObjectBuilder* newObjectPtr = newObject.get();
    m_properties.push_back(std::move(newObject));
    return newObjectPtr;
  }
}
