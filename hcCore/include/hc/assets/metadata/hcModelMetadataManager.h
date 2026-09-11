#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"
#include "hc/assets/metadata/hcMetadataCommons.h"

namespace hc
{
  class IAssetManager;
  class MaterialDescriptor;
  class Model;
}

namespace hc::assets::metadata
{
  /**
   * @brief Represents a material override for a model.
   */
  struct HC_CORE_EXPORT ModelMaterialOverride : public io::ISerializable
  { 
    String name = "";                                   ///< The name of the material to override
    SizeT  materialIndex = 0;                           ///< The index of the material in the model's material list that this override applies to
    Path sourcePath = "";                               ///< The path to the source of the material override in case of external source
    MaterialDescriptor material;                        ///< The material descriptor for the override
    sourceType::Type sourceType = sourceType::Embedded; ///< The type of the source (embedded or external)

    /**
     * @copydoc io::ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc io::ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;
    
    /**
     * @brief Clears the material override data.
     */
    void clear();
  };

  /**
   * @brief Represents metadata for a model
   */
  struct HC_CORE_EXPORT ModelMetadata : public io::ISerializable
  { 
    Vector<SharedPtr<ModelMaterialOverride>> materialOverrides; ///< A collection of material overrides for the model

    /**
     * @copydoc io::ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc io::ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Adds an embedded material override to the metadata.
     *
     * @param name The name of the material to override.
     * @param materialIndex The index of the material in the model's material list that
     * this override applies to.
     * @param descriptor The material descriptor for the override.
     */
    void addEmbeddedMaterialOverride(
      const String& name,
      SizeT materialIndex,
      const MaterialDescriptor& descriptor
    );

    /**
     * @brief Adds an external material override to the metadata.
     *
     * @param name The name of the material to override.
     * @param materialIndex The index of the material in the model's material list that
     * this override applies to.
     * @param sourcePath The path to the source of the material. Could be absolute or
     * relative to the asset manager's root.
     */
    void addExternalMaterialOverride(
      const String& name,
      SizeT materialIndex,
      const Path& sourcePath
    );

    /**
     * @brief Removes a material override from the metadata by name.
     *
     * @param name The name of the material to override to remove.
     */
    void removeMaterialOverride(const String& name);

    /**
     * @brief Clears all material overrides from the metadata.
     */
    void clear();
  };

  /**
   * @brief Manages the loading and saving of model metadata.
   */
  class HC_CORE_EXPORT ModelMetadataManager : public NonCopyable, public NonMovable
  {
  public:
    ModelMetadataManager(IAssetManager& assetManager);
    ~ModelMetadataManager();

    /**
     * @brief Checks if metadata exists for the specified model path.
     *
     * @param modelPath The path to the model file.
     * @return true if metadata exists, false otherwise.
     */
    bool has(const Path& modelPath) const;

    /**
     * @brief Loads metadata for the specified model path.
     *
     * @param modelPath The path to the model file.
     * @return The loaded ModelMetadata object.
     */
    ModelMetadata load(const Path& modelPath);

    /**
     * @brief Loads and applies metadata for the specified model path to the given model.
     *
     * @param modelPath The path to the model file.
     * @param model The model object to apply the metadata to.
     */
    void apply(const Path& modelPath, Model& model);

    /**
     * @brief Saves metadata for the specified model path and model.
     *
     * @param modelPath The path to the model file.
     * @param model The model object to save metadata from.
     */
    void save(const Path& modelPath, const Model& model);

    /**
     * @brief Saves the provided metadata for the specified model path.
     *
     * @param modelPath The path to the model file.
     * @param metadata The metadata object to save.
     */
    void save(const Path& modelPath, const ModelMetadata& metadata);

    /**
     * @brief Saves an embedded material override for the specified model path.
     *
     * @param modelPath The path to the model file.
     * @param materialName The name of the material to override.
     * @param materialIndex The index of the material in the model's material list that
     * this override applies to.
     * @param descriptor The material descriptor for the override.
     */
    void saveEmbeddedMaterialOverride(
      const Path& modelPath,
      const String& materialName,
      SizeT materialIndex,
      const MaterialDescriptor& descriptor
    );

    /**
     * @brief Saves an external material override for the specified model path.
     *
     * @param modelPath The path to the model file.
     * @param materialName The name of the material to override.
     * @param materialIndex The index of the material in the model's material list that
     * this override applies to.
     * @param sourcePath The path to the source of the material. Could be absolute or
     * relative to the asset manager's root.
     */
    void saveExternalMaterialOverride(
      const Path& modelPath,
      const String& materialName,
      SizeT materialIndex,
      const Path& sourcePath
    );

    /**
     * @brief Removes a material override for the specified model path by material name.
     *
     * @param modelPath The path to the model file.
     * @param materialName The name of the material to override to remove.
     */
    void removeMaterialOverride(
      const Path& modelPath,
      const String& materialName
    );

  private:
    IAssetManager& m_assetManager;

    static Path GetMetadataFilePath(const Path& modelPath);
    void applyMetadata(const ModelMetadata& metadata, Model& model);
  };
}
