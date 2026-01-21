#pragma once

#include "hc/hcCorePrerequisites.h"
#include <fstream>

namespace hc
{
  /**
   * @brief Interface for JSON serialization and deserialization of objects.
   *
   * IJsonSerializer provides a contract for converting objects of type T to and
   * from their JSON string representation. Implementations should define how to
   * serialize an object to JSON and how to reconstruct an object from a JSON
   * string.
   *
   * @tparam T The type of object to serialize and deserialize.
   */
  template <typename T>
  class IJsonSerializer
  {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IJsonSerializer() = default;

    /**
     * @brief Serializes the given object to a JSON string.
     *
     * @param data Pointer to the object to serialize.
     *
     * @return An Optional containing the JSON string if serialization succeeds,
     * or an empty Optional on failure.
     */
    virtual Optional<String> serialize(const T* data) const = 0;

    /**
     * @brief Deserializes an object from the given JSON string.
     *
     * @param json The JSON string to deserialize from.
     *
     * @return Pointer to the newly created object, or nullptr on failure.
     */
    virtual T* deserialize(const String& json) const = 0;
  };
}
