#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  /**
   * @brief Generic uniform buffer object (UBO) class template for managing GPU buffers
   * that store data of type T.
   *
   * This class own a UBO handle, manages its lifecycle, and provides methods to
   * initialize storage, bindBase to a binding point and upload new data.
   */
  template<typename T>
  class UniformBufferObject : public NonCopyable
  {
  public:
    UniformBufferObject();
    ~UniformBufferObject();

    /**
     * @brief Initializes the uniform buffer object (UBO).
     *
     * This method generates a new UBO, binds it, and allocates storage for the data of
     * type T.
     *
     * @param initialData Initial data to upload to the UBO.
     */
    void initialize(const T& initialData);

    /**
     * @brief Binds the UBO to a specified binding point index.
     *
     * This makes the UBO available to shaders that reference the same binding point.
     *
     * @param bindingIndex The index of the binding point to bindBase the UBO to.
     */
    void bindBase(UInt32 bindingIndex);

    /**
     * @brief Uploads new data to the UBO.
     *
     * This method updates the contents of the UBO with the provided data.
     *
     * @param data The new data to upload to the UBO.
     */
    void upload(const T& data);

    /**
     * @brief Destroys the UBO and releases GPU resources.
     *
     * After calling this method, the UBO is no longer valid.
     */
    void destroy();

    /**
     * @brief Checks if the UBO has been initialized.
     *
     * @return True if the UBO is initialized and valid, false otherwise.
     */
    bool isInitialized() const;

  private:
    UInt32 m_ubo;
    bool m_isInitialized;

    void assertIsValid() const;
  };

  template<typename T>
  inline UniformBufferObject<T>::UniformBufferObject():
    m_ubo(0), m_isInitialized(false)
  {}

  template<typename T>
  inline UniformBufferObject<T>::~UniformBufferObject()
  {
    destroy();
  }

  template<typename T>
  inline void UniformBufferObject<T>::initialize(const T& initialData)
  {
    if (m_isInitialized)
      throw RuntimeErrorException("UniformBufferObject is already initialized.");

    GLint currentUBOBinding = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentUBOBinding);

    try
    {
      glGenBuffers(1, &m_ubo);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(T), &initialData, GL_DYNAMIC_DRAW);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      destroy();
      glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
      throw;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
    m_isInitialized = true;
  }

  template<typename T>
  inline void UniformBufferObject<T>::bindBase(UInt32 bindingIndex)
  {
    assertIsValid();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, m_ubo);
  }

  template<typename T>
  inline void UniformBufferObject<T>::upload(const T & data)
  {
    assertIsValid();

    GLint currentUBOBinding = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &currentUBOBinding);

    try
    {
      glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
      throw;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, currentUBOBinding);
  }

  template<typename T>
  inline void UniformBufferObject<T>::destroy()
  {
    if (m_ubo != 0)
    {
      glDeleteBuffers(1, &m_ubo);
      m_ubo = 0;
    }
    m_isInitialized = false;
  }

  template<typename T>
  inline bool UniformBufferObject<T>::isInitialized() const
  {
    return m_isInitialized;
  }

  template<typename T>
  inline void UniformBufferObject<T>::assertIsValid() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("UniformBufferObject is not initialized.");
  }

  using LightFrameUBO = UniformBufferObject<LightFrameData>;
  using CameraFrameUBO = UniformBufferObject<CameraRenderData>;
}
