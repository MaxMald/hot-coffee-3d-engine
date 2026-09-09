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
    Color color;        ///< The base color of the material
    Path mainImagePath; ///< The file path to the main texture image

    UnlitMaterialDescriptor();
    UnlitMaterialDescriptor(const Path& path);
    virtual ~UnlitMaterialDescriptor() = default;

    /**
     * Gets the material type used by this material.
     *
     * @return The unlit material type identifier
     */
    materialType::Type getType() const override;

    /**
     * Gets the file paths of all images referenced by this material.
     *
     * @param paths Output vector to populate with image paths
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
