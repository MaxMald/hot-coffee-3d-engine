#include "hc/hcOpenGlMaterialManager.h"

namespace hc
{
  OpenGlMaterialManager::OpenGlMaterialManager() :
    m_textureManager(nullptr),
    m_assetManager(nullptr)
  {
  }

  OpenGlMaterialManager::~OpenGlMaterialManager()
  {
  }

  SharedPtr<IMaterial> OpenGlMaterialManager::createMaterialFromFile(
    const Path& materialDescriptorPath
  )
  {
    SharedPtr<MaterialDescriptor> mat = m_assetManager->load<MaterialDescriptor>(
      materialDescriptorPath
    );

    return createMaterialFromDescriptor(mat);
  }

  SharedPtr<IMaterial> OpenGlMaterialManager::createMaterialFromDescriptor(
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

  SharedPtr<IMaterial> OpenGlMaterialManager::createMaterialFromDescriptor(
    SharedPtr<MaterialDescriptor> descriptor
  )
  {
    if (!descriptor)
    {
      LogService::Error(
        String::Format("Invalid MaterialDescriptor provided.")
      );
      return m_defaultUnlitMaterial;
    }

    auto it = m_cachedMaterials.find(descriptor->getId());
    if (it != m_cachedMaterials.end())
      return it->second;

    shadingType::Type shaderType = descriptor->getShaderType();
    if (shaderType == shadingType::Type::Unlit)
    {
      return createUnlitMaterial(descriptor);
    }

    String shaderTypeStr = shadingType::toString(descriptor->getShaderType());
    LogService::Error(
      String::Format(
        "Unsupported shader type '%s' in MaterialDescriptor for key.",
        shaderTypeStr.c_str()
      )
    );

    return m_defaultUnlitMaterial;
  }

  void OpenGlMaterialManager::clear()
  {
    m_cachedMaterials.clear();
  }

  void OpenGlMaterialManager::initialize(
    SharedPtr<AssetManager> assetManager,
    ITextureManager* textureManager
  )
  {
    m_textureManager = textureManager;
    m_assetManager = assetManager;
    m_defaultUnlitMaterial = createUnlitMaterial(
      MakeShared<UnlitMaterialDescriptor>()
    );
  }

  SharedPtr<UnlitMaterial> OpenGlMaterialManager::createUnlitMaterial(
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

    m_cachedMaterials[descriptor->getId()] = material;
    return material;
  }
}
