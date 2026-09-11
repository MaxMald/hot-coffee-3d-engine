#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

#include <hc/assets/metadata/hcMetadataFileFormat.h>
#include <hc/assets/metadata/hcModelMetadataManager.h>

namespace hc::editor
{
  EditorMetadataManager::EditorMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager)
  {}

  EditorMetadataManager::~EditorMetadataManager()
  {}

  void EditorMetadataManager::prepare()
  {}

  void EditorMetadataManager::destroy()
  {}

  void EditorMetadataManager::saveModelMetadata(MeshComponent& meshComponent)
  {
    Path sourcePath = meshComponent.getSourcePath();
    if (sourcePath.empty())
    {
      LogService::Warning(
        "EditorMetadataManager::saveModelMetadata: MeshComponent has no source path. Cannot save metadata."
      );
      return;
    }

    SharedPtr<IMesh> mesh = meshComponent.getMesh();
    if (!mesh)
    {
      LogService::Warning(
        String::Format(
          "EditorMetadataManager::saveModelMetadata: MeshComponent with source path '%s' has no mesh. Cannot save metadata.",
          sourcePath.toGenericString().c_str()
        )
      );
      return;
    }

    SharedPtr<Model> model = m_assetManager.getModelAssetManager().load(sourcePath);
    if (!model)
    {
      LogService::Warning(
        String::Format(
          "EditorMetadataManager::saveModelMetadata: Failed to load model from source path '%s'. Cannot save metadata.",
          sourcePath.toGenericString().c_str()
        )
      );
      return;
    }

    hc::assets::metadata::ModelMetadata modelMetadata;

    const Vector<SharedPtr<IMaterial>>& materials = mesh->getMaterials();
    for (const SharedPtr<IMaterial>& material : materials)
    {
      if (!material)
        continue;

      SharedPtr<MaterialDescriptor> materialDescriptor = createMaterialDescriptor(material);
      if (materialDescriptor == nullptr)
      {
        LogService::Error(
          String::Format(
            "EditorMetadataManager::saveModelMetadata: Failed to create MaterialDescriptor for material '%s'. Skipping.",
            material->getName().c_str()
          )
        );
        continue;
      }

      modelMetadata.materials.push_back(materialDescriptor);
    }

    hc::assets::metadata::ModelMetadataManager::SaveMetadata(sourcePath, modelMetadata);
  }

  SharedPtr<MaterialDescriptor> EditorMetadataManager::createMaterialDescriptor(
    const SharedPtr<IMaterial>& material
  )
  {
    if (!material)
      return nullptr;

    SharedPtr<MaterialDescriptor> descriptor = MakeShared<MaterialDescriptor>();
    descriptor->setType(material->getMaterialType());
    descriptor->name = material->getName();
    descriptor->alphaCutoutThreshold = material->getAlphaCutoutThreshold();
    descriptor->doubleSided = material->isDoubleSided();
    descriptor->renderMode = material->getRenderMode();

    switch (descriptor->getType())
    {
    case materialType::Unlit:
      copyUnlitDataFromMaterial(material, descriptor);
      break;
    case materialType::BlinnPhong:
      copyBlinnPhongDataFromMaterial(material, descriptor);
      break;
    case materialType::Hair:
      copyHairDataFromMaterial(material, descriptor);
      break;
    default:
      LogService::Error(
        String::Format(
          "EditorMetadataManager::createMaterialDescriptor: Unsupported material type '%d' for material '%s'.",
          static_cast<int>(descriptor->getType()),
          material->getName().c_str()
        )
      );
      return nullptr;
    }

    return descriptor;
  }

  void EditorMetadataManager::copyUnlitDataFromMaterial(
    const SharedPtr<IMaterial>& material,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    hc::assets::materialDescriptor::UnlitData* unlitData = descriptor->getIfUnlitData();
    if (!unlitData)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyUnlitDataFromMaterial: Material '%s' is not of type Unlit. Cannot copy data.",
          material->getName().c_str()
        )
      );
      return;
    }

    const SharedPtr<UnlitMaterial> unlitMat = std::dynamic_pointer_cast<UnlitMaterial>(material);
    if (!unlitMat)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyUnlitDataFromMaterial: Failed to cast material '%s' to UnlitMaterial.",
          material->getName().c_str()
        )
      );
      return;
    }

    unlitData->color = unlitMat->getColor();
    // TODO get the image path
  }

  void EditorMetadataManager::copyBlinnPhongDataFromMaterial(
    const SharedPtr<IMaterial>& material,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
  }

  void EditorMetadataManager::copyHairDataFromMaterial(
    const SharedPtr<IMaterial>& material,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {

  }
}
