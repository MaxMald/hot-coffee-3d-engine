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
  struct ObjectHeader
  {
    String name = "";   ///< The name of the object.
    UInt32 version = 0; ///< The version of the object.
    SizeT size = 0;     ///< The size of the object in bytes.
  };

  /**
   * @brief Represents a serialized object with its header and data.
   */
  class ObjectData
  {
  public:
    ObjectData();
    ObjectData(const String& name, UInt32 version);
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
