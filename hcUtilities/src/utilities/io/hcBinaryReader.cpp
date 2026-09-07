#include "hc/utilities/io/hcBinaryReader.h"

namespace hc::io
{
  BinaryReader::BinaryReader(std::istream& stream) :
    m_stream(stream),
    m_currentObject(nullptr),
    m_objectStack()
  {
  }

  bool BinaryReader::readBool()
  {
    UInt8 byteValue = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&byteValue), sizeof(UInt8));
    else
      m_stream.read(reinterpret_cast<char*>(&byteValue), sizeof(UInt8));

    return byteValue != 0;
  }

  Int8 BinaryReader::readInt8()
  {
    Int8 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Int8));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Int8));

    return value;
  }

  Int8 BinaryReader::peekInt8()
  {
    if (m_currentObject != nullptr)
    {
      Int8 value = 0;
      m_currentObject->peek(reinterpret_cast<Byte*>(&value), sizeof(Int8));
      return value;
    }

    std::streampos originalPos = m_stream.tellg();
    Int8 value = readInt8();
    m_stream.seekg(originalPos);
    return value;
  }

  Int16 BinaryReader::readInt16()
  {
    Int16 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Int16));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Int16));

    return value;
  }

  Int16 BinaryReader::peekInt16()
  {
    if (m_currentObject != nullptr)
    {
      Int16 value = 0;
      m_currentObject->peek(reinterpret_cast<Byte*>(&value), sizeof(Int16));
      return value;
    }

    std::streampos originalPos = m_stream.tellg();
    Int16 value = readInt16();
    m_stream.seekg(originalPos);
    return value;
  }

  Int32 BinaryReader::readInt32()
  {
    Int32 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Int32));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Int32));

    return value;
  }

  Int64 BinaryReader::readInt64()
  {
    Int64 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Int64));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Int64));

    return value;
  }

  UInt8 BinaryReader::readUInt8()
  {
    UInt8 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(UInt8));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(UInt8));

    return value;
  }

  UInt8 BinaryReader::peekUInt8()
  {
    if (m_currentObject != nullptr)
    {
      UInt8 value = 0;
      m_currentObject->peek(reinterpret_cast<Byte*>(&value), sizeof(UInt8));
      return value;
    }

    std::streampos originalPos = m_stream.tellg();
    UInt8 value = readUInt8();
    m_stream.seekg(originalPos);
    return value;
  }

  UInt16 BinaryReader::readUInt16()
  {
    UInt16 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(UInt16));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(UInt16));

    return value;
  }

  UInt16 BinaryReader::peekUInt16()
  {
    if (m_currentObject != nullptr)
    {
      UInt16 value = 0;
      m_currentObject->peek(reinterpret_cast<Byte*>(&value), sizeof(UInt16));
      return value;
    }

    std::streampos originalPos = m_stream.tellg();
    UInt16 value = readUInt16();
    m_stream.seekg(originalPos);
    return value;
  }

  UInt32 BinaryReader::readUInt32()
  {
    UInt32 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(UInt32));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(UInt32));

    return value;
  }

  UInt64 BinaryReader::readUInt64()
  {
    UInt64 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(UInt64));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(UInt64));

    return value;
  }

  Char BinaryReader::readChar()
  {
    Char value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Char));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Char));

    return value;
  }

  Char16 BinaryReader::readChar16()
  {
    Char16 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Char16));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Char16));

    return value;
  }

  Char32 BinaryReader::readChar32()
  {
    Char32 value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(Char32));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Char32));

    return value;
  }

  UChar BinaryReader::readUChar()
  {
    UChar value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(UChar));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(UChar));

    return value;
  }

  float BinaryReader::readFloat()
  {
    float value = 0.0f;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value), sizeof(float));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(float));

    return value;
  }

  Byte BinaryReader::readByte()
  {
    Byte value = 0;

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(&value, sizeof(Byte));
    else
      m_stream.read(reinterpret_cast<char*>(&value), sizeof(Byte));

    return value;
  }

  void BinaryReader::readBytes(Vector<Byte>& buffer, SizeT size)
  {
    if (size == 0)
      return;

    buffer.resize(size);
    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(buffer.data(), size);
    else
      m_stream.read(reinterpret_cast<char*>(buffer.data()), size);
  }

  SizeT BinaryReader::readSizeT()
  {
    UInt64 fixedValue = readUInt64();
    return static_cast<SizeT>(fixedValue);
  }

  Path BinaryReader::readPath()
  {
    String pathString = readString();
    return Path(pathString.c_str());
  }

  String BinaryReader::readString()
  {
    SizeT length = readSizeT();
    if (length == 0)
      return String();

    String value;
    value.resize(length);

    if (m_currentObject != nullptr)
      m_currentObject->readAndConsume(reinterpret_cast<Byte*>(&value[0]), length);
    else
      m_stream.read(&value[0], length);

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

  Vector4f BinaryReader::readVector4f()
  {
    Vector4f value;
    value.x = readFloat();
    value.y = readFloat();
    value.z = readFloat();
    value.w = readFloat();
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

  Angle BinaryReader::readAngle()
  {
    float radians = readFloat();
    return Angle::FromRadians(radians);
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

  const ObjectHeader& BinaryReader::startReadingObject()
  {
    ObjectHeader header = readObjectHeader();
    UniquePtr<ObjectData> newObject = MakeUnique<ObjectData>(header);
    readBytes(newObject->getData(), header.size);

    if (m_currentObject != nullptr)
    {
      m_objectStack.push(std::move(m_currentObject));
      m_currentObject.reset();
    }

    m_currentObject = std::move(newObject);
    return m_currentObject->getHeader();
  }

  void BinaryReader::finishReadingObject()
  {
    if (m_currentObject == nullptr)
      throw RuntimeErrorException("No object is currently being read.");

    m_currentObject.reset();
    if (!m_objectStack.empty())
    { 
      m_currentObject = std::move(m_objectStack.top());
      m_objectStack.pop();
    }
  }

  bool BinaryReader::isValid() const
  {
    return m_stream.good();
  }

  bool BinaryReader::hasMoreData() const
  {
    if (m_currentObject != nullptr)
      return m_currentObject->getData().size() > 0;
    return m_stream.peek() != EOF;
  }

  bool BinaryReader::isReadingObject() const
  {
    return m_currentObject != nullptr;
  }

  ObjectHeader BinaryReader::readObjectHeader()
  {
    ObjectHeader header;
    header.name = readString();
    header.version = readUInt32();
    header.size = readSizeT();
    return header;
  }
}
