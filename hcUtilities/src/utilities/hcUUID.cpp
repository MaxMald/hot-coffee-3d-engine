#include "hc/utilities/hcUUID.h"

#define UUID_SYSTEM_GENERATOR
#include <stduuid/uuid.h>

#include "hc/utilities/io/hcBinaryWriter.h"
#include "hc/utilities/io/hcBinaryReader.h"

namespace hc
{
  static constexpr size_t UUID_BYTE_SIZE = 16;

  struct UUID::Impl
  {
    uuids::uuid uuid;

    Impl() : uuid() {}

    Impl(const String& uuidString) : uuid()
    {
      std::optional<uuids::uuid> optionalUuid = uuids::uuid::from_string(uuidString.c_str());
      if (!optionalUuid.has_value())
        throw InvalidArgumentException("Invalid UUID string: " + uuidString);

      if (uuids::to_string(optionalUuid.value()) != uuidString)
        throw InvalidArgumentException("Invalid UUID string format: " + uuidString);

      uuid = optionalUuid.value();
    }

    Impl(const uuids::uuid& u) : uuid(u) {}

    Impl(uuids::uuid&& u) : uuid(std::move(u)) {}
  };

  UUID UUID::Generate()
  {
    uuids::uuid_system_generator generator;
    uuids::uuid id = generator();

    if (id.is_nil())
      throw RuntimeErrorException("Failed to generate a valid UUID");

    return UUID(new Impl(id));
  }

  UUID::UUID() :
    m_impl(new Impl())
  {
  }

  UUID::UUID(const String& uuidString) :
    m_impl(new Impl(uuidString))
  {
  }

  UUID::UUID(const UUID& other) :
    m_impl(new Impl(other.m_impl->uuid))
  {
  }

  UUID& UUID::operator=(const UUID& other)
  {
    if (this != &other)
    {
      delete m_impl;
      m_impl = new Impl(other.m_impl->uuid);
    }
    return *this;
  }

  UUID::UUID(UUID&& other) noexcept
  {
    m_impl = other.m_impl;
    other.m_impl = nullptr;
  }

  UUID& UUID::operator=(UUID&& other) noexcept
  {
    if (this != &other)
    {
      delete m_impl;
      m_impl = other.m_impl;
      other.m_impl = nullptr;
    }
    return *this;
  }

  UUID::~UUID()
  {
    if (m_impl)
      delete m_impl;
  }

  bool UUID::operator==(const UUID& other) const
  {
    return m_impl->uuid == other.m_impl->uuid;
  }

  bool UUID::operator!=(const UUID& other) const
  {
    return m_impl->uuid != other.m_impl->uuid;
  }

  bool UUID::operator<(const UUID& other) const
  {
    return m_impl->uuid < other.m_impl->uuid;
  }

  bool UUID::operator<=(const UUID& other) const
  {
    return m_impl->uuid < other.m_impl->uuid || m_impl->uuid == other.m_impl->uuid;
  }

  bool UUID::operator>(const UUID& other) const
  {
    return !(m_impl->uuid < other.m_impl->uuid || m_impl->uuid == other.m_impl->uuid);
  }

  bool UUID::operator>=(const UUID& other) const
  {
    return !(m_impl->uuid < other.m_impl->uuid);
  }

  void UUID::serialize(BinaryWriter& writer) const
  {
    auto bytes = m_impl->uuid.as_bytes();
    if (bytes.size() != UUID_BYTE_SIZE)
      throw RuntimeErrorException(
        "Invalid UUID byte size: " + std::to_string(bytes.size())
      );

    for (size_t i = 0; i < UUID_BYTE_SIZE; ++i)
      writer.writeUInt8(static_cast<UInt8>(bytes[i]));
  }

  void UUID::deserialize(BinaryReader& reader)
  {
    std::array<uuids::uuid::value_type, UUID_BYTE_SIZE> bytes{};
    for (size_t i = 0; i < UUID_BYTE_SIZE; ++i)
      bytes[i] = reader.readUInt8();

    uuids::uuid uuid(bytes);
    m_impl->uuid = uuid;
  }

  String UUID::toString() const
  {
    return String(uuids::to_string(m_impl->uuid));
  }

  SizeT UUID::hash() const
  {
    return std::hash<uuids::uuid>{}(m_impl->uuid);
  }

  UUID::UUID(Impl* impl) :
    m_impl(impl)
  {
  }
}
