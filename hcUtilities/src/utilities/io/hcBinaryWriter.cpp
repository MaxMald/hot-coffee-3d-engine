#include "hc/utilities/io/hcBinaryWriter.h"

#include <fstream>
#define UUID_SYSTEM_GENERATOR
#include <stduuid/uuid.h>

#include "hc/utilities/io/hcObjectSerialization.h"

namespace hc::io
{
  BinaryWriter::BinaryWriter() :
    m_stream(nullptr),
    m_objectStack()
  {}

  BinaryWriter::~BinaryWriter()
  {
    if (m_stream != nullptr)
      m_stream.reset();

    while (!m_objectStack.empty())
      m_objectStack.pop();
  }

  bool BinaryWriter::prepare(const Path& filePath, String& outError)
  {
    try
    {
      shutdown();

      UniquePtr<std::ofstream> fileStream = MakeUnique<std::ofstream>(filePath, std::ios::binary);
      if (!fileStream->is_open())
        throw RuntimeErrorException(
          String::Format("BinaryWriter: Failed to open file for writing: %s", filePath.toString().c_str())
        );

      m_stream = std::move(fileStream);
      assertStreamValid();
      return true;
    }
    catch (const Exception& e)
    {
      shutdown();
      outError = e.what();
      return false;
    }
  }

  void BinaryWriter::shutdown()
  {
    if (m_stream != nullptr)
    {
      m_stream->flush();

      if (m_stream->fail())
      {
        String error = "BinaryWriter: Failed to flush the stream.";
        m_stream.reset();
        throw RuntimeErrorException(error);
      }

      m_stream.reset();
    }

    while (!m_objectStack.empty())
      m_objectStack.pop();
  }

  void BinaryWriter::writeBool(bool value)
  {
    assertStreamValid();
    UInt8 byteValue = value ? 1 : 0;
    m_stream->write(reinterpret_cast<const char*>(&byteValue), sizeof(UInt8));
  }

  void BinaryWriter::writeInt8(Int8 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int8));
  }

  void BinaryWriter::writeInt16(Int16 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int16));
  }

  void BinaryWriter::writeInt32(Int32 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int32));
  }

  void BinaryWriter::writeInt64(Int64 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int64));
  }

  void BinaryWriter::writeUInt8(UInt8 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt8));
  }

  void BinaryWriter::writeUInt16(UInt16 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt16));
  }

  void BinaryWriter::writeUInt32(UInt32 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt32));
  }

  void BinaryWriter::writeUInt64(UInt64 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt64));
  }

  void BinaryWriter::writeChar(Char value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char));
  }

  void BinaryWriter::writeChar16(Char16 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char16));
  }

  void BinaryWriter::writeChar32(Char32 value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char32));
  }

  void BinaryWriter::writeUChar(UChar value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UChar));
  }

  void BinaryWriter::writeFloat(float value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(float));
  }

  void BinaryWriter::writeByte(Byte value)
  {
    assertStreamValid();
    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Byte));
  }

  void BinaryWriter::writeBytes(const Byte* data, SizeT size)
  {
    assertStreamValid();
    if (data == nullptr || size == 0)
      return;

    m_stream->write(reinterpret_cast<const char*>(data), size);
  }

  void BinaryWriter::writeSizeT(SizeT value)
  {
    assertStreamValid();
    UInt64 fixedValue = static_cast<UInt64>(value);
    writeUInt64(fixedValue);
  }

  void BinaryWriter::writeUUID(const UUID& value)
  {
    assertStreamValid();
    auto bytes = value.asBytes();
    if (bytes.size() != UUID::UUID_BYTE_SIZE)
      throw RuntimeErrorException(
        "Invalid UUID byte size: " + std::to_string(bytes.size())
      );

    writeSizeT(UUID::UUID_BYTE_SIZE);
    for (size_t i = 0; i < UUID::UUID_BYTE_SIZE; ++i)
      writeUInt8(static_cast<UInt8>(bytes[i]));
  }

  void BinaryWriter::writePath(const Path& value)
  {
    String pathString = value.toGenericString();
    writeString(pathString);
  }

  void BinaryWriter::writeString(const String& value)
  { 
    SizeT length = static_cast<SizeT>(value.length());
    writeSizeT(length);

    if (length == 0)
      return;

    assertStreamValid();
    m_stream->write(value.c_str(), length);
  }

  void BinaryWriter::writeVector3f(const Vector3f& value)
  {
    writeFloat(value.x);
    writeFloat(value.y);
    writeFloat(value.z);
  }

  void BinaryWriter::writeVector4f(const Vector4f& value)
  {
    writeFloat(value.x);
    writeFloat(value.y);
    writeFloat(value.z);
    writeFloat(value.w);
  }

  void BinaryWriter::writeMatrix4(const Matrix4& value)
  {
    for (int i = 0; i < 16; ++i)
    {
      writeFloat(value.m[i / 4][i % 4]);
    }
  }

  void BinaryWriter::writeAngle(const Angle& value)
  {
    writeFloat(value.toRadians());
  }

  void BinaryWriter::writeColor(const Color& value)
  {
    writeFloat(value.r);
    writeFloat(value.g);
    writeFloat(value.b);
    writeFloat(value.a);
  }

  void BinaryWriter::startWritingObject(UInt32 type, UInt32 version)
  {
    assertStreamValid();

    ObjectTrackingInfo trackingInfo;
    trackingInfo.startPosition = m_stream->tellp();

    ObjectHeader header;
    header.size = 0; // Placeholder, will be updated later
    header.type = type;
    header.version = version;

    writeObjectHeader(header);
    m_objectStack.push(trackingInfo);
  }

  void BinaryWriter::finishWritingObject()
  {
    assertStreamValid();

    if (m_objectStack.empty())
      throw RuntimeErrorException("No object is currently being written.");

    ObjectTrackingInfo currentObject = m_objectStack.top();
    m_objectStack.pop();

    std::streampos endPosition = m_stream->tellp();
    std::streampos size = endPosition - currentObject.startPosition;

    m_stream->seekp(currentObject.startPosition);
    writeUInt64(static_cast<UInt64>(size));

    m_stream->seekp(endPosition);
  }
  
  bool BinaryWriter::isWritingObject() const
  {
    return m_objectStack.size() > 0;
  }

  bool BinaryWriter::isValid() const
  {
    return m_stream != nullptr && m_stream->good();
  }

  void BinaryWriter::writeObjectHeader(const ObjectHeader& header)
  {
    assertStreamValid();
    writeUInt64(header.size);
    writeUInt32(header.type);
    writeUInt32(header.version);
  }
}
