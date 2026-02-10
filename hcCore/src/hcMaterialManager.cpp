#include "hc/hcMaterialManager.h"
#include "hc/hcShaderProgramManager.h"
#include "hc/hcAssetManager.h"
#include "hc/hcMaterialFactoriesManager.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcITextureManager.h"
#include "hc/hcIMaterialFactory.h"

namespace hc
{
  MaterialManager::MaterialManager(
    AssetManager& assetManager,
    ITextureManager& textureManager,
    IShaderProgramManager& shaderProgramManager,
    UniquePtr<MaterialFactoriesManager> materialFactoriesManager
  ) :
    m_assetManager(assetManager),
    m_shaderProgramManager(shaderProgramManager),
    m_textureManager(textureManager),
    m_materialFactoriesManager(std::move(materialFactoriesManager))
  {
  }

  SharedPtr<IMaterial> MaterialManager::createMaterialFromFile(
    const Path& materialDescriptorPath
  )
  {
    SharedPtr<MaterialDescriptor> mat = m_assetManager.load<MaterialDescriptor>(
      materialDescriptorPath
    );

    if (!mat)
    {
      LogService::Error(
        String::Format(
          "Failed to load MaterialDescriptor from path '%s'.",
          materialDescriptorPath.string().c_str()
        )
      );
      return nullptr;
    }

    return createMaterialFromDescriptor(mat);
  }

  SharedPtr<IMaterial> MaterialManager::createMaterialFromDescriptor(
    const String& materialDescriptorKey
  )
  {
    SharedPtr<MaterialDescriptor> mat = m_assetManager.get<MaterialDescriptor>(
      materialDescriptorKey
    );

    if (!mat)
    {
      LogService::Error(
        String::Format(
          "MaterialDescriptor with key '%s' not found.",
          materialDescriptorKey.c_str()
        )
      );
      return nullptr;
    }

    return createMaterialFromDescriptor(mat);
  }

  SharedPtr<IMaterial> MaterialManager::createMaterialFromDescriptor(
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    if (!descriptor)
    {
      LogService::Error(
        String::Format("Invalid MaterialDescriptor provided.")
      );
      return nullptr;
    }

    if (hasCachedResource(descriptor->getId()))
      return getCachedResource(descriptor->getId());

    if (!m_materialFactoriesManager)
    {
      LogService::Error(
        String::Format("MaterialFactoriesManager is not initialized.")
      );
      return nullptr;
    }

    if (!m_materialFactoriesManager->hasFactory(descriptor->getShaderType()))
    {
      String shaderTypeStr = shadingType::toString(descriptor->getShaderType());
      LogService::Error(
        String::Format(
          "No material factory registered for shader type '%s'.",
          shaderTypeStr.c_str()
        )
      );
      return nullptr;
    }

    IMaterialFactory& materialFactor = m_materialFactoriesManager->getFactory(descriptor->getShaderType());
    SharedPtr<IMaterial> material = materialFactor.create(
      descriptor,
      m_textureManager,
      m_shaderProgramManager
    );

    if (!material)
    {
      String shaderTypeStr = shadingType::toString(descriptor->getShaderType());
      LogService::Error(
        String::Format(
          "Material factory for shader type '%s' failed to create material.",
          shaderTypeStr.c_str()
        )
      );
      return nullptr;
    }

    cacheResource(descriptor->getId(), material);
    m_materials.push_back(material);

    return material;
  }

  const Vector<SharedPtr<IMaterial>>& MaterialManager::getMaterials() const
  {
    return m_materials;
  }

  void MaterialManager::clear()
  {
    clearCache();
    for (const auto& material : m_materials)
      material->destroy();
    m_materials.clear();
  }
}
