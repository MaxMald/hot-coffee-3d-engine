#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIJsonSerializer.h"

namespace hc
{
  /**
   * @brief Singleton utility for registering and using JSON serializers for
   * various types.
   */
  class HC_CORE_EXPORT JsonSerializer : public NonCopyable
  {
  public:
    /*
    * @brief Gets the singleton instance of the JsonSerializer.
    * 
    * @return Reference to the JsonSerializer instance.
    */
    static JsonSerializer& Instance();

    /*
    * @brief Prepares the JsonSerializer for use.
    */
    static void Prepare();

    /*
    * @brief Shuts down the JsonSerializer.
    */
    static void Shutdown();

    /**
     * @brief Serializes an object to a JSON string using the registered serializer.
     *
     * @tparam T The type of the object.
     *
     * @param data Pointer to the object to serialize.
     *
     * @return An Optional containing the JSON string if successful, or empty on failure.
     */
    template<typename T>
    static Optional<String> Serialize(const T* data);

    /**
     * @brief Deserializes an object from a JSON string using the registered
     * serializer.
     *
     * @tparam T The type of the object.
     *
     * @param json The JSON string to deserialize from.
     *
     * @return Pointer to the newly created object, or nullptr on failure.
     */
    template<typename T>
    static T* Deserialize(const String& json);

    ~JsonSerializer() = default;

    /**
     * @brief Registers a JSON serializer for a specific type.
     *
     * @tparam T The type to register the serializer for.
     *
     * @param serializer Unique pointer to the serializer instance.
     */
    template<typename T>
    void registerSerializer(UniquePtr<IJsonSerializer<T>> serializer);

    /**
     * @brief Serializes an object to a JSON string using the registered
     * serializer.
     *
     * @tparam T The type of the object.
     *
     * @param data Pointer to the object to serialize.
     *
     * @return An Optional containing the JSON string if successful, or empty on
     * failure.
     */
    template<typename T>
    Optional<String> serialize(const T* data) const;

    /**
     * @brief Deserializes an object from a JSON string using the registered
     * serializer.
     *
     * @tparam T The type of the object.
     *
     * @param json The JSON string to deserialize from.
     *
     * @return Pointer to the newly created object, or nullptr on failure.
     */
    template<typename T>
    T* deserialize(const String& json) const;

  private:
    static JsonSerializer* s_instance;

    UnorderedMap<TypeIndex, UniquePtr<IJsonSerializer<void>>> m_serializers;

    JsonSerializer();
    void onPrepare();
  };

  template<typename T>
  Optional<String> JsonSerializer::Serialize(const T* data)
  {
    return Instance().serialize<T>(data);
  }

  template<typename T>
  T* JsonSerializer::Deserialize(const String& json)
  {
    return Instance().deserialize<T>(json);
  }

  template<typename T>
  void JsonSerializer::registerSerializer(UniquePtr<IJsonSerializer<T>> serializer)
  {
    TypeIndex typeIdx(typeid(T));
    m_serializers[typeIdx] = std::move(
      reinterpret_cast<UniquePtr<IJsonSerializer<void>>&>(serializer)
    );
  }

  template<typename T>
  Optional<String> JsonSerializer::serialize(const T* data) const
  {
    TypeIndex typeIdx(typeid(T));
    auto it = m_serializers.find(typeIdx);
    if (it == m_serializers.end())
      return {};

    auto* serializer = reinterpret_cast<IJsonSerializer<T>*>(it->second.get());
    return serializer->serialize(data);
  }

  template<typename T>
  T* JsonSerializer::deserialize(const String& json) const
  {
    std::type_index typeIdx(typeid(T));
    auto it = m_serializers.find(typeIdx);
    if (it == m_serializers.end())
      return nullptr;

    auto* serializer = reinterpret_cast<IJsonSerializer<T>*>(it->second.get());
    return serializer->deserialize(json);
  }
}
