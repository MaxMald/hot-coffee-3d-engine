#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include <functional>

namespace hc
{
  /**
   * @brief A simple unique identifier class for engine objects.
   */
  class Id
  {
  public:
    /**
     * @brief Creates a new unique Id.
     * 
     * @return A new Id instance.
     */
    static Id Create()
    {
      return Id(++s_currentId);
    }

    ~Id() = default;

    bool operator==(const Id& other) const
    {
      return m_id == other.m_id;
    }

    bool operator!=(const Id& other) const
    {
      return m_id != other.m_id;
    }

    /**
     * @brief Returns the underlying value of the Id.
     * 
     * @return The 64-bit value of the Id.
     */
    UInt64 value() const
    {
      return m_id;
    }

    /**
     * @brief Returns a string representation of the Id.
     */
    String toString() const
    {
      return String::Format("%llu", m_id);
    }

  private:
    static UInt64 s_currentId;
    UInt64 m_id;

    explicit Id(UInt64 id) : m_id(id) {}
    Id() : m_id(0) {}
  };

  // Definition of the static member
  inline UInt64 Id::s_currentId = 0;
}

// Hash specialization for hc::Id to allow use in unordered containers
namespace std
{
  template<>
  struct hash<hc::Id>
  {
    hc::SizeT operator()(const hc::Id& id) const noexcept
    {
      return std::hash<hc::UInt64>{}(id.value());
    }
  };
}
