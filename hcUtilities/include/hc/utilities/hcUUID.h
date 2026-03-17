#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/io/hcISerializable.h"

namespace hc
{
  class HC_UTILITY_EXPORT UUID : public ISerializable
  {
  public:
    static UUID Generate();

    UUID();
    explicit UUID(const String& uuidString);
    UUID(const UUID& other);
    UUID& operator=(const UUID& other);
    UUID(UUID&& other) noexcept;
    UUID& operator=(UUID&& other) noexcept;
    ~UUID();

    bool operator==(const UUID& other) const;
    bool operator!=(const UUID& other) const;
    bool operator<(const UUID& other) const;
    bool operator<=(const UUID& other) const;
    bool operator>(const UUID& other) const;
    bool operator>=(const UUID& other) const;

    void serialize(BinaryWriter& writer) const override;
    void deserialize(BinaryReader& reader) override;
    String toString() const;

  private:
    struct Impl;
    Impl* m_impl;

    /**
     * @brief Constructs a Json object from an implementation pointer.
     * @param impl Pointer to the implementation.
     */
    explicit UUID(Impl* impl);
  };
}
