#include "hc/hcMaterialManager.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcAssetManager.h"
#include "hc/hcIGraphicsManager.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcUnlitMaterial.h"
#include "hc/hcLogService.h"

namespace hc
{
  MaterialManager::MaterialManager()
  {
  }

  MaterialManager::~MaterialManager()
  {
  }

  void MaterialManager::resolveDependencies(DependencyContainer& container)
  {
    m_assetManager = container.resolve<AssetManager>();
    m_graphicsManager = container.resolve<IGraphicsManager>();
  }

  SharedPtr<IMaterial> MaterialManager::createMaterial(
    const String& key,
    const String& materialDescriptorKey
  )
  {
    if (m_assetManager->contains<MaterialDescriptor>(materialDescriptorKey))
    {
      SharedPtr<MaterialDescriptor> descriptor =
        m_assetManager->get<MaterialDescriptor>(materialDescriptorKey);
      return createMaterial(key, descriptor);
    }

    LogService::Error(
      String::Format(
        "MaterialDescriptor with key '%s' not found.",
        materialDescriptorKey.c_str()
      )
    );

    return nullptr;
  }

  SharedPtr<IMaterial> MaterialManager::createMaterial(
    const String& key,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    switch (descriptor->getShaderType())
    {
    case shaderType::Unlit:
    {
      SharedPtr<UnlitMaterialDescriptor> unlitDescriptor =
        std::static_pointer_cast<UnlitMaterialDescriptor>(descriptor);
      return createUnlitMaterial(key, unlitDescriptor);
    }

    default:
    {
      String shaderTypeStr = shaderType::toString(descriptor->getShaderType());
      LogService::Error(
        String::Format(
          "Unsupported shader type '%s' in MaterialDescriptor for key '%s'.",
          shaderTypeStr.c_str(),
          key.c_str()
        )
      );

      return nullptr;
    }
    }
  }

  SharedPtr<UnlitMaterial> MaterialManager::createUnlitMaterial(
    const String& key,
    SharedPtr<UnlitMaterialDescriptor> descriptor
  )
  {
    SharedPtr<ITexture> mainTexture = nullptr;
    if (!descriptor->getMainImageKey().empty())
    {
      // TODO create texture from graphics
    }

    SharedPtr<UnlitMaterial> material = MakeShared<UnlitMaterial>();
    material->initialize(descriptor, mainTexture);
    m_materials[key] = material;

    return material;
  }

  SharedPtr<IMaterial> MaterialManager::get(const String& key) const
  {
    auto it = m_materials.find(key);
    if (it != m_materials.end())
      return it->second;

    return nullptr;
  }

  bool MaterialManager::contains(const String& key) const
  {
    return m_materials.find(key) != m_materials.end();
  }

  const UnorderedMap<String, SharedPtr<IMaterial>>& MaterialManager::getMaterials() const
  {
    return m_materials;
  }

  void MaterialManager::clear()
  {
    m_materials.clear();
  }
}
