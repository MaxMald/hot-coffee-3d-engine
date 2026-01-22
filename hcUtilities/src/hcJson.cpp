#include "hc/hcJson.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

namespace hc
{
  struct Json::Impl
  {
    nlohmann::json json;
    Impl() = default;
    Impl(const nlohmann::json& j) : json(j) {}
    Impl(nlohmann::json&& j) : json(std::move(j)) {}
  };

  Json::Json()
    : m_impl(new Impl())
  {
  }

  Json::Json(const Json& other)
    : m_impl(new Impl(other.m_impl->json))
  {
  }

  Json::Json(Json&& other) noexcept
    : m_impl(other.m_impl)
  {
    other.m_impl = nullptr;
  }

  Json::~Json()
  {
    delete m_impl;
  }

  Json Json::parse(const String& jsonString)
  {
    if (jsonString.empty())
      throw std::invalid_argument("jsonString is empty");
    return Json(new Impl(nlohmann::json::parse(jsonString)));
  }

  Json Json::loadFromFile(const Path& filePath)
  {
    if (filePath.empty())
      throw std::invalid_argument("filePath is empty");
    std::ifstream file(filePath);
    if (!file)
      throw std::runtime_error("Failed to open JSON file");

    return Json(new Impl(nlohmann::json::parse(file, nullptr, true, true)));
  }

  Json& Json::operator=(const Json& other)
  {
    if (this != &other)
    {
      m_impl->json = other.m_impl->json;
    }
    return *this;
  }

  Json& Json::operator=(Json&& other) noexcept
  {
    if (this != &other)
    {
      delete m_impl;
      m_impl = other.m_impl;
      other.m_impl = nullptr;
    }
    return *this;
  }

  Json Json::operator[](const String& key) const
  {
    if (!m_impl->json.is_object())
      throw std::out_of_range("Not a JSON object");
    return Json(new Impl(m_impl->json[key]));
  }

  Json Json::operator[](SizeT index) const
  {
    if (!m_impl->json.is_array())
      throw std::out_of_range("Not a JSON array");
    return Json(new Impl(m_impl->json.at(static_cast<nlohmann::json::size_type>(index))));
  }

  bool Json::contains(const String& key) const
  {
    return m_impl->json.is_object() && m_impl->json.contains(key);
  }

  bool Json::isNull() const
  {
    return m_impl->json.is_null();
  }

  bool Json::isObject() const
  {
    return m_impl->json.is_object();
  }

  bool Json::isArray() const
  {
    return m_impl->json.is_array();
  }

  bool Json::isString() const
  {
    return m_impl->json.is_string();
  }

  bool Json::isNumber() const
  {
    return m_impl->json.is_number();
  }

  bool Json::isBoolean() const
  {
    return m_impl->json.is_boolean();
  }

  SizeT Json::getStringLength() const
  {
    if (!m_impl->json.is_string())
      throw std::runtime_error("Not a JSON string");
    return static_cast<SizeT>(m_impl->json.get_ref<const std::string&>().size());
  }

  SizeT Json::getSize() const
  {
    if (!m_impl->json.is_array())
      throw std::runtime_error("Not a JSON array");
    return static_cast<SizeT>(m_impl->json.size());
  }

  String Json::getString() const
  {
    if (!m_impl->json.is_string())
      throw std::runtime_error("Not a JSON string");
    return m_impl->json.get<std::string>().c_str();
  }

  void Json::getArrayInt32(Int32* outArray, const SizeT& arraySize) const
  {
    if (!m_impl->json.is_array())
      throw std::runtime_error("Not a JSON array");
    if (!outArray && arraySize > 0)
      throw std::invalid_argument("outArray is null");
    SizeT count = std::min(arraySize, static_cast<SizeT>(m_impl->json.size()));
    for (SizeT i = 0; i < count; ++i)
    {
      if (!m_impl->json[i].is_number_integer())
        throw std::runtime_error("Array element is not an integer");
      outArray[i] = static_cast<Int32>(m_impl->json[i]);
    }
  }

  bool Json::getBool() const
  {
    if (!m_impl->json.is_boolean())
      throw std::runtime_error("Not a JSON boolean");
    return m_impl->json.get<bool>();
  }

  Int32 Json::getInt32() const
  {
    if (!m_impl->json.is_number_integer())
      throw std::runtime_error("Not a JSON integer");
    return m_impl->json.get<Int32>();
  }

  float Json::getFloat() const
  {
    if (!m_impl->json.is_number())
      throw std::runtime_error("Not a JSON number");
    return m_impl->json.get<float>();
  }

  Json::Json(Impl* impl)
    : m_impl(impl)
  {
  }
}
