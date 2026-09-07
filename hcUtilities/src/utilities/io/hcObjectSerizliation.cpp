#include "hc/utilities/io/hcObjectSerialization.h"

namespace hc::io
{
  ObjectData::ObjectData()
    : m_header(), m_data()
  {}

  ObjectData::ObjectData(const String& name, UInt32 version)
    : m_header(), m_data()
  {
    m_header.name = name;
    m_header.version = version;
    m_header.size = 0;
  }

  ObjectData::ObjectData(const ObjectHeader& header)
    : m_header(header), m_data()
  {
  }

  void ObjectData::append(const void* data, SizeT dataSize)
  {
    if (!data || dataSize == 0)
      return;

    const Byte* byteData = static_cast<const Byte*>(data);
    m_data.insert(m_data.end(), byteData, byteData + dataSize);
    m_header.size = m_data.size();
  }

  void ObjectData::peek(Byte* data, SizeT dataSize)
  {
    if (!data || dataSize == 0)
      return;

    if (dataSize > m_data.size())
      throw RuntimeErrorException("ObjectData::peek: Buffer size is larger than the object's data size.");

    std::copy(m_data.begin(), m_data.begin() + dataSize, data);
  }

  void ObjectData::readAndConsume(Byte* buffer, SizeT dataSize)
  {
    if (!buffer || dataSize == 0)
      return;

    if (dataSize > m_data.size())
      throw RuntimeErrorException("ObjectData::readAndConsume: Buffer size is larger than the object's data size.");

    std::copy(m_data.begin(), m_data.begin() + dataSize, buffer);
    m_data.erase(m_data.begin(), m_data.begin() + dataSize);
    m_header.size = m_data.size();
  }
}
