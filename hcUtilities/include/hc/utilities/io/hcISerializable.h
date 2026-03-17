#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"

namespace hc
{
  class BinaryReader;
  class BinaryWriter;

  /**
   * @brief Interface for objects that can be serialized and deserialized.
   *
   * ISerializable provides a contract for objects to implement binary
   * serialization and deserialization. Classes implementing this interface
   * can save their state to a binary stream and restore it from a binary
   * stream using BinaryWriter and BinaryReader.
   */
  class HC_UTILITY_EXPORT ISerializable
  {
  public:
    virtual ~ISerializable();

    /**
     * @brief Serializes the object to binary format.
     *
     * Writes the object's state to the provided BinaryWriter. The
     * implementation should write all necessary data to fully reconstruct
     * the object's state during deserialization.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void serialize(BinaryWriter& writer) const = 0;

    /**
     * @brief Deserializes the object from binary format.
     *
     * Reads the object's state from the provided BinaryReader. The
     * implementation should read data in the same order it was written
     * during serialization to properly reconstruct the object's state.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void deserialize(BinaryReader& reader) = 0;

  protected:
    ISerializable();
  };
}
