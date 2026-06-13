#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/memory/hcMemory.h"
#include "hc/utilities/hcString.h"

namespace hc
{
  /**
   * @brief Utility class for managing a contiguous block of memory
   * for trivially copyable types.
   *
   * Buffer allocates raw storage for a fixed number of elements.
   * Allocated memory is not initialized and its contents are undefined
   * until written by the caller.
   *
   * Buffer is intended for binary data, vertices, indices, image pixels,
   * and other POD/trivially-copyable types.
   *
   * @tparam T Trivially copyable element type.
   */
  template<typename T>
  class Buffer
  {
    static_assert(
      std::is_trivially_copyable_v<T>,
      "Buffer can only be instantiated with trivially copyable types."
    );

  public:
    /**
     * @brief Default constructor. Creates an empty buffer with no allocated memory.
     */
    Buffer();

    /**
     * @brief Constructs a buffer with the specified number of elements. Allocates
     * memory but does not initialize it.
     *
     * @param size Number of elements to allocate.
     */
    Buffer(SizeT size);

    /**
     * @brief Copy constructor. Creates a new buffer by copying the contents of another
     * buffer.
     *
     * @param other Buffer to copy from.
     */
    Buffer(const Buffer& other);

    /**
     * @brief Move constructor. Creates a new buffer by taking ownership of the contents
     * of another buffer. The source buffer is left in an empty state.
     *
     * @param other Buffer to move from.
     */
    Buffer(Buffer&& other) noexcept;

    /**
     * @brief Destructor. Releases any allocated memory.
     */
    ~Buffer();

    Buffer& operator= (const Buffer& other);
    Buffer& operator= (Buffer&& other) noexcept;
    T& operator[](SizeT index);
    const T& operator[](SizeT index) const;

    /**
     * @brief Initializes the buffer with data from a source pointer. Allocates and copies
     * the given number of elements.
     *
     * @param src Pointer to source data.
     * @param count Number of elements to copy.
     */
    void initialize(const T* src, SizeT count);

    /**
     * @brief Initializes the buffer by taking ownership of an existing buffer. No copy is
     * performed; ownership is transferred.
     *
     * @param src Unique pointer to the source buffer.
     * @param count Number of elements in the buffer.
     */
    void initialize(memory::UniqueArrayPtr<T>&& src, SizeT count);

    /**
     * @brief Returns a pointer to the buffer data.
     *
     * @return Pointer to the buffer data.
     */
    T* data();

    /**
     * @brief Returns a const pointer to the buffer data.
     *
     * @return Const pointer to the buffer data.
     */
    const T* data() const;

    /**
     * @brief Returns the number of elements in the buffer.
     * @return Buffer size.
     */
    SizeT size() const;

    /**
     * @brief Checks if the buffer is empty (size is zero).
     *
     * @return True if the buffer is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Allocates storage for the specified number of elements.
     *
     * Existing contents are discarded. Newly allocated memory is not initialized and
     * contains undefined values.
     *
     * @param size Number of elements to allocate.
     */
    void reset(SizeT size);

    /**
     * @brief Clears the buffer, releasing all allocated memory and setting the size to
     * zero.
     */
    void clear();

  protected:
    memory::UniqueArrayPtr<T> m_data;
    SizeT m_size;
  };

  template<typename T>
  Buffer<T>::Buffer() : m_data(nullptr), m_size(0)
  {}

  template<typename T>
  Buffer<T>::Buffer(SizeT size)
    : m_data(size ? memory::AllocArray<T>(size) : nullptr), m_size(size)
  {}

  template<typename T>
  Buffer<T>::Buffer(const Buffer& other)
    : m_data(other.m_size ? memory::AllocArray<T>(other.m_size) : nullptr), m_size(other.m_size)
  {
    if (m_data && other.m_data)
      std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
  }

  template<typename T>
  Buffer<T>::Buffer(Buffer&& other) noexcept
    : m_data(std::move(other.m_data)), m_size(other.m_size)
  {
    other.m_size = 0;
  }

  template<typename T>
  Buffer<T>::~Buffer() = default;

  template<typename T>
  Buffer<T>& Buffer<T>::operator=(const Buffer& other)
  {
    if (this != &other)
    {
      m_data.reset(other.m_size ? memory::AllocArray<T>(other.m_size) : nullptr);
      m_size = other.m_size;

      if (m_data && other.m_data)
        std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
    }

    return *this;
  }

  template<typename T>
  Buffer<T>& Buffer<T>::operator=(Buffer&& other) noexcept
  {
    if (this != &other)
    {
      m_data = std::move(other.m_data);
      m_size = other.m_size;
      other.m_size = 0;
    }

    return *this;
  }

  template<typename T>
  T& Buffer<T>::operator[](SizeT index)
  {
    if (index >= m_size)
      throw OutOfRangeException("Buffer index out of range.");
    return m_data[index];
  }

  template<typename T>
  const T& Buffer<T>::operator[](SizeT index) const
  {
    if (index >= m_size)
      throw OutOfRangeException("Buffer index out of range.");
    return m_data[index];
  }

  template<typename T>
  void Buffer<T>::initialize(const T* src, SizeT count)
  {
    if (count == 0)
    {
      m_data.reset();
      m_size = 0;
      return;
    }

    if (!src)
      throw InvalidArgumentException("Source pointer cannot be null when count is greater than zero.");

    m_data.reset(memory::AllocArray<T>(count));
    m_size = count;

    if (m_data && src)
      std::copy(src, src + count, m_data.get());
  }

  template<typename T>
  inline void Buffer<T>::initialize(memory::UniqueArrayPtr<T>&& src, SizeT count)
  {
    if (count == 0)
    {
      m_data.reset();
      m_size = 0;
      return;
    }

    if (!src)
      throw InvalidArgumentException("Source buffer cannot be null when count is greater than zero.");

    m_data = std::move(src);
    m_size = count;
  }

  template<typename T>
  T* Buffer<T>::data()
  {
    return m_data.get();
  }

  template<typename T>
  const T* Buffer<T>::data() const
  {
    return m_data.get();
  }

  template<typename T>
  SizeT Buffer<T>::size() const
  {
    return m_size;
  }

  template<typename T>
  bool Buffer<T>::empty() const
  {
    return m_size == 0;
  }

  template<typename T>
  inline void Buffer<T>::reset(SizeT size)
  {
    m_data.reset(size ? memory::AllocArray<T>(size) : nullptr);
    m_size = size;
  }

  template<typename T>
  inline void Buffer<T>::clear()
  {
    m_data.reset();
    m_size = 0;
  }

  using BufferByte = Buffer<Byte>;
  using BufferUInt32 = Buffer<UInt32>;
  using BufferFloat = Buffer<float>;
}
