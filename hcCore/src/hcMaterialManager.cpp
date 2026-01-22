#include "hc/hcMaterialManager.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcAssetManager.h"
#include "hc/hcTextureManager.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcUnlitMaterial.h"
#include "hc/hcLogService.h"
#include "hc/hcImage.h"

namespace hc
{
  void MaterialManager::resolveDependencies(DependencyContainer& container)
  {
    m_assetManager = container.resolve<AssetManager>();
    m_textureManager = container.resolve<TextureManager>();
  }

  SharedPtr<IMaterial> MaterialManager::createMaterialFromFile(
    const Path& materialDescriptorPath
  )
  {
    SharedPtr<MaterialDescriptor> mat = m_assetManager->loadDirect<MaterialDescriptor>(
      materialDescriptorPath
    );

    return createMaterialFromDescriptor(mat);
  }

  SharedPtr<IMaterial> MaterialManager::createMaterialFromDescriptor(
    const String& materialDescriptorKey
  )
  {
    SharedPtr<MaterialDescriptor> mat = m_assetManager->get<MaterialDescriptor>(
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

    String cacheKey = getCacheKey(descriptor);
    auto it = m_cacheMaterials.find(cacheKey);
    if (it != m_cacheMaterials.end())
      return it->second;

    shaderType::Type shaderType = descriptor->getShaderType();
    if (shaderType == shaderType::Type::Unlit)
    {
      return createUnlitMaterial(cacheKey, descriptor);
    }

    String shaderTypeStr = shaderType::toString(descriptor->getShaderType());
    LogService::Error(
      String::Format(
        "Unsupported shader type '%s' in MaterialDescriptor for key.",
        shaderTypeStr.c_str()
      )
    );

    return nullptr;
  }

  String MaterialManager::getCacheKey(
    SharedPtr<MaterialDescriptor> descriptor
  ) const
  {
    if (!descriptor)
      return String();

    return descriptor->getPath().string();
  }

  SharedPtr<IMaterial> MaterialManager::get(const String& key) const
  {
    auto it = m_cacheMaterials.find(key);
    if (it != m_cacheMaterials.end())
      return it->second;

    return nullptr;
  }

  bool MaterialManager::contains(const String& key) const
  {
    return m_cacheMaterials.find(key) != m_cacheMaterials.end();
  }

  const UnorderedMap<String, SharedPtr<IMaterial>>& MaterialManager::getCacheMaterials() const
  {
    return m_cacheMaterials;
  }

  void MaterialManager::clear()
  {
    m_cacheMaterials.clear();
  }

  SharedPtr<UnlitMaterial> MaterialManager::createUnlitMaterial(
    const String& cacheKey,
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    SharedPtr<UnlitMaterialDescriptor> unlitDescriptor =
      std::static_pointer_cast<UnlitMaterialDescriptor>(descriptor);

    if (!unlitDescriptor)
    {
      LogService::Error(String::Format("Invalid UnlitMaterialDescriptor provided."));
      return nullptr;
    }

    SharedPtr<ITexture> mainTexture = nullptr;
    if (!unlitDescriptor->getMainImagePath().empty())
    {
      mainTexture = m_textureManager->createTextureFromFile(
        unlitDescriptor->getMainImagePath()
      );
    }

    SharedPtr<UnlitMaterial> material = MakeShared<UnlitMaterial>();
    material->initialize(unlitDescriptor, mainTexture);

    m_cacheMaterials[cacheKey] = material;
    return material;
  }
}
