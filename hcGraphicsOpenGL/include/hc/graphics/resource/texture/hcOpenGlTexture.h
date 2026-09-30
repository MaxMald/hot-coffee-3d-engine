#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include <GL/glew.h>

namespace hc
{
  class Image;

  /**
   * @brief Represents an OpenGL texture resource.
   */
  class OpenGlTexture : public ITexture
  {
  public:
    /**
     * @brief Default constructor for OpenGlTexture. Creates an uninitialized texture.
     */
    OpenGlTexture();
    ~OpenGlTexture() override;

    /**
     * @copydoc ITexture::initialize(const Image&)
     */
    void initialize(
      const Image& image,
      const Path& sourcePath = Path()
    ) override;

    /**
     * @brief Returns the width of the texture in pixels.
     *
     * @return Texture width.
     */
    UInt32 getWidth() const override;

    /**
     * @brief Returns the height of the texture in pixels.
     *
     * @return Texture height.
     */
    UInt32 getHeight() const override;

    /**
     * @copydoc ITexture::getTextureFormat
     */
    textureFormatType::Type getTextureFormat() const override;

    /**
    * @copydoc ITexture::getColorSpace
    */
    colorSpaceType::Type getColorSpace() const override;

    /**
     * @brief Resizes the texture to the specified dimensions. Resizing a texture created
     * from an image is not allowed, create a new texture instead.
     *
     * @param width The new width of the texture in pixels.
     * @param height The new height of the texture in pixels.
     */
    void resize(UInt32 width, UInt32 height) override;

    /**
     * @brief Binds the texture to the specified texture slot.
     *
     * @param slot Texture unit to bind to (default is 0).
     */
    void bind(UInt32 slot = 0) const override;

    /**
     * @brief Unbinds the texture from the specified texture slot.
     *
     * @param slot Texture unit to unbind from (default is 0).
     */
    void unbind(UInt32 slot = 0) const override;

    /**
     * @brief Checks if the texture is valid and has been created.
     *
     * @return True if the texture is valid, false otherwise.
     */
    bool isValid() const override;

    /**
     * @brief Destroys the texture and releases OpenGL resources.
     */
    void destroy() override;

    /**
     * @brief Returns the native OpenGL handle for the texture.
     *
     * @return Pointer to the OpenGL texture handle.
     */
    void* getNativeHandle() const override;

    /**
     * @copydoc ITexture::getSourcePath
     */
    const Path& getSourcePath() const override
    {
      return m_sourcePath;
    }

    /**
     * @brief Returns the OpenGL texture ID.
     *
     * @return OpenGL texture identifier.
     */
    GLuint getTextureId() const;

  private:
    Path m_sourcePath;
    GLuint m_textureId;
    UInt32 m_width;
    UInt32 m_height;
    textureFormatType::Type m_textureFormat;
    colorSpaceType::Type m_colorSpace;
    bool m_created;

    /**
     * @brief Asserts that the texture has been created before performing operations on it.
     *
     * @throws RuntimeErrorException if the texture has not been created yet.
     */
    inline void assertIsCreated() const
    {
      if (!m_created)
        throw RuntimeErrorException("Texture has not been created yet.");
    }
  };
}
