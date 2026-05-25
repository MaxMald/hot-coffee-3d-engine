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

    /**
     * @brief Constructs an OpenGlTexture from the given image.
     *
     * @param image Shared pointer to the image data used for texture creation.
     */
    OpenGlTexture(SharedPtr<Image> image);

    /**
    * @brief Constructs an empty OpenGlTexture with specified dimensions and format.
    *
    * @param width The width of the texture in pixels.
    * @param height The height of the texture in pixels.
    * @param internalFormat The internal format of the texture (default is GL_RGBA8).
    * @param format The format of the pixel data (default is GL_RGBA).
    * @param type The data type of the pixel data (default is GL_UNSIGNED_BYTE).
    */
    OpenGlTexture(
      UInt32 width,
      UInt32 height,
      GLenum internalFormat = GL_RGBA8,
      GLenum format = GL_RGBA,
      GLenum type = GL_UNSIGNED_BYTE
    );

    ~OpenGlTexture() override;

    /**
     * @brief Returns the unique identifier of the texture.
     *
     * @return Reference to the texture Id.
     */
    const Id& getId() const override;

    /**
     * @copydoc ITexture::initialize(const Image&)
     */
    void initialize(const Image& image) override;

    /**
     * @copydoc ITexture::initialize(UInt32, UInt32, UInt8)
     */
    void initialize(UInt32 width, UInt32 height, UInt8 channels) override;

    /**
     * @copydoc ITexture::initialize(UInt32, UInt32, UInt8, const Color&)
     */
    void initialize(UInt32 width, UInt32 height, UInt8 channels, const Color& initColor) override;

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
     * @copydoc ITexture::getChannels
     */
    UInt8 getChannels() const override;

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
     * @brief Returns the OpenGL texture ID.
     *
     * @return OpenGL texture identifier.
     */
    GLuint getTextureId() const;

    /**
     * @brief Initializes the texture with the specified dimensions and format. This
     * creates an empty texture without pixel data.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param internalFormat The internal format of the texture (e.g., GL_RGBA8).
     * @param format The format of the pixel data (e.g., GL_RGBA).
     * @param type The data type of the pixel data (e.g., GL_UNSIGNED_BYTE).
     * @param initData Optional pointer to initial pixel data to upload to the texture. If
     * nullptr, the texture will be created with uninitialized data.
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      GLenum internalFormat,
      GLenum format,
      GLenum type,
      const void* initData = nullptr
    );

  private:
    Id m_id;
    GLuint m_textureId;
    UInt32 m_width;
    UInt32 m_height;
    UInt8 m_channels;
    GLenum m_internalFormat;
    GLenum m_format;
    GLenum m_type;
    bool m_created;

    static void assertNumberOfChannels(UInt8 channels);
    static void assertDimensionsAreGreaterThanZero(UInt32 width, UInt32 height);
    void assertIsCreated() const;
    
  };
}
