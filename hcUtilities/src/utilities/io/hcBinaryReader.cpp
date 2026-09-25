#include "hc/utilities/io/hcBinaryReader.h"
#include <fstream>

#define UUID_SYSTEM_GENERATOR
#include <stduuid/uuid.h>

namespace hc::io
{
  BinaryReader::BinaryReader() :
    m_stream(nullptr),
    m_objectStack()
  {
  }

  BinaryReader::~BinaryReader()
  {
    shutdown();
  }

  bool BinaryReader::prepare(const Path& filePath, String& outError)
  {
    try
    {
      shutdown();

      UniquePtr<std::ifstream> fileStream = MakeUnique<std::ifstream>(filePath, std::ios::binary);
      if (!fileStream->is_open())
        throw RuntimeErrorException(
          String::Format("BinaryReader: Failed to open file for reading: %s", filePath.toString().c_str())
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

  void BinaryReader::shutdown()
  {
    if (m_stream)
    {
      m_stream->clear();
      m_stream->seekg(0, std::ios::beg);
      m_stream.reset();
    }

    while (!m_objectStack.empty())
      m_objectStack.pop();
  }

  bool BinaryReader::readBool()
  {
    UInt8 byteValue = 0;
    readStream(reinterpret_cast<char*>(&byteValue), sizeof(UInt8));
    return byteValue != 0;
  }

  bool BinaryReader::peekBool()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    bool value = readBool();
    m_stream->seekg(originalPos);
    return value;
  }

  Int8 BinaryReader::readInt8()
  {
    Int8 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Int8));
    return value;
  }

