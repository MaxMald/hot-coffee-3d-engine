#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Manages model metadata for the editor.
   *
   * This class provides functionality to create and manage model metadata entries
   * for models used in the editor. It allows saving material overrides for models,
   * handling both embedded and external materials.
   */
  class EditorModelMetadataManager
  {
  public:
    EditorModelMetadataManager(IAssetManager& assetManager);
    ~EditorModelMetadataManager();

    /**
     * @brief Creates a new model metadata entry for the specified model.
     *
     * This function creates a new model metadata entry for the given model path. If the
     * model metadata already exists, it will be overwritten.
     *
     * @param pathToModel The path to the model for which the metadata is being created.
     */
    void create(const Path& pathToModel);

    /**
     * @brief Saves the material for override in the model metadata.
     *
     * This function saves the provided material as an override for the specified model.
     * It handles both embedded and external materials based on the material's source
     * path.
     *
     * @param pathToModel The path to the model for which the material override is being
     * saved.
     * @param materialName The name of the material to be overridden.
     * @param materialIndex The index of the material in the model's material list.
     * @param material The shared pointer to the IMaterial instance to be saved as an
     * override.
     *
     * @return true if the material override was successfully saved; false otherwise.
     */
    bool saveMaterialForOverride(
      const Path& pathToModel,
      const String& materialName,
      SizeT materialIndex,
      const SharedPtr<IMaterial>& material
    );

  private:
    IAssetManager& m_assetManager;

    SharedPtr<MaterialDescriptor> createMaterialDescriptor(const SharedPtr<IMaterial>& material);
    void copyUnlitDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
    void copyBlinnPhongDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
    void copyHairDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
  };
}
