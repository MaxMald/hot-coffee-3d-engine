#pragma once

#include <istream>
#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/hcVector3.h"
#include "hc/utilities/hcVector4.h"
#include "hc/utilities/hcMatrix4.h"
#include "hc/utilities/hcColor.h"
#include "hc/utilities/hcAngle.h"
#include "hc/utilities/io/hcObjectSerialization.h"

namespace hc
{
  namespace io
  {
    /**
   * @brief Reads primitive types and engine data structures from a binary
   *        stream.
   *
   * BinaryReader provides methods for deserializing various data types from
   * a binary input stream.
   *
   * @note Peek methods require the underlying stream to be seekable. They
   *       will fail with non-seekable streams (e.g., network streams, pipes).
   */
    class HC_UTILITY_EXPORT BinaryReader
    {
    public:
      /**
       * @brief Constructs a BinaryReader with the specified input stream.
       *
       * @param stream Reference to the input stream to read from.
       */
      explicit BinaryReader(std::istream& stream);
      virtual ~BinaryReader() = default;

      /**
       * @brief Reads a boolean value.
       *
       * @return The boolean value read from the stream.
       */
      bool readBool();

      /**
       * @brief Reads an 8-bit signed integer.
       *
       * @return The integer value read from the stream.
       */
      Int8 readInt8();

      /**
       * @brief Peeks at the next 8-bit signed integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      Int8 peekInt8();

      /**
       * @brief Reads a 16-bit signed integer.
       *
       * @return The integer value read from the stream.
       */
      Int16 readInt16();

      /**
       * @brief Peeks at the next 16-bit signed integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      Int16 peekInt16();

      /**
       * @brief Reads a 32-bit signed integer.
       *
       * @return The integer value read from the stream.
       */
      Int32 readInt32();

      /**
       * @brief Reads a 64-bit signed integer.
       *
       * @return The integer value read from the stream.
       */
      Int64 readInt64();

      /**
       * @brief Reads an 8-bit unsigned integer.
       *
       * @return The integer value read from the stream.
       */
      UInt8 readUInt8();

      /**
       * @brief Peeks at the next 8-bit unsigned integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      UInt8 peekUInt8();

      /**
       * @brief Reads a 16-bit unsigned integer.
       *
       * @return The integer value read from the stream.
       */
      UInt16 readUInt16();

      /**
       * @brief Peeks at the next 16-bit unsigned integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      UInt16 peekUInt16();

      /**
       * @brief Reads a 32-bit unsigned integer.
       *
       * @return The integer value read from the stream.
       */
      UInt32 readUInt32();

      /**
       * @brief Reads a 64-bit unsigned integer.
       *
       * @return The integer value read from the stream.
       */
      UInt64 readUInt64();

      /**
       * @brief Reads a character.
       *
       * @return The character value read from the stream.
       */
      Char readChar();

      /**
       * @brief Reads a 16-bit character.
       *
       * @return The character value read from the stream.
       */
      Char16 readChar16();

      /**
       * @brief Reads a 32-bit character.
       *
       * @return The character value read from the stream.
       */
      Char32 readChar32();

      /**
       * @brief Reads an unsigned character.
       *
       * @return The character value read from the stream.
       */
      UChar readUChar();

      /**
       * @brief Reads a 32-bit floating point value.
       *
       * @return The float value read from the stream.
       */
      float readFloat();

      /**
       * @brief Reads a byte.
       *
       * @return The byte value read from the stream.
       */
      Byte readByte();

      /**
       * @brief Reads a specified number of bytes into a buffer.
       *
       * @param buffer The vector to store the read bytes.
       * @param size The number of bytes to read.
       */
      void readBytes(Vector<Byte>& buffer, SizeT size);

      /**
       * @brief Reads a size_t value.
       *
       * @return The size value read from the stream.
       */
      SizeT readSizeT();

      /**
       * @brief Reads a filesystem path from the stream.
       *
       * Reads a length-prefixed string and converts it to a Path object.
       * The path format is cross-platform compatible.
       *
       * @return The path read from the stream.
       */
      Path readPath();

      /**
       * @brief Reads a length-prefixed string from the stream.
       *
       * Reads a UInt32 length value followed by the string data.
       *
       * @return The string read from the stream.
       */
      String readString();

      /**
       * @brief Reads a 3D vector with float components.
       *
       * @return The vector read from the stream.
       */
      Vector3f readVector3f();

      /**
       * @brief Reads a 4D vector with float components.
       *
       * @return The vector read from the stream.
       */
      Vector4f readVector4f();

      /**
       * @brief Reads a 4x4 matrix with float components.
       *
       * @return The matrix read from the stream.
       */
      Matrix4 readMatrix4();

      /**
       * @brief Reads an angle value stored in radians.
       *
       * @return The angle read from the stream.
       */
      Angle readAngle();

      /**
       * @brief Reads a color with four float components (RGBA).
       *
       * @return The color read from the stream.
       */
      Color readColor();

      /**
       * @brief Starts reading an object from the stream.
       *
       * Reads the object header (name, version, size) and prepares for reading
       * the object's data.
       *
       * @return A constant reference to the ObjectHeader of the object being read.
       */
      const ObjectHeader& startReadingObject();

      /**
       * @brief Finishes reading an object from the stream.
       *
       * Completes the reading of the object's data and performs any necessary cleanup.
       */
      void finishReadingObject();

      /**
       * @brief Checks if the stream is in a valid state.
       *
       * @return True if the stream is valid and ready for reading, false
       *         otherwise.
       */
      bool isValid() const;

      /**
       * @brief Checks if there is more data available in the stream. If it is reading an
       * object, it checks if there is more data in the current object.
       *
       * @return True if there is more data to read, false otherwise.
       */
      bool hasMoreData() const;

      /**
       * @brief Checks if the reader is currently reading an object.
       *
       * @return True if reading an object, false otherwise.
       */
      bool isReadingObject() const;

    protected:
      std::istream& m_stream;
      UniquePtr<ObjectData> m_currentObject;
      Stack<UniquePtr<ObjectData>> m_objectStack;

      ObjectHeader readObjectHeader();
    };
  }
}
