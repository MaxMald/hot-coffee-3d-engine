#pragma once

#include "hc/editor/services/hcIEditorService.h"

namespace hc::editor
{
  class EditorMetadataManager : public IEditorService
  {
  public:
    EditorMetadataManager(IAssetManager& assetManager);
    virtual ~EditorMetadataManager() override;

    void prepare() override;
    void destroy() override;

    void saveModelMetadata(MeshComponent& meshComponent);

  private:
    IAssetManager& m_assetManager;

    SharedPtr<MaterialDescriptor> createMaterialDescriptor(const SharedPtr<IMaterial>& material);
    void copyUnlitDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
    void copyBlinnPhongDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
    void copyHairDataFromMaterial(const SharedPtr<IMaterial>& material, SharedPtr<MaterialDescriptor> descriptor);
  };
}
