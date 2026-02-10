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
     * @brief Constructs an OpenGlTexture from the given image.
     *
     * @param image Shared pointer to the image data used for texture creation.
     */
    OpenGlTexture(SharedPtr<Image> image);
    ~OpenGlTexture() override;

    /**
     * @brief Returns the unique identifier of the texture.
     * 
     * @return Reference to the texture Id.
     */
    const Id& getId() const override;

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
     * @brief Returns the image associated with this texture.
     * 
     * @return Shared pointer to the image.
     */
    SharedPtr<Image> getImage() override;

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

  private:
    Id m_id;
    SharedPtr<Image> m_image;
    GLuint m_textureId;
    UInt32 m_width;
    UInt32 m_height;
    UInt32 m_channels;
    bool m_created;
  };
}
