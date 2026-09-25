#pragma once

#include <istream>
#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/hcPath.h"
#include "hc/utilities/hcVector3.h"
#include "hc/utilities/hcVector4.h"
#include "hc/utilities/hcMatrix4.h"
#include "hc/utilities/hcColor.h"
#include "hc/utilities/hcAngle.h"
#include "hc/utilities/io/hcObjectSerialization.h"
#include "hc/utilities/hcUUID.h"

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
     *
     * @note Serialization format uses fixed-width types (UInt64, UInt32, etc.)
     *       rather than platform-dependent types (SizeT). This ensures
     *       cross-platform compatibility within the same endianness.
     *
     * @note The current implementation assumes little-endian byte order.
     *       Files written on little-endian systems may not deserialize
     *       correctly on big-endian systems without additional endianness
     *       conversion. Future enhancements should consider endianness
     *       handling if cross-architecture support is needed.
     */
    class HC_UTILITY_EXPORT BinaryReader
    {
    public:
      explicit BinaryReader();
      virtual ~BinaryReader();

      /**
       * @brief Prepares the binary reader by opening a binary file for reading.
       *
       * @param filePath The path to the binary file to open.
       * @param outError A string to receive an error message if preparation fails.
       *
       * @return True if the file was opened successfully, false otherwise.
       */
      bool prepare(const Path& filePath, String& outError);

      /**
       * @brief Safely shuts down the binary reader, closing any open streams and
       * releasing resources.
       */
      void shutdown();

      /**
       * @brief Reads a boolean value.
       *
       * @return The boolean value read from the stream.
       */
      bool readBool();

      /**
       * @brief Peeks at the next boolean value without advancing the stream
       * position.
       *
       * @return The boolean value peeked from the stream.
       */
      bool peekBool();

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
       * @brief Peeks at the next 32-bit signed integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      Int32 peekInt32();

      /**
       * @brief Reads a 64-bit signed integer.
       *
       * @return The integer value read from the stream.
       */
      Int64 readInt64();

      /**
       * @brief Peeks at the next 64-bit signed integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      Int64 peekInt64();

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
       * @brief Peeks at the next 32-bit unsigned integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      UInt32 peekUInt32();

      /**
       * @brief Reads a 64-bit unsigned integer.
       *
       * @return The integer value read from the stream.
       */
      UInt64 readUInt64();

      /**
       * @brief Peeks at the next 64-bit unsigned integer without advancing the
       * stream position.
       *
       * @return The integer value peeked from the stream.
       */
      UInt64 peekUInt64();

      /**
       * @brief Reads a character.
       *
       * @return The character value read from the stream.
       */
      Char readChar();

      /**
       * @brief Peeks at the next character without advancing the stream
       * position.
       *
       * @return The character value peeked from the stream.
       */
      Char peekChar();

      /**
       * @brief Reads a 16-bit character.
       *
       * @return The character value read from the stream.
       */
      Char16 readChar16();

      /**
       * @brief Peeks at the next 16-bit character without advancing the stream
       * position.
       *
       * @return The character value peeked from the stream.
       */
      Char16 peekChar16();

      /**
       * @brief Reads a 32-bit character.
       *
       * @return The character value read from the stream.
       */
      Char32 readChar32();

      /**
       * @brief Peeks at the next 32-bit character without advancing the stream
       * position.
       *
       * @return The character value peeked from the stream.
       */
      Char32 peekChar32();

      /**
       * @brief Reads an unsigned character.
       *
       * @return The character value read from the stream.
       */
      UChar readUChar();

      /**
       * @brief Peeks at the next unsigned character without advancing the stream
       * position.
       *
       * @return The character value peeked from the stream.
       */
      UChar peekUChar();

      /**
       * @brief Reads a 32-bit floating point value.
       *
       * @return The float value read from the stream.
       */
      float readFloat();

      /**
       * @brief Peeks at the next 32-bit floating point value without advancing
       * the stream position.
       *
       * @return The float value peeked from the stream.
       */
      float peekFloat();

      /**
       * @brief Reads a byte.
       *
       * @return The byte value read from the stream.
       */
      Byte readByte();

      /**
       * @brief Peeks at the next byte without advancing the stream position.
       *
       * @return The byte value peeked from the stream.
       */
      Byte peekByte();

      /**
       * @brief Reads a specified number of bytes into a buffer.
       *
       * @param buffer The vector to store the read bytes.
       * @param size The number of bytes to read.
       */
      void readBytes(Vector<Byte>& buffer, SizeT size);

      /**
       * @brief Reads a specified number of bytes into a buffer.
       *
       * @param buffer The pointer to the buffer to store the read bytes.
       * @param size The number of bytes to read.
       */
      void readBytes(Byte* buffer, SizeT size);

      /**
       * @brief Reads a size_t value.
       *
       * @return The size value read from the stream.
       */
      SizeT readSizeT();

      /**
       * @brief Peeks at the next size_t value without advancing the stream
       * position.
       *
       * @return The size value peeked from the stream.
       */
      SizeT peekSizeT();

      /**
       * @brief Reads a UUID from the stream.
       *
       * Reads a 16-byte UUID and constructs a UUID object.
       *
       * @return The UUID read from the stream.
       */
      UUID readUUID();

      /**
       * @brief Peeks at the next UUID without advancing the stream position.
       *
       * Reads a 16-byte UUID and constructs a UUID object, but does not consume
       * the bytes from the stream.
       *
       * @return The UUID peeked from the stream.
       */
      UUID peekUUID();

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
       * @brief Peeks at the next filesystem path without advancing the stream
       * position.
       *
       * Reads a length-prefixed string and converts it to a Path object, but
       * does not consume the bytes from the stream.
       *
       * @return The path peeked from the stream.
       */
      Path peekPath();

      /**
       * @brief Reads a length-prefixed string from the stream.
       *
       * Reads a UInt32 length value followed by the string data.
       *
       * @return The string read from the stream.
       */
      String readString();

      /**
       * @brief Peeks at the next length-prefixed string without advancing the
       * stream position.
       *
       * Reads a UInt32 length value followed by the string data, but does not
       * consume the bytes from the stream.
       *
       * @return The string peeked from the stream.
       */
      String peekString();

      /**
       * @brief Reads a 3D vector with float components.
       *
       * @return The vector read from the stream.
       */
      Vector3f readVector3f();

      /**
       * @brief Peeks at the next 3D vector with float components without
       * advancing the stream position.
       *
       * @return The vector peeked from the stream.
       */
      Vector3f peekVector3f();

      /**
       * @brief Reads a 4D vector with float components.
       *
       * @return The vector read from the stream.
       */
      Vector4f readVector4f();

      /**
       * @brief Peeks at the next 4D vector with float components without
       * advancing the stream position.
       *
       * @return The vector peeked from the stream.
       */
      Vector4f peekVector4f();

      /**
       * @brief Reads a 4x4 matrix with float components.
       *
       * @return The matrix read from the stream.
       */
      Matrix4 readMatrix4();

      /**
       * @brief Peeks at the next 4x4 matrix with float components without
       * advancing the stream position.
       *
       * @return The matrix peeked from the stream.
       */
      Matrix4 peekMatrix4();

      /**
       * @brief Reads an angle value stored in radians.
       *
       * @return The angle read from the stream.
       */
      Angle readAngle();

      /**
       * @brief Peeks at the next angle value stored in radians without advancing
       * the stream position.
       *
       * @return The angle peeked from the stream.
       */
      Angle peekAngle();

      /**
       * @brief Reads a color with four float components (RGBA).
       *
       * @return The color read from the stream.
       */
      Color readColor();

      /**
       * @brief Peeks at the next color with four float components (RGBA) without
       * advancing the stream position.
       *
       * @return The color peeked from the stream.
       */
      Color peekColor();

      /**
       * @brief Peeks at the next object header without advancing the stream position.
       *
       * This method reads the object header from the stream but does not consume it,
       * allowing for inspection of the next object without affecting the stream state.
       *
       * @return The ObjectHeader of the next object in the stream.
       */
      ObjectHeader peekObjectHeader();

      /**
       * @brief Starts reading an object from the stream.
       *
       * Reads the object header (name, version, size) and prepares for reading
       * the object's data.
       *
       * @return The ObjectHeader of the object being read.
       */
      ObjectHeader startReadingObject();

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
       * @brief Checks if the end of the stream has been reached.
       *
       * @return True if the end of the stream or object has been reached, false otherwise.
       */
      bool isEndOfStream() const;

      /**
       * @brief Checks if the end of the current object has been reached.
       *
       * @return True if the end of the current object has been reached, false otherwise.
       */
      bool isEndOfObject() const;

      /**
       * @brief Checks if the reader is currently reading an object.
       *
       * @return True if reading an object, false otherwise.
       */
      bool isReadingObject() const;

    protected:
      UniquePtr<std::istream> m_stream;
      Stack<ObjectTrackingInfo> m_objectStack;

      ObjectHeader readObjectHeader();

      /**
       * @brief Asserts that the stream is valid for reading.
       *
       * Throws a RuntimeErrorException if the stream is null or not in a good state.
       */
      inline void assertStreamValid() const
      {
        if (m_stream == nullptr || !m_stream->good())
          throw RuntimeErrorException("BinaryReader: Stream is not valid for reading.");
      }

      /**
       * @brief Reads a specified number of bytes from the stream into a buffer.
       *
       * This method checks if the read operation would exceed the bounds of the
       * current object being read (if any). If it does, an exception is thrown.
       *
       * @param buffer The buffer to read data into.
       * @param size The number of bytes to read.
       *
       * @throws RuntimeErrorException if the read operation exceeds the current
       *         object's bounds or if the stream is not valid.
       */
      inline void readStream(char* buffer, SizeT size)
      {
        assertStreamValid();

        if (m_objectStack.size() > 0)
        {
          std::streampos currentPos = m_stream->tellg();
          std::streampos endPos = m_objectStack.top().endPosition;
          if (currentPos + static_cast<std::streampos>(size) > endPos)
            throw RuntimeErrorException("BinaryReader: Attempt to read beyond the end of the current object.");
        }

        m_stream->read(buffer, static_cast<std::streamsize>(size));
      }
    };
  }
}
