#pragma once
#include <ostream>
#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/hcVector3.h"
#include "hc/utilities/hcVector4.h"
#include "hc/utilities/hcMatrix4.h"
#include "hc/utilities/hcColor.h"
#include "hc/utilities/hcAngle.h"

namespace hc
{
  /**
   * @brief Writes primitive types and engine data structures to a binary
   * stream.
   *
   * BinaryWriter provides methods for serializing various data types to a
   * binary output stream.
   */
  class HC_UTILITY_EXPORT BinaryWriter
  {
  public:
    /**
     * @brief Constructs a BinaryWriter with the specified output stream.
     *
     * @param stream Reference to the output stream to write to.
     */
    explicit BinaryWriter(std::ostream& stream);
    virtual ~BinaryWriter() = default;

    /**
     * @brief Writes a boolean value as a single byte.
     *
     * @param value The boolean to write.
     */
    void writeBool(bool value);

    /**
     * @brief Writes an 8-bit signed integer.
     *
     * @param value The integer to write.
     */
    void writeInt8(Int8 value);

    /**
     * @brief Writes a 16-bit signed integer.
     *
     * @param value The integer to write.
     */
    void writeInt16(Int16 value);

    /**
     * @brief Writes a 32-bit signed integer.
     *
     * @param value The integer to write.
     */
    void writeInt32(Int32 value);

    /**
     * @brief Writes a 64-bit signed integer.
     *
     * @param value The integer to write.
     */
    void writeInt64(Int64 value);

    /**
     * @brief Writes an 8-bit unsigned integer.
     *
     * @param value The integer to write.
     */
    void writeUInt8(UInt8 value);

    /**
     * @brief Writes a 16-bit unsigned integer.
     *
     * @param value The integer to write.
     */
    void writeUInt16(UInt16 value);

    /**
     * @brief Writes a 32-bit unsigned integer.
     *
     * @param value The integer to write.
     */
    void writeUInt32(UInt32 value);

    /**
     * @brief Writes a 64-bit unsigned integer.
     *
     * @param value The integer to write.
     */
    void writeUInt64(UInt64 value);

    /**
     * @brief Writes a character.
     *
     * @param value The character to write.
     */
    void writeChar(Char value);

    /**
     * @brief Writes a 16-bit character.
     *
     * @param value The character to write.
     */
    void writeChar16(Char16 value);

    /**
     * @brief Writes a 32-bit character.
     *
     * @param value The character to write.
     */
    void writeChar32(Char32 value);

    /**
     * @brief Writes an unsigned character.
     *
     * @param value The character to write.
     */
    void writeUChar(UChar value);

    /**
     * @brief Writes a 32-bit floating point value.
     *
     * @param value The float to write.
     */
    void writeFloat(float value);

    /**
     * @brief Writes a byte.
     *
     * @param value The byte to write.
     */
    void writeByte(Byte value);

    /**
     * @brief Writes a size_t value.
     *
     * @param value The size value to write.
     */
    void writeSizeT(SizeT value);

    /**
     * @brief Writes a filesystem path in cross-platform format.
     *
     * Converts the path to a generic string format using forward slashes
     * to ensure cross-platform compatibility. The path is written as a
     * length-prefixed string.
     *
     * @param value The path to write.
     */
    void writePath(const Path& value);

    /**
     * @brief Writes a string with a length prefix.
     *
     * Writes the string length as a UInt32 followed by the string data.
     * This allows for efficient reading without requiring null
     * terminators.
     *
     * @param value The string to write.
     */
    void writeString(const String& value);

    /**
     * @brief Writes a 3D vector with float components.
     *
     * @param value The vector to write.
     */
    void writeVector3f(const Vector3f& value);

    /**
     * @brief Writes a 4D vector with float components.
     *
     * @param value The vector to write.
     */
    void writeVector4f(const Vector4f& value);

    /**
     * @brief Writes a 4x4 matrix with float components.
     *
     * @param value The matrix to write.
     */
    void writeMatrix4(const Matrix4& value);

    /**
     * @brief Writes an angle value in radians.
     *
     * @param value The angle to write.
     */
    void writeAngle(const Angle& value);

    /**
     * @brief Writes a color with four float components (RGBA).
     *
     * @param value The color to write.
     */
    void writeColor(const Color& value);

    /**
     * @brief Checks if the stream is in a valid state.
     *
     * @return True if the stream is valid and ready for writing, false
     *         otherwise.
     */
    bool isValid() const;

  protected:
    std::ostream& m_stream;
  };
}
