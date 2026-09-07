#include "hc/utilities/io/hcBinaryWriter.h"
#include "hc/utilities/io/hcObjectSerialization.h"

namespace hc::io
{
  BinaryWriter::BinaryWriter(std::ostream& stream) :
    m_stream(stream),
    m_currentObject(nullptr),
    m_objectStack()
  {
  }

  void BinaryWriter::writeBool(bool value)
  {
    UInt8 byteValue = value ? 1 : 0;

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&byteValue, sizeof(UInt8));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&byteValue), sizeof(UInt8));
  }

  void BinaryWriter::writeInt8(Int8 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int8));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int8));
  }

  void BinaryWriter::writeInt16(Int16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int16));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int16));
  }

  void BinaryWriter::writeInt32(Int32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int32));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int32));
  }

  void BinaryWriter::writeInt64(Int64 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int64));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int64));
  }

  void BinaryWriter::writeUInt8(UInt8 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt8));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt8));
  }

  void BinaryWriter::writeUInt16(UInt16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt16));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt16));
  }

  void BinaryWriter::writeUInt32(UInt32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt32));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt32));
  }

  void BinaryWriter::writeUInt64(UInt64 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt64));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt64));
  }

  void BinaryWriter::writeChar(Char value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char));
  }

  void BinaryWriter::writeChar16(Char16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char16));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char16));
  }

  void BinaryWriter::writeChar32(Char32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char32));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char32));
  }

  void BinaryWriter::writeUChar(UChar value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UChar));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UChar));
  }

  void BinaryWriter::writeFloat(float value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(float));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(float));
  }

  void BinaryWriter::writeByte(Byte value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Byte));
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Byte));
  }

  void BinaryWriter::writeBytes(const Byte* data, SizeT size)
  {
    if (data == nullptr || size == 0)
      return;

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(const_cast<Byte*>(data), size);
      return;
    }

    m_stream.write(reinterpret_cast<const char*>(data), size);
  }

  void BinaryWriter::writeSizeT(SizeT value)
  {
    UInt64 fixedValue = static_cast<UInt64>(value);

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&fixedValue, sizeof(UInt64));
      return;
    }

    writeUInt64(fixedValue);
  }

  void BinaryWriter::writePath(const Path& value)
  {
    String pathString = value.generic_string();
    writeString(pathString);
  }

  void BinaryWriter::writeString(const String& value)
  {
    SizeT length = static_cast<SizeT>(value.length());
    writeSizeT(length);

    if (length == 0)
      return;

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(reinterpret_cast<const void*>(value.c_str()), length);
      return;
    }

    m_stream.write(value.c_str(), length);
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

  void BinaryWriter::startWritingObject(const String& name, UInt32 version)
  {
    if (m_currentObject != nullptr)
    {
      m_objectStack.push(std::move(m_currentObject));
      m_currentObject = MakeUnique<ObjectData>(name, version);
    }
    else
    {
      m_currentObject = MakeUnique<ObjectData>(name, version);
    }
  }

  void BinaryWriter::finishWritingObject()
  {
    if (m_currentObject == nullptr)
      throw RuntimeErrorException("No object is currently being written.");

    UniquePtr<ObjectData> finishedObject = std::move(m_currentObject);
    m_currentObject.reset();

    if (!m_objectStack.empty())
    {
      m_currentObject = std::move(m_objectStack.top());
      m_objectStack.pop();
    }

    writeObject(*finishedObject);
    finishedObject.reset();
  }
  
  bool BinaryWriter::isWritingObject() const
  {
    return m_currentObject != nullptr;
  }

  bool BinaryWriter::isValid() const
  {
    return m_stream.good();
  }

  void BinaryWriter::writeObject(const ObjectData& objectData)
  {
    const ObjectHeader& header = objectData.getHeader();
    writeObjectHeader(header);

    const Vector<Byte>& data = objectData.getData();
    if (data.empty())
      return;

    writeBytes(data.data(), data.size());
  }

  void BinaryWriter::writeObjectHeader(const ObjectHeader& header)
  {
    writeString(header.name);
    writeUInt32(header.version);
    writeSizeT(header.size);
  }
}
