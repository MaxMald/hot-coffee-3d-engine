#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  /**
   * Abstract base class for material descriptors.
   *
   * Defines the interface for material descriptors that describe
   * material properties and associated resources. Implementations
   * provide specific material types with their shading models and
   * texture references.
   */
  class HC_CORE_EXPORT AMaterialDescriptor : public Asset, public io::ISerializable
  {
  public:
    materialRenderMode::Type renderMode;  ///< The render mode of the material (e.g., Opaque, Transparent)
    float alphaCutoutThreshold;           ///< The threshold for alpha cutout rendering (used in AlphaCutout mode)
    bool doubleSided;                     ///< Indicates whether the material is double-sided (renders both front and back faces)
    String name;                          ///< The name of the material descriptor

    virtual ~AMaterialDescriptor() = default;

    /**
     * Gets the material type used by this material.
     *
     * @return The material type identifier
     */
    virtual materialType::Type getType() const = 0;

    /**
     * Gets the file paths of all images used by this material descriptor.
     *
     * This function populates the provided vector with the paths of all images used by
     * the material descriptor. This method does not clean the vector before adding paths,
     * so the caller should ensure it is empty or handle the existing contents
     * appropriately.
     *
     * @param paths Output parameter to receive the list of image paths.
     */
    virtual void getImagesPaths(Vector<Path>& paths) const = 0;

    /**
     * Serializes the material descriptor to a binary format.
     *
     * This method writes the state of the material descriptor to the provided
     * BinaryWriter. Derived classes should implement their specific serialization
     * logic in the onSerialization method.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void serialize(io::BinaryWriter& writer) const override;

    /**
     * Deserializes the material descriptor from a binary format.
     *
     * This method reads the state of the material descriptor from the provided
     * BinaryReader. Derived classes should implement their specific deserialization
     * logic in the onDeserialization method.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void deserialize(io::BinaryReader& reader) override;

    /**
     * Clears the material descriptor's state, resetting it to default values.
     */
    virtual void clear();

  protected:

    /**
     * Constructs a material descriptor with the given asset path.
     *
     * @param path The file path to the material descriptor asset
     */
    AMaterialDescriptor(const Path& path);

    /**
     * Hook for derived classes to implement their specific serialization logic.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void onSerialization(io::BinaryWriter& writer) const = 0;

    /**
     * Hook for derived classes to implement their specific deserialization logic.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void onDeserialization(io::BinaryReader& reader) = 0;

    /**
     * Gets the version number of the derived class for serialization purposes.
     *
     * This allows for versioning of serialized data, enabling backward compatibility
     * and proper handling of different versions of material descriptors.
     *
     * @return The version number of the derived class.
     */
    virtual UInt16 getDerivedVersion() const = 0;
  };
}
