#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"

namespace hc
{
  /**
   * @brief Represents a JSON value and provides methods for parsing, accessing,
   * and manipulating JSON data.
   *
   * The Json class supports parsing JSON strings, loading JSON from files, and
   * accessing JSON objects, arrays, and values. It provides type-checking
   * methods and value retrieval for supported types.
   */
  class HC_UTILITY_EXPORT Json
  {
  public:
    /**
     * @brief Default constructor. Constructs a null JSON value.
     */
    Json();

    /**
     * @brief Copy constructor.
     * @param other The Json object to copy from.
     */
    Json(const Json& other);

    /**
     * @brief Move constructor.
     * @param other The Json object to move from.
     */
    Json(Json&& other) noexcept;

    /**
     * @brief Destructor.
     */
    ~Json();

    /**
     * @brief Parses a JSON string and returns a Json object.
     * @param jsonString The JSON string to parse.
     * @return Parsed Json object.
     * @throws std::runtime_error if parsing fails.
     */
    static Json parse(const String& jsonString);

    /**
     * @brief Loads and parses a JSON file.
     * @param filePath The path to the JSON file.
     * @return Parsed Json object.
     * @throws std::runtime_error if loading or parsing fails.
     */
    static Json loadFromFile(const Path& filePath);

    /**
     * @brief Copy assignment operator.
     * @param other The Json object to copy from.
     * @return Reference to this object.
     */
    Json& operator=(const Json& other);

    /**
     * @brief Move assignment operator.
     * @param other The Json object to move from.
     * @return Reference to this object.
     */
    Json& operator=(Json&& other) noexcept;

    /**
     * @brief Accesses a value by key in a JSON object.
     * @param key The key to access.
     * @return The Json value associated with the key.
     * @throws std::out_of_range if the key does not exist or if not an object.
     */
    Json operator[](const String& key) const;

    /**
     * @brief Accesses a value by index in a JSON array.
     * @param index The array index.
     * @return The Json value at the specified index.
     * @throws std::out_of_range if the index is out of bounds or if not an array.
     */
    Json operator[](SizeT index) const;

    /**
     * @brief Checks if the JSON object contains a given key.
     * @param key The key to check.
     * @return True if the key exists, false otherwise.
     */
    bool contains(const String& key) const;

    /**
     * @brief Checks if the value is null.
     * @return True if null, false otherwise.
     */
    bool isNull() const;

    /**
     * @brief Checks if the value is a JSON object.
     * @return True if object, false otherwise.
     */
    bool isObject() const;

    /**
     * @brief Checks if the value is a JSON array.
     * @return True if array, false otherwise.
     */
    bool isArray() const;

    /**
     * @brief Checks if the value is a string.
     * @return True if string, false otherwise.
     */
    bool isString() const;

    /**
     * @brief Checks if the value is a number.
     * @return True if number, false otherwise.
     */
    bool isNumber() const;

    /**
     * @brief Checks if the value is a boolean.
     * @return True if boolean, false otherwise.
     */
    bool isBoolean() const;

    /**
     * @brief Gets the length of the string value.
     * @return Length of the string, or 0 if not a string.
     */
    SizeT getStringLength() const;

    /**
     * @brief Gets the size of the array or object.
     * @return Number of elements if array or object, 0 otherwise.
     */
    SizeT getSize() const;

    /**
     * @brief Gets the string value.
     * @return The string value.
     * @throws std::runtime_error if not a string.
     */
    String getString() const;

    /**
     * @brief Copies the contents of an integer array value into the provided buffer.
     * @param outArray Pointer to the output buffer.
     * @param arraySize Size of the output buffer.
     * @throws std::runtime_error if not an array of integers or size mismatch.
     */
    void getArrayInt32(Int32* outArray, const SizeT& arraySize) const;

    /**
     * @brief Gets the boolean value.
     * @return The boolean value.
     * @throws std::runtime_error if not a boolean.
     */
    bool getBool() const;

    /**
     * @brief Gets the 32-bit integer value.
     * @return The integer value.
     * @throws std::runtime_error if not an integer.
     */
    Int32 getInt32() const;

    /**
     * @brief Gets the floating-point value.
     * @return The float value.
     * @throws std::runtime_error if not a number.
     */
    float getFloat() const;

  private:
    struct Impl;
    Impl* m_impl;

    /**
     * @brief Constructs a Json object from an implementation pointer.
     * @param impl Pointer to the implementation.
     */
    explicit Json(Impl* impl);
  };
}
