#include "hc/hcJsonObjectBuilder.h"
#include "hc/hcJsonArrayBuilder.h"
#include "hc/hcJsonFromattedValue.h"
#include "hc/hcColor.h"

namespace hc
{
  JsonObjectBuilder::JsonObjectBuilder() :
    m_propertyName("")
  {
  }

  JsonObjectBuilder::JsonObjectBuilder(const String& name) :
    m_propertyName(name)
  {
  }

  JsonObjectBuilder::~JsonObjectBuilder()
  {
  }

  String JsonObjectBuilder::toString() const
  {
    String result = "";
    if (!m_propertyName.empty())
      result = String::Format("\"%s\":", m_propertyName.c_str());

    result += "{";
    bool first = true;
    for (const auto& property : m_properties)
    {
      if (!first)
        result += ",";

      result += property->toString();
      first = false;
    }
    result += "}";
    return result;
  }

  void JsonObjectBuilder::setPropertyName(const String& name)
  {
    m_propertyName = name;
  }

  void JsonObjectBuilder::setFloat(const String& name, float value)
  {
    String formattedValue = String::Format("\"%s\": %f", name.c_str(), value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonObjectBuilder::setNumber(const String& name, Int32 value)
  {
    String formattedValue = String::Format("\"%s\": %d", name.c_str(), value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonObjectBuilder::setNumber(const String& name, UInt32 value)
  {
    String formattedValue = String::Format("\"%s\": %u", name.c_str(), value);
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonObjectBuilder::setString(const String& name, const String& value)
  {
    String formattedValue = String::Format("\"%s\": \"%s\"", name.c_str(), value.c_str());
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonObjectBuilder::setBool(const String& name, bool value)
  {
    String formattedValue = String::Format("\"%s\": %s", name.c_str(), value ? "true" : "false");
    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  void JsonObjectBuilder::setColor(const String& name, const Color& color)
  {
    String formattedValue = String::Format(
      "\"%s\": { \"r\": %f, \"g\": %f, \"b\": %f, \"a\": %f }",
      name.c_str(),
      color.r,
      color.g,
      color.b,
      color.a
    );

    m_properties.push_back(
      MakeUnique<JsonFormattedValue>(formattedValue)
    );
  }

  JsonArrayBuilder* JsonObjectBuilder::createArray(const String& name)
  {
    UniquePtr<JsonArrayBuilder> newArray = MakeUnique<JsonArrayBuilder>(name);
    JsonArrayBuilder* newArrayPtr = newArray.get();
    m_properties.push_back(std::move(newArray));
    return newArrayPtr;
  }

  JsonObjectBuilder* JsonObjectBuilder::createObject(const String& name)
  {
    UniquePtr<JsonObjectBuilder> newObject = MakeUnique<JsonObjectBuilder>(name);
    JsonObjectBuilder* newObjectPtr = newObject.get();
    m_properties.push_back(std::move(newObject));
    return newObjectPtr;
  }
}
