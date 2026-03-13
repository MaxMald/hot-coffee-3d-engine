#include "hc/graphics/resource/material/hcMaterialManager.h"
#include "hc/graphics/resource/shaderProgram/hcShaderProgramManager.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManager.h"
#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/material/hcIMaterialFactory.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"
#include <limits>

namespace hc
{
  UInt16 MaterialManager::s_nextMaterialId = 0;

  MaterialManager::MaterialManager(
    IAssetManager& assetManager,
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
    SharedPtr<AMaterialDescriptor> mat = m_assetManager
      .getMaterialDescriptorAssetManager()
      .load(materialDescriptorPath);

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
    SharedPtr<AMaterialDescriptor> descriptor
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

    if (s_nextMaterialId == std::numeric_limits<UInt16>::max())
    {
      LogService::Error(
        String::Format(
          "Maximum number of materials (%u) exceeded; cannot allocate new material IDs.",
          static_cast<UInt32>(std::numeric_limits<UInt16>::max())
        )
      );

      return nullptr;
    }

    IMaterialFactory& materialFactor = m_materialFactoriesManager->getFactory(
      descriptor->getShaderType()
    );

    SharedPtr<IMaterial> material = materialFactor.create(
      s_nextMaterialId++,
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
