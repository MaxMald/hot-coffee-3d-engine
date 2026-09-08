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
     * This function populates the provided vector with the paths of all images
     * used by the material descriptor. This method does not cleans the vector
     * before adding paths, so the caller should ensure it is empty or handle the
     * existing contents appropriately.
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
     *
     * This method can be used to reset the material descriptor before reusing it
     * or when it is no longer needed. Derived classes should override this method
     * to clear their specific state as well.
     */
    virtual void clear();

    /**
     * Gets the name of the material descriptor.
     *
     * @return The name of the material descriptor.
     */
    const String& getName() const { return m_name; }

    /**
     * Gets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     *
     * @return The material render mode.
     */
    materialRenderMode::Type getRenderMode() const;
    
    /**
     * Sets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     *
     * @param renderMode The material render mode to set.
     */
    void setRenderMode(materialRenderMode::Type renderMode);

    /**
     * Sets the alpha cutout threshold for the material. This value determines
     * the cutoff point for alpha testing when the material is rendered in alpha cutout
     * mode.
     * 
     * @param threshold The alpha cutout threshold value, typically between 0.0 and 1.0.
     */
    void setAlphaCutoutThreshold(float threshold);

    /**
     * Gets the alpha cutout threshold for the material. This value determines
     * the cutoff point for alpha testing when the material is rendered in alpha cutout
     * mode.
     *
     * @return The alpha cutout threshold value, typically between 0.0 and 1.0.
     */
    float getAlphaCutoutThreshold() const;

    /**
     * Determines if the material is double-sided, meaning it should be rendered
     * with back-face culling disabled so that both sides of the geometry are visible.
     *
     * @return True if the material is double-sided, false otherwise.
     */
    bool isDoubleSided() const;

    /**
     * Sets whether the material is double-sided, meaning it should be rendered
     * with back-face culling disabled so that both sides of the geometry are visible.
     *
     * @param doubleSided True to make the material double-sided, false for single-sided.
     */
    void setDoubleSided(bool doubleSided);

  protected:
    materialRenderMode::Type m_renderMode;
    float m_alphaCutoutThreshold;
    bool m_doubleSided;
    String m_name;

    /**
     * Constructs a material descriptor with the given asset path.
     *
     * @param path The file path to the material descriptor asset
     * @param name The name of the material descriptor
     * @param renderMode The material render mode (default is Opaque)
     */
    AMaterialDescriptor(
      const Path& path,
      const String& name = "",
      materialRenderMode::Type renderMode = materialRenderMode::Type::Opaque
    );

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
