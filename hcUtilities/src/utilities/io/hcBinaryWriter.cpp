#include "hc/utilities/io/hcBinaryWriter.h"

#include <fstream>
#define UUID_SYSTEM_GENERATOR
#include <stduuid/uuid.h>

#include "hc/utilities/io/hcObjectSerialization.h"

namespace hc::io
{
  BinaryWriter::BinaryWriter() :
    m_stream(nullptr),
    m_currentObject(nullptr),
    m_objectStack()
  {}

  BinaryWriter::~BinaryWriter()
  {
    shutdown();
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
      m_stream.reset();
    }

    m_currentObject.reset();
    while (!m_objectStack.empty())
    {
      m_objectStack.pop();
    }
  }

  void BinaryWriter::writeBool(bool value)
  {
    UInt8 byteValue = value ? 1 : 0;

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&byteValue, sizeof(UInt8));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&byteValue), sizeof(UInt8));
  }

  void BinaryWriter::writeInt8(Int8 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int8));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int8));
  }

  void BinaryWriter::writeInt16(Int16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int16));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int16));
  }

  void BinaryWriter::writeInt32(Int32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int32));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int32));
  }

  void BinaryWriter::writeInt64(Int64 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Int64));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Int64));
  }

  void BinaryWriter::writeUInt8(UInt8 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt8));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt8));
  }

  void BinaryWriter::writeUInt16(UInt16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt16));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt16));
  }

  void BinaryWriter::writeUInt32(UInt32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt32));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt32));
  }

  void BinaryWriter::writeUInt64(UInt64 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UInt64));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UInt64));
  }

  void BinaryWriter::writeChar(Char value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char));
  }

  void BinaryWriter::writeChar16(Char16 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char16));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char16));
  }

  void BinaryWriter::writeChar32(Char32 value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Char32));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Char32));
  }

  void BinaryWriter::writeUChar(UChar value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(UChar));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(UChar));
  }

  void BinaryWriter::writeFloat(float value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(float));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(float));
  }

  void BinaryWriter::writeByte(Byte value)
  {
    if (m_currentObject != nullptr)
    {
      m_currentObject->append(&value, sizeof(Byte));
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(&value), sizeof(Byte));
  }

  void BinaryWriter::writeBytes(const Byte* data, SizeT size)
  {
    if (data == nullptr || size == 0)
      return;

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(reinterpret_cast<const Byte*>(data), size);
      return;
    }

    m_stream->write(reinterpret_cast<const char*>(data), size);
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

  void BinaryWriter::writeUUID(const UUID& value)
  {
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

    if (m_currentObject != nullptr)
    {
      m_currentObject->append(reinterpret_cast<const void*>(value.c_str()), length);
      return;
    }

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
    if (m_currentObject != nullptr)
    {
      m_objectStack.push(std::move(m_currentObject));
      m_currentObject = MakeUnique<ObjectData>(type, version);
    }
    else
    {
      m_currentObject = MakeUnique<ObjectData>(type, version);
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
    return m_stream != nullptr && m_stream->good();
  }

  void BinaryWriter::writeObject(const ObjectData& objectData)
  {
    const ObjectHeader& header = objectData.getHeader();
    writeBytes(reinterpret_cast<const Byte*>(&header), sizeof(ObjectHeader));

    const Vector<Byte>& data = objectData.getData();
    if (data.empty())
      return;

    writeBytes(data.data(), data.size());
  }
}
