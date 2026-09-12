#include "hc/editor/services/metadataManager/hcEditorModelMetadataManager.h"

#include <hc/assets/metadata/hcModelMetadataManager.h>

namespace hc::editor
{
  EditorModelMetadataManager::EditorModelMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager)
  {
  }

  EditorModelMetadataManager::~EditorModelMetadataManager()
  {
  }

  void EditorModelMetadataManager::create(
    const Path& pathToModel
  )
  {
    SharedPtr<Model> model;

    try
    {
      model = m_assetManager
        .getModelAssetManager()
        .load(pathToModel);
    }
    catch (const std::exception& e)
    {
      LogService::Error(
        String::Format(
          "EditorModelMetadataManager::create: Failed to load model '%s'. Error: %s",
          pathToModel.toGenericString().c_str(),
          e.what()
        )
      );
      return;
    }

    if (!model)
    {
      LogService::Error(
        String::Format(
          "EditorModelMetadataManager::create: Model '%s' is null after loading.",
          pathToModel.toGenericString().c_str()
        )
      );
      return;
    }

    hc::assets::metadata::ModelMetadataManager& modelMetaManager
      = m_assetManager.getModelMetadataManager();

    if (modelMetaManager.has(pathToModel))
    {
      LogService::Warning(
        String::Format(
          "EditorModelMetadataManager::create: Model metadata will be overwritten for '%s'.",
          pathToModel.toGenericString().c_str()
        )
      );
    }

    try
    {
      modelMetaManager.save(pathToModel, *model);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "EditorModelMetadataManager::create: Failed to save model metadata for '%s'. Error: %s",
          pathToModel.toGenericString().c_str(),
          e.what()
        )
      );
    }
  }

  bool EditorModelMetadataManager::saveMaterialForOverride(
    const Path& pathToModel,
    const String& materialName,
    SizeT materialIndex,
    const SharedPtr<IMaterial>& material
  )
  {
    if (!material)
    {
      LogService::Error(
        String::Format(
          "EditorModelMetadataManager::saveMaterialForOverride: Material is null for model '%s', material name '%s'",
          pathToModel.toGenericString().c_str(),
          materialName.c_str()
        )
      );
      return false;
    }

    hc::assets::metadata::ModelMetadataManager& modelMetaManager
      = m_assetManager.getModelMetadataManager();

    if (!modelMetaManager.has(pathToModel))
      create(pathToModel);

    modelMetaManager.removeMaterialOverride(
      pathToModel,
      materialName
    );

    Path matSourcePath = material->getSourcePath();
    if (matSourcePath.empty()) // Embedded Material
    {
      SharedPtr<MaterialDescriptor> descriptor = createMaterialDescriptor(material);
      if (!descriptor)
      {
        LogService::Error(
          String::Format(
            "EditorModelMetadataManager::saveMaterialForOverride: Failed to create material descriptor for model '%s', material name '%s'",
            pathToModel.toGenericString().c_str(),
            materialName.c_str()
          )
        );
        return false;
      }

      try
      {
        modelMetaManager.saveEmbeddedMaterialOverride(
          pathToModel,
          materialName,
          materialIndex,
          *descriptor
        );
      }
      catch (const Exception& e)
      {
        LogService::Error(
          String::Format(
            "EditorModelMetadataManager::saveMaterialForOverride: Failed to save material override for model '%s', material name '%s'. Error: %s",
            pathToModel.toGenericString().c_str(),
            materialName.c_str(),
            e.what()
          )
        );
        return false;
      }
    }
    else // External Material
    {
      Path resolvedSourcePath = matSourcePath;
      if (resolvedSourcePath.isAbsolute())
        resolvedSourcePath = resolvedSourcePath.toRelative(m_assetManager.getRootPath());

      try
      {
        modelMetaManager.saveExternalMaterialOverride(
          pathToModel,
          materialName,
          materialIndex,
          resolvedSourcePath
        );
      }
      catch (const Exception& e)
      {
        LogService::Error(
          String::Format(
            "EditorModelMetadataManager::saveMaterialForOverride: Failed to save external material override for model '%s', material name '%s'. Error: %s",
            pathToModel.toGenericString().c_str(),
            materialName.c_str(),
            e.what()
          )
        );
        return false;
      }
    }
    return true;
  }

  SharedPtr<MaterialDescriptor> EditorModelMetadataManager::createMaterialDescriptor(
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

  void EditorModelMetadataManager::copyUnlitDataFromMaterial(
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

    SharedPtr<ITexture> mainTexture = unlitMat->getMainTexture();
    if (mainTexture)
      unlitData->textureImagePath = mainTexture->getSourcePath();
  }

  void EditorModelMetadataManager::copyBlinnPhongDataFromMaterial(
    const SharedPtr<IMaterial>& material,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    hc::assets::materialDescriptor::BlinnPhongData* matData = descriptor->getIfBlinnPhongData();
    if (!matData)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyBlinnPhongDataFromMaterial: Material '%s' is not of type BlinnPhong. Cannot copy data.",
          material->getName().c_str()
        )
      );
      return;
    }

    const SharedPtr<BlinnPhongMaterial> blinnPhongMat = std::dynamic_pointer_cast<BlinnPhongMaterial>(material);
    if (!blinnPhongMat)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyBlinnPhongDataFromMaterial: Failed to cast material '%s' to BlinnPhongMaterial.",
          material->getName().c_str()
        )
      );
      return;
    }

    matData->color = blinnPhongMat->getColor();
    matData->shininess = blinnPhongMat->getShininess();

    SharedPtr<ITexture> pTexture = blinnPhongMat->getAlbedoTexture();
    if (pTexture)
      matData->diffuseImagePath = pTexture->getSourcePath();

    pTexture = blinnPhongMat->getNormalTexture();
    if (pTexture)
      matData->normalImagePath = pTexture->getSourcePath();

    pTexture = blinnPhongMat->getSpecularTexture();
    if (pTexture)
      matData->specularImagePath = pTexture->getSourcePath();
  }

  void EditorModelMetadataManager::copyHairDataFromMaterial(
    const SharedPtr<IMaterial>& material,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    hc::assets::materialDescriptor::HairData* matData = descriptor->getIfHairData();
    if (!matData)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyHairDataFromMaterial: Material '%s' is not of type Hair. Cannot copy data.",
          material->getName().c_str()
        )
      );
      return;
    }

    const SharedPtr<HairMaterial> hairMat = std::dynamic_pointer_cast<HairMaterial>(material);
    if (!hairMat)
    {
      LogService::Error(
        String::Format(
          "EditorMetadataManager::copyHairDataFromMaterial: Failed to cast material '%s' to HairMaterial.",
          material->getName().c_str()
        )
      );
      return;
    }

    matData->color = hairMat->getColor();
    matData->specularPrimaryColor = hairMat->getSpecularPrimaryColor();
    matData->specularSecondaryColor = hairMat->getSpecularSecondaryColor();
    matData->shininess = hairMat->getShininess();
    matData->specularPrimaryShift = hairMat->getSpecularPrimaryShift();
    matData->specularSecondaryShift = hairMat->getSpecularSecondaryShift();
    matData->specularWidth = hairMat->getSpecularWidth();
    matData->specularStrength = hairMat->getSpecularStrength();

    SharedPtr<ITexture> pTexture = hairMat->getAlbedoTexture();
    if (pTexture)
      matData->albedoImagePath = pTexture->getSourcePath();

    pTexture = hairMat->getNormalTexture();
    if (pTexture)
      matData->normalImagePath = pTexture->getSourcePath();

    pTexture = hairMat->getSpecularTexture();
    if (pTexture)
      matData->specularImagePath = pTexture->getSourcePath();
  }
}
