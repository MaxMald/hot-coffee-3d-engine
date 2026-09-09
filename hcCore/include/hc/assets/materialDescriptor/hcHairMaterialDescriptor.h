#pragma once

#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  /**
   * Descriptor for a Hair material asset.
   */
  class HC_CORE_EXPORT HairMaterialDescriptor :
    public AMaterialDescriptor
  {
  public:
    Color color;                    ///< The base color of the material
    Color specularPrimaryColor;     ///< The primary specular color of the material
    Color specularSecondaryColor;   ///< The secondary specular color of the material
    float shininess;                ///< The shininess of the material, controlling the specular highlight size
    float specularPrimaryShift;     ///< The shift of the primary specular highlight
    float specularSecondaryShift;   ///< The shift of the secondary specular highlight
    float specularWidth;            ///< The width of the specular highlight
    float specularStrength;         ///< The strength of the specular highlight
    Path albedoImagePath;           ///< The file path to the albedo texture image
    Path normalImagePath;           ///< The file path to the normal map image
    Path specularImagePath;         ///< The file path to the specular texture image

    HairMaterialDescriptor();
    HairMaterialDescriptor(const Path& path);
    virtual ~HairMaterialDescriptor() = default;

    /**
     * @copydoc AMaterialDescriptor::getType
     */
    materialType::Type getType() const override;

    /**
     * @copydoc AMaterialDescriptor::getImagesPaths
     */
    void getImagesPaths(Vector<Path>& paths) const override;

    /**
     * @copydoc AMaterialDescriptor::clear
     */
    virtual void clear() override;

  protected:
    /**
     * @copydoc AMaterialDescriptor::onSerialization
     */
    virtual void onSerialization(io::BinaryWriter& writer) const override;

    /**
     * @copydoc AMaterialDescriptor::onDeserialization
     */
    virtual void onDeserialization(io::BinaryReader& reader) override;
    
    /**
     * @copydoc AMaterialDescriptor::getDerivedVersion
     */
    virtual UInt16 getDerivedVersion() const override;
  };
}
