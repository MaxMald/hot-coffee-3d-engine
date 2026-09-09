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
    Color color;            ///< The base color of the material
    float shininess;        ///< The shininess of the material, controlling the specular highlight size
    Path albedoImagePath;   ///< The file path to the albedo texture image
    Path normalImagePath;   ///< The file path to the normal map image
    Path specularImagePath; ///< The file path to the specular texture image

    BlinnPhongMaterialDescriptor();
    BlinnPhongMaterialDescriptor(const Path& path);
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