  Int8 BinaryReader::peekInt8()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Int8 value = readInt8();
    m_stream->seekg(originalPos);
    return value;
  }

  Int16 BinaryReader::readInt16()
  {
    Int16 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Int16));
    return value;
  }

  Int16 BinaryReader::peekInt16()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Int16 value = readInt16();
    m_stream->seekg(originalPos);
    return value;
  }

  Int32 BinaryReader::readInt32()
  {
    Int32 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Int32));
    return value;
  }

  Int32 BinaryReader::peekInt32()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Int32 value = readInt32();
    m_stream->seekg(originalPos);
    return value;
  }

  Int64 BinaryReader::readInt64()
  {
    Int64 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Int64));
    return value;
  }

  Int64 BinaryReader::peekInt64()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Int64 value = readInt64();
    m_stream->seekg(originalPos);
    return value;
  }

  UInt8 BinaryReader::readUInt8()
  {
    UInt8 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(UInt8));
    return value;
  }

  UInt8 BinaryReader::peekUInt8()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UInt8 value = readUInt8();
    m_stream->seekg(originalPos);
    return value;
  }

  UInt16 BinaryReader::readUInt16()
  {
    UInt16 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(UInt16));
    return value;
  }

  UInt16 BinaryReader::peekUInt16()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UInt16 value = readUInt16();
    m_stream->seekg(originalPos);
    return value;
  }

  UInt32 BinaryReader::readUInt32()
  {
    UInt32 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(UInt32));
    return value;
  }

  UInt32 BinaryReader::peekUInt32()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UInt32 value = readUInt32();
    m_stream->seekg(originalPos);
    return value;
  }

  UInt64 BinaryReader::readUInt64()
  {
    UInt64 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(UInt64));
    return value;
  }

  UInt64 BinaryReader::peekUInt64()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UInt64 value = readUInt64();
    m_stream->seekg(originalPos);
    return value;
  }

  Char BinaryReader::readChar()
  {
    Char value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Char));
    return value;
  }

  Char BinaryReader::peekChar()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Char value = readChar();
    m_stream->seekg(originalPos);
    return value;
  }

  Char16 BinaryReader::readChar16()
  {
    Char16 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Char16));
    return value;
  }

  Char16 BinaryReader::peekChar16()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Char16 value = readChar16();
    m_stream->seekg(originalPos);
    return value;
  }

  Char32 BinaryReader::readChar32()
  {
    Char32 value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(Char32));
    return value;
  }

  Char32 BinaryReader::peekChar32()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Char32 value = readChar32();
    m_stream->seekg(originalPos);
    return value;
  }

  UChar BinaryReader::readUChar()
  {
    UChar value = 0;
    readStream(reinterpret_cast<char*>(&value), sizeof(UChar));
    return value;
  }

  UChar BinaryReader::peekUChar()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UChar value = readUChar();
    m_stream->seekg(originalPos);
    return value;
  }

  float BinaryReader::readFloat()
  {
    float value = 0.0f;
    readStream(reinterpret_cast<char*>(&value), sizeof(float));
    return value;
  }

  float BinaryReader::peekFloat()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    float value = readFloat();
    m_stream->seekg(originalPos);
    return value;
  }

  Byte BinaryReader::readByte()
  {
    Byte value = static_cast<Byte>(0);
    readStream(reinterpret_cast<char*>(&value), sizeof(Byte));
    return value;
  }

  Byte BinaryReader::peekByte()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Byte value = readByte();
    m_stream->seekg(originalPos);
    return value;
  }

  void BinaryReader::readBytes(Vector<Byte>& buffer, SizeT size)
  {
    if (size == 0)
      return;

    assertStreamValid();
    buffer.resize(size);
    readStream(reinterpret_cast<char*>(buffer.data()), size);
  }

  void BinaryReader::readBytes(Byte* buffer, SizeT size)
  {
    if (size == 0)
      return;

    if (buffer == nullptr)
      throw RuntimeErrorException("BinaryReader: Cannot read bytes into the given buffer. Buffer pointer is null.");

    readStream(reinterpret_cast<char*>(buffer), size);
  }

  SizeT BinaryReader::readSizeT()
  {
    assertStreamValid();
    UInt64 fixedValue = readUInt64();
    return static_cast<SizeT>(fixedValue);
  }

  SizeT BinaryReader::peekSizeT()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    SizeT value = readSizeT();
    m_stream->seekg(originalPos);
    return value;
  }

  UUID BinaryReader::readUUID()
  {
    assertStreamValid();
    SizeT byteSize = readSizeT();
    if (byteSize != UUID::UUID_BYTE_SIZE)
      throw RuntimeErrorException(
        String::Format("BinaryReader: Invalid UUID size read from stream. Expected %zu bytes, but got %zu bytes.",
          UUID::UUID_BYTE_SIZE,
          byteSize
        )
      );

    Array<UInt8, UUID::UUID_BYTE_SIZE> bytes{};
    for (SizeT i = 0; i < UUID::UUID_BYTE_SIZE; ++i)
      bytes[i] = readUInt8();

    return UUID(bytes);
  }

  UUID BinaryReader::peekUUID()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    UUID value = readUUID();
    m_stream->seekg(originalPos);
    return value;
  }

  Path BinaryReader::readPath()
  {
    assertStreamValid();
    String pathString = readString();
    return Path(pathString.c_str());
  }

  Path BinaryReader::peekPath()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Path value = readPath();
    m_stream->seekg(originalPos);
    return value;
  }

  String BinaryReader::readString()
  {
    SizeT length = readSizeT();
    if (length == 0)
      return String();

    String value;
    value.resize(length);

    readStream(reinterpret_cast<char*>(&value[0]), length);
    return value;
  }

  String BinaryReader::peekString()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    String value = readString();
    m_stream->seekg(originalPos);
    return value;
  }

  Vector3f BinaryReader::readVector3f()
  {
    Vector3f value;
    value.x = readFloat();
    value.y = readFloat();
    value.z = readFloat();
    return value;
  }

  Vector3f BinaryReader::peekVector3f()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Vector3f value = readVector3f();
    m_stream->seekg(originalPos);
    return value;
  }

  Vector4f BinaryReader::readVector4f()
  {
    Vector4f value;
    value.x = readFloat();
    value.y = readFloat();
    value.z = readFloat();
    value.w = readFloat();
    return value;
  }

  Vector4f BinaryReader::peekVector4f()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Vector4f value = readVector4f();
    m_stream->seekg(originalPos);
    return value;
  }

  Matrix4 BinaryReader::readMatrix4()
  {
    Matrix4 value;
    for (int i = 0; i < 16; ++i)
    {
      value.m[i / 4][i % 4] = readFloat();
    }
    return value;
  }

  Matrix4 BinaryReader::peekMatrix4()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Matrix4 value = readMatrix4();
    m_stream->seekg(originalPos);
    return value;
  }

  Angle BinaryReader::readAngle()
  {
    float radians = readFloat();
    return Angle::FromRadians(radians);
  }

  Angle BinaryReader::peekAngle()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Angle value = readAngle();
    m_stream->seekg(originalPos);
    return value;
  }

  Color BinaryReader::readColor()
  {
    Color value;
    value.r = readFloat();
    value.g = readFloat();
    value.b = readFloat();
    value.a = readFloat();
    return value;
  }

  Color BinaryReader::peekColor()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    Color value = readColor();
    m_stream->seekg(originalPos);
    return value;
  }

  ObjectHeader BinaryReader::peekObjectHeader()
  {
    assertStreamValid();
    std::streampos originalPos = m_stream->tellg();
    ObjectHeader header = readObjectHeader();
    m_stream->seekg(originalPos);
    return header;
  }

  ObjectHeader BinaryReader::startReadingObject()
  {
    assertStreamValid();
    std::streampos startPos = m_stream->tellg();
    ObjectHeader header = readObjectHeader();

    std::streampos endPos = startPos + static_cast<std::streampos>(header.size);
    ObjectTrackingInfo trackingInfo;
    trackingInfo.startPosition = startPos;
    trackingInfo.endPosition = endPos;
    m_objectStack.push(trackingInfo);

    return header;
  }

  void BinaryReader::finishReadingObject()
  {
    if (m_objectStack.empty())
      throw RuntimeErrorException("No object is currently being read.");

    ObjectTrackingInfo currentObject = m_objectStack.top();
    m_objectStack.pop();

    std::streampos currentPos = m_stream->tellg();
    if (currentPos < currentObject.endPosition)
      m_stream->seekg(currentObject.endPosition);
  }

  bool BinaryReader::isValid() const
  {
    return m_stream != nullptr && m_stream->good();
  }
  bool BinaryReader::isEndOfStream() const
  {
    if (m_stream == nullptr)
      throw RuntimeErrorException("BinaryReader: Stream is not valid for reading.");

    return m_stream->peek() == EOF;
  }

  bool BinaryReader::isEndOfObject() const
  {
    assertStreamValid();
    if (m_objectStack.empty())
      throw RuntimeErrorException("No object is currently being read.");

    std::streampos currentPos = m_stream->tellg();
    return currentPos >= m_objectStack.top().endPosition;
  }

  bool BinaryReader::isReadingObject() const
  {
    assertStreamValid();
    return !m_objectStack.empty();
  }

  ObjectHeader BinaryReader::readObjectHeader()
  {
    ObjectHeader header;
    header.size = readUInt64();
    header.type = readUInt32();
    header.version = readUInt32();
    return header;
  }
}
