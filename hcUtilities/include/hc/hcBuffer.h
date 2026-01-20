#pragma once

#include "hc/hcUtilitiesPrerequisites.h"

namespace hc
{
  /**
   * @brief Utility class for managing a contiguous buffer of elements.
   *
   * Buffer provides memory management, copy/move semantics, and initialization
   * for a block of elements of type T.
   *
   * @tparam T Element type.
   */
  template<typename T>
  class Buffer
  {
  public:
    /**
     * @brief Constructs a buffer with the given size.
     * 
     * @param size Number of elements to allocate.
     */
    Buffer(SizeT size);
    Buffer(const Buffer& other);
    Buffer(Buffer&& other) noexcept;
    virtual ~Buffer();

    Buffer& operator= (const Buffer& other);
    Buffer& operator= (Buffer&& other) noexcept;

    /**
     * @brief Initializes the buffer with data from a source pointer. Allocates
     * and copies the given number of elements.
     * 
     * @param src Pointer to source data.
     * @param count Number of elements to copy.
     */
    void initialize(const T* src, SizeT count);

    /**
     * @brief Initializes the buffer by taking ownership of an existing buffer.
     * No copy is performed; ownership is transferred.
     * 
     * @param src Unique pointer to the source buffer.
     * @param count Number of elements in the buffer.
     */
    void initialize(UniquePtr<T[]>&& src, SizeT count);

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

  protected:
    UniquePtr<T[]> m_data;
    SizeT m_size;
  };

  template<typename T>
  Buffer<T>::Buffer(SizeT size)
    : m_data(size ? new T[size] : nullptr), m_size(size)
  {}

  template<typename T>
  Buffer<T>::Buffer(const Buffer& other)
    : m_data(other.m_size ? new T[other.m_size] : nullptr), m_size(other.m_size)
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
      m_data.reset(other.m_size ? new T[other.m_size] : nullptr);
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
  void Buffer<T>::initialize(const T* src, SizeT count)
  {
    m_data.reset(count ? new T[count] : nullptr);
    m_size = count;

    if (m_data && src)
      std::copy(src, src + count, m_data.get());
  }

  template<typename T>
  inline void Buffer<T>::initialize(UniquePtr<T[]>&& src, SizeT count)
  {
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

  using BufferByte = Buffer<Byte>;
  using BufferUInt32 = Buffer<UInt32>;
  using BufferFloat = Buffer<float>;
}
