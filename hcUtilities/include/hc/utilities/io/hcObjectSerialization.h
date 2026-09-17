#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"

namespace hc::io
{
  /**
   * @brief Represents the header of a serialized object.
   *
   * This information is used during serialization and deserialization to ensure
   * compatibility and proper handling of the object's data.
   */
  struct alignas(8) ObjectHeader
  {
    UInt64 size = static_cast<UInt64>(0);    ///< The size of the object in bytes.
    UInt32 type = static_cast<UInt32>(0);    ///< The type identifier of the object.
    UInt32 version = static_cast<UInt32>(0); ///< The version of the object.

    /**
     * @brief Checks if the header matches a given version.
     *
     * @param version The version to compare against.
     * 
     * @return True if the versions match, false otherwise.
     */
    bool matchVersion(UInt32 _version) const
    {
      return this->version == _version;
    }

    /**
     * @brief Checks if the header matches a given type.
     *
     * @param type The type to compare against.
     *
     * @return True if the types match, false otherwise.
     */
    bool matchType(UInt32 _type) const
    {
      return this->type == _type;
    }

    /**
     * @brief Checks if the header matches a given type and version.
     *
     * @param type The type to compare against.
     * @param version The version to compare against.
     * 
     * @return True if both the type and version match, false otherwise.
     */
    bool match(const UInt32& _type, UInt32 _version) const
    {
      return this->type == _type && this->version == _version;
    }
  };

  /**
   * @brief Represents a serialized object with its header and data.
   */
  class ObjectData
  {
  public:
    ObjectData();
    ObjectData(UInt32 type, UInt32 version);
    ObjectData(const ObjectHeader& header);

    /**
     * @brief Appends raw data to the object.
     *
     * This method appends the provided data to the object's internal data buffer
     * and updates the size in the header accordingly.
     *
     * @param data Pointer to the raw data to append.
     * @param dataSize Size of the data in bytes.
     */
    void append(const void* data, SizeT dataSize);

    /**
     * @brief Reads data from the object without consuming it.
     *
     * This method reads the specified amount of data from the object's internal
     * data buffer into the provided buffer without removing that data from the
     * internal buffer. The size in the header remains unchanged.
     *
     * @param data Pointer to the buffer where the data will be copied.
     * @param dataSize Size of the data to read in bytes.
     */
    void peek(Byte* data, SizeT dataSize);

    /**
     * @brief Reads and consumes data from the object.
     *
     * This method reads the specified amount of data from the object's internal
     * data buffer into the provided buffer and removes that data from the
     * internal buffer. The size in the header is updated accordingly.
     *
     * @param buffer Pointer to the buffer where the data will be copied.
     * @param dataSize Size of the data to read in bytes.
     *
     * @throws RuntimeErrorException if the requested data size exceeds the
     *         available data in the object.
     */
    void readAndConsume(Byte* buffer, SizeT dataSize);

    /**
     * @brief Retrieves the header of the object.
     *
     * @return A constant reference to the ObjectHeader.
     */
    const ObjectHeader& getHeader() const { return m_header; }

    /**
     * @brief Retrieves the serialized data of the object.
     *
     * @return A constant reference to the vector containing the object's data.
     */
    const Vector<Byte>& getData() const { return m_data; }

    /**
     * @brief Retrieves the serialized data of the object (non-const version).
     *
     * @return A reference to the vector containing the object's data.
     */
    Vector<Byte>& getData() { return m_data; }

  private:
    ObjectHeader m_header;
    Vector<Byte> m_data;
  };
}
