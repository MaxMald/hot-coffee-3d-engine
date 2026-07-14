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
     * @copydoc ITexture::initialize(UInt32, UInt32, textureFormatType::Type)
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type colorFormat
    ) override;

    /**
     * @copydoc ITexture::initialize(UInt32, UInt32, textureFormatType::Type, colorSpaceType::Type)
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type colorFormat,
      colorSpaceType::Type colorSpace
    ) override;

    /**
     * @copydoc ITexture::initialize(UInt32, UInt32, textureFormatType::Type, colorSpaceType::Type, const Color&)
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type colorFormat,
      colorSpaceType::Type colorSpace,
      const Color& initColor
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
     * @brief Returns the OpenGL texture ID.
     *
     * @return OpenGL texture identifier.
     */
    GLuint getTextureId() const;

    /**
     * @brief Internal method to initialize the texture with specified parameters. This is
     * called by the public initialize methods after validating parameters.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param colorFormat The color format of the texture.
     * @param colorSpace The color space of the texture.
     * @param initData Optional pointer to initial pixel data to upload to the texture.
     */
    void initialize(
      UInt32 width,
      UInt32 height,
      textureFormatType::Type colorFormat,
      colorSpaceType::Type colorSpace,
      const void* initData
    );

  private:
    Id m_id;
    GLuint m_textureId;
    UInt32 m_width;
    UInt32 m_height;
    textureFormatType::Type m_textureFormat;
    colorSpaceType::Type m_colorSpace;
    bool m_created;

    static void assertDimensionsAreGreaterThanZero(UInt32 width, UInt32 height);
    void assertIsCreated() const;
    
  };
}
