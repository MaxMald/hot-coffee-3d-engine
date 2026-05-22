#include "hc/graphics/resource/material/hcMaterialManager.h"
#include "hc/graphics/resource/shaderProgram/hcShaderProgramManager.h"
#include "hc/graphics/resource/material/hcMaterialFactoriesManager.h"
#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/material/hcIMaterialFactory.h"
#include "hc/graphics/resource/material/hcUnlitMaterial.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"
#include <limits>

namespace hc
{
  UInt16 MaterialManager::s_nextMaterialId = 0;

  MaterialManager::MaterialManager(
    IAssetManager& assetManager,
    ITextureManager& textureManager,
    IShaderProgramManager& shaderProgramManager
  ) :
    m_assetManager(assetManager),
    m_shaderProgramManager(shaderProgramManager),
    m_textureManager(textureManager)
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

    const shadingType::Type shaderType = descriptor->getShaderType();

    if (shaderType == shadingType::Unlit)
    {
      const UnlitMaterialDescriptor* unlitDescriptor =
        dynamic_cast<const UnlitMaterialDescriptor*>(descriptor.get());

      if (!unlitDescriptor)
      {
        LogService::Error(
          String::Format(
            "Failed to cast MaterialDescriptor to UnlitMaterialDescriptor for shader type 'Unlit'."
          )
        );
        return nullptr;
      }

      return createUnlitMaterial(*unlitDescriptor);
    }

    throw RuntimeErrorException(
      String::Format(
        "Not implemented shader type '%u' in MaterialDescriptor; cannot create material.",
        static_cast<UInt32>(shaderType)
      )
    );
  }

  SharedPtr<UnlitMaterial> MaterialManager::createUnlitMaterial(
    const UnlitMaterialDescriptor& descriptor
  )
  {
    SharedPtr<ITexture> mainTexture = nullptr;
    if (!descriptor.getMainImagePath().empty())
    {
      mainTexture = m_textureManager.createTextureFromFile(descriptor.getMainImagePath());
      if (!mainTexture)
      {
        LogService::Error(
          String::Format(
            "Failed to load main texture for UnlitMaterial from path '%s'.",
            descriptor.getMainImagePath().string().c_str()
          )
        );
      }
    }

    SharedPtr<UnlitMaterial> material = MakeShared<UnlitMaterial>(generateMaterialId());
    material->initialize(
      descriptor,
      m_shaderProgramManager.getUnlitShaderProgram(),
      mainTexture
    );

    m_materials.push_back(material);
    return material;
  }

  const Vector<SharedPtr<IMaterial>>& MaterialManager::getMaterials() const
  {
    return m_materials;
  }

  void MaterialManager::clear()
  {
    for (const auto& material : m_materials)
      material->destroy();
    m_materials.clear();
  }

  UInt16 MaterialManager::generateMaterialId()
  {
    if (s_nextMaterialId == std::numeric_limits<UInt16>::max())
    {
      LogService::Error(
        String::Format(
          "Maximum number of materials (%u) exceeded; cannot allocate new material IDs.",
          static_cast<UInt32>(std::numeric_limits<UInt16>::max())
        )
      );
      return 0;
    }

    return s_nextMaterialId++;
  }
}
