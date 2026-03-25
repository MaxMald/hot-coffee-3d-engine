#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/io/hcISerializable.h"

namespace hc
{
  /**
   * @brief A universally unique identifier (UUID) class.
   *
   * UUID provides a platform-independent wrapper around UUID generation and
   * manipulation.
   */
  class HC_UTILITY_EXPORT UUID : public ISerializable
  {
  public:
    /**
     * @brief Generates a new random UUID using the system generator.
     * 
     * @return A newly generated UUID.
     */
    static UUID Generate();

    /**
     * @brief Constructs a default UUID with all zeros.
     */
    UUID();

    /**
     * @brief Constructs a UUID from a string representation.
     *
     * The string must be in standard UUID format:
     * "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" where x is a hexadecimal
     * digit.
     *
     * @param uuidString The UUID string to parse.
     * 
     * @throws InvalidArgumentException if the string is not a valid UUID.
     */
    explicit UUID(const String& uuidString);

    /**
     * @brief Copy constructor.
     *
     * @param other The UUID to copy from.
     */
    UUID(const UUID& other);

    /**
     * @brief Copy assignment operator.
     *
     * @param other The UUID to copy from.
     * 
     * @return Reference to this UUID.
     */
    UUID& operator=(const UUID& other);

    /**
     * @brief Move constructor.
     *
     * @param other The UUID to move from.
     */
    UUID(UUID&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param other The UUID to move from.
     * 
     * @return Reference to this UUID.
     */
    UUID& operator=(UUID&& other) noexcept;

    ~UUID();

    /**
     * @brief Checks if two UUIDs are equal.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if the UUIDs are equal, false otherwise.
     */
    bool operator==(const UUID& other) const;

    /**
     * @brief Checks if two UUIDs are not equal.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if the UUIDs are not equal, false otherwise.
     */
    bool operator!=(const UUID& other) const;

    /**
     * @brief Checks if this UUID is less than another.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if this UUID is less than other, false otherwise.
     */
    bool operator<(const UUID& other) const;

    /**
     * @brief Checks if this UUID is less than or equal to another.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if this UUID is less than or equal to other.
     */
    bool operator<=(const UUID& other) const;

    /**
     * @brief Checks if this UUID is greater than another.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if this UUID is greater than other, false otherwise.
     */
    bool operator>(const UUID& other) const;

    /**
     * @brief Checks if this UUID is greater than or equal to another.
     *
     * @param other The UUID to compare with.
     * 
     * @return True if this UUID is greater than or equal to other.
     */
    bool operator>=(const UUID& other) const;

    /**
     * @brief Serializes the UUID to a binary stream.
     *
     * Writes the UUID as 16 bytes in binary format.
     *
     * @param writer The binary writer to write to.
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the UUID from a binary stream.
     *
     * Reads 16 bytes in binary format to reconstruct the UUID.
     *
     * @param reader The binary reader to read from.
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Converts the UUID to its string representation.
     *
     * @return A string in the format
     * "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
     */
    String toString() const;

    /**
     * @brief Computes a hash value for the UUID.
     *
     * @return A hash value suitable for use in hash-based containers.
     */
    SizeT hash() const;

  private:
    struct Impl;
    Impl* m_impl;

    /**
     * @brief Constructs a UUID from an implementation pointer.
     *
     * @param impl Pointer to the implementation.
     */
    explicit UUID(Impl* impl);
  };
}

namespace std
{
  /**
   * @brief Hash specialization for hc::UUID.
   *
   * Allows UUID to be used as a key in unordered containers such as
   * std::unordered_map and std::unordered_set.
   */
  template<>
  struct hash<hc::UUID>
  {
    /**
     * @brief Computes the hash value for a UUID.
     *
     * @param uuid The UUID to hash.
     *
     * @return The hash value.
     */
    std::size_t operator()(const hc::UUID& uuid) const noexcept
    {
      return uuid.hash();
    }
  };
}
