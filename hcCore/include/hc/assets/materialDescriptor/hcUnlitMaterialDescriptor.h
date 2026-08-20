#pragma once

#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  /**
   * Descriptor for an unlit material asset.
   *
   * Defines an unlit material with a base color and optional main texture.
   * Unlit materials do not respond to lighting and are rendered with
   * their color and texture directly.
   */
  class HC_CORE_EXPORT UnlitMaterialDescriptor :
    public AMaterialDescriptor
  {
  public:
    /**
     * Constructs a default unlit material descriptor with default properties.
     */
    UnlitMaterialDescriptor();

    /**
     * Constructs an unlit material descriptor.
     *
     * @param path The file path to the material descriptor asset
     * @param name The name of the material descriptor
     * @param color The base color of the material
     * @param mainImagePath The file path to the main texture image
     */
    UnlitMaterialDescriptor(
      const Path& path,
      const String& name,
      const Color& color,
      const Path& mainImagePath
    );

    virtual ~UnlitMaterialDescriptor() = default;

    /**
     * Gets the shading type used by this material.
     *
     * @return The unlit shading type identifier
     */
    shadingType::Type getShaderType() const override;

    /**
     * Gets the file paths of all images referenced by this material.
     *
     * @param paths Output vector to populate with image paths
     */
    void getImagesPaths(Vector<Path>& paths) const override;

    /**
     * Gets the base color property of the unlit material.
     *
     * @return The material's color value
     */
    const Color& getColor() const;

    /**
     * Gets the file path for the main texture image.
     *
     * @return The path to the main texture asset
     */
    const Path& getMainImagePath() const;

  private:
    Color m_color;
    Path m_mainImagePath;
  };
}
