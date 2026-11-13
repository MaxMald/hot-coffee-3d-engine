#pragma once

#include <GL/glew.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc::graphics::resource::textureArray
{
  /**
   * @brief Represents an OpenGL texture array resource.
   */
  class OpenGlTextureArray : public ITextureArray
  {
  public:
    /**
     * @brief Default constructor for OpenGlTextureArray. Creates an uninitialized texture
     * array.
     */
    OpenGlTextureArray();
    ~OpenGlTextureArray() override;

    /**
     * @copydoc ITextureArray::initialize(const TextureArrayInitializationSettings&)
     */
    void initialize(
      const TextureArrayInitializationSettings& initializationSettings
    ) override;

    /**
     * @copydoc ITextureArray::getWidth
     */
    UInt32 getWidth() const override
    {
      return m_width;
    }

    /**
     * @copydoc ITextureArray::getHeight
     */
    UInt32 getHeight() const override
    {
      return m_height;
    }

    /**
     * @copydoc ITextureArray::getLayerCount
     */
    UInt32 getLayerCount() const override
    {
      return m_height;
    }

    /**
     * @copydoc ITextureArray::getTextureFormat
     */
    textureFormatType::Type getTextureFormat() const override
    {
      return m_textureFormat;
    }

    /**
    * @copydoc ITextureArray::getColorSpace
    */
    colorSpaceType::Type getColorSpace() const override
    {
      return m_colorSpace;
    }

    /**
     * @copydoc IGraphicResource::isValid
     */
    bool isValid() const override
    {
      return m_isValid;
    }

    /**
     * @copydoc IGraphicResource::destroy
     */
    void destroy() override;

    /**
     * @brief Returns the OpenGL texture array ID.
     *
     * @return OpenGL texture array identifier.
     */
    GLuint getOpenGLTextureArrayId() const
    {
      return m_textureArrayId;
    }

  private:
    GLuint m_textureArrayId;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_layerCount;
    textureFormatType::Type m_textureFormat;
    colorSpaceType::Type m_colorSpace;
    bool m_isValid;

    /**
     * @brief Asserts that the OpenGL texture array is valid. Throws a RuntimeErrorException
     * if the texture array is not valid.
     */
    inline void assertIsValid() const
    {
      if (!m_isValid)
        throw RuntimeErrorException("OpenGlTextureArray: The OpenGL texture array is not valid.");
    }
  };
}
