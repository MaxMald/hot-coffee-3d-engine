#pragma once

#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  /**
   * Descriptor for a Blinn-Phong material asset.
   */
  class HC_CORE_EXPORT BlinnPhongMaterialDescriptor :
    public AMaterialDescriptor
  {
  public:

    /**
     * Constructs a Blinn-Phong material descriptor.
     *
     * @param path The file path to the material descriptor asset
     * @param name The name of the material descriptor
     * @param color The base color of the material
     * @param shininess The shininess of the material, controlling the specular highlight
     * size. Clamped between 1.0 and 256.0.
     * @param albedoImagePath The file path to the albedo texture image
     * @param normalImagePath The file path to the normal map image
     * @param specularImagePath The file path to the specular texture image
     */
    BlinnPhongMaterialDescriptor(
      const Path& path,
      const String& name,
      const Color& color,
      float shininess,
      const Path& albedoImagePath,
      const Path& normalImagePath,
      const Path& specularImagePath
    );
    virtual ~BlinnPhongMaterialDescriptor() = default;

    /**
     * @copydoc AMaterialDescriptor::getType
     */
    materialType::Type getType() const override;

    /**
     * @copydoc AMaterialDescriptor::getImagesPaths
     */
    void getImagesPaths(Vector<Path>& paths) const override;

    /**
     * @brief Gets the base color of the material.
     *
     * @return The base color of the material
     */
    const Color& getColor() const;

    /**
     * @brief Gets the shininess of the material.
     *
     * @return The shininess of the material
     */
    float getShininess() const;

    /**
     * @brief Gets the file path for the albedo texture image.
      *
      * @return The path to the albedo texture asset
     */
    const Path& getAlbedoImagePath() const;

    /**
     * @brief Gets the file path for the normal map image.
     *
     * @return The path to the normal map asset
     */
    const Path& getNormalImagePath() const;

    /**
     * @brief Gets the file path for the specular texture image.
     *
     * @return The path to the specular texture asset
     */
    const Path& getSpecularImagePath() const;

  private:
    Color m_color;
    float m_shininess;
    Path m_albedoImagePath;
    Path m_normalImagePath;
    Path m_specularImagePath;
  };
}
