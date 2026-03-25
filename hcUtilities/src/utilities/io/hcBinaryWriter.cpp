#include "hc/utilities/io/hcBinaryWriter.h"

namespace hc
{
  BinaryWriter::BinaryWriter(std::ostream& stream) :
    m_stream(stream)
  {
  }

  void BinaryWriter::writeBool(bool value)
  {
    UInt8 byteValue = value ? 1 : 0;
    m_stream.write(reinterpret_cast<const char*>(&byteValue), sizeof(UInt8));
  }

  void BinaryWriter::writeInt8(Int8 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int8));
  }

  void BinaryWriter::writeInt16(Int16 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int16));
  }

  void BinaryWriter::writeInt32(Int32 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int32));
  }

  void BinaryWriter::writeInt64(Int64 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Int64));
  }

  void BinaryWriter::writeUInt8(UInt8 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt8));
  }

  void BinaryWriter::writeUInt16(UInt16 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt16));
  }

  void BinaryWriter::writeUInt32(UInt32 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt32));
  }

  void BinaryWriter::writeUInt64(UInt64 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UInt64));
  }

  void BinaryWriter::writeChar(Char value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char));
  }

  void BinaryWriter::writeChar16(Char16 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char16));
  }

  void BinaryWriter::writeChar32(Char32 value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Char32));
  }

  void BinaryWriter::writeUChar(UChar value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(UChar));
  }

  void BinaryWriter::writeFloat(float value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(float));
  }

  void BinaryWriter::writeByte(Byte value)
  {
    m_stream.write(reinterpret_cast<const char*>(&value), sizeof(Byte));
  }

  void BinaryWriter::writeSizeT(SizeT value)
  {
    UInt64 fixedValue = static_cast<UInt64>(value);
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

  bool BinaryWriter::isValid() const
  {
    return m_stream.good();
  }
}
