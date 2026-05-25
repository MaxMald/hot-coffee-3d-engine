#include "hc/graphics/resource/material/hcMaterialManager.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/graphics/resource/shaderProgram/hcShaderProgramManager.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/texture/hcITextureManager.h"
#include "hc/graphics/resource/material/hcUnlitMaterial.h"
#include "hc/graphics/resource/material/hcBlinnPhongMaterial.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"
#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"
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
    m_textureManager(textureManager),
    m_whiteTexture(nullptr),
    m_defaultNormalTexture(nullptr)
  {}

  void MaterialManager::initialize()
  {
    createDefaultTextures();
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
    SharedPtr<ITexture> mainTexture = getTextureFromPath(descriptor.getMainImagePath());
    if (!mainTexture)
      mainTexture = m_whiteTexture;

    SharedPtr<UnlitMaterial> material = MakeShared<UnlitMaterial>(generateMaterialId());
    material->initialize(
      descriptor,
      m_shaderProgramManager.getUnlitShaderProgram(),
      mainTexture
    );

    m_materials.push_back(material);
    return material;
  }

  SharedPtr<BlinnPhongMaterial> MaterialManager::createBlinnPhongMaterial(
    const BlinnPhongMaterialDescriptor& descriptor
  )
  {
    SharedPtr<ITexture> albedoTexture = getTextureFromPath(descriptor.getAlbedoImagePath());
    if (!albedoTexture)
      albedoTexture = m_whiteTexture;

    SharedPtr<ITexture> normalTexture = getTextureFromPath(descriptor.getNormalImagePath());
    if (!normalTexture)
      normalTexture = m_defaultNormalTexture;

    SharedPtr<ITexture> specularTexture = getTextureFromPath(descriptor.getSpecularImagePath());
    if (!specularTexture)
      specularTexture = m_whiteTexture;

    SharedPtr<BlinnPhongMaterial> material = MakeShared<BlinnPhongMaterial>(generateMaterialId());
    material->initilize(
      descriptor,
      albedoTexture,
      normalTexture,
      specularTexture,
      m_shaderProgramManager.getBlinnPhongForwardProgram()
    );

    m_materials.push_back(material);
    return material;
  }

  const Vector<SharedPtr<IMaterial>>& MaterialManager::getMaterials() const
  {
    return m_materials;
  }

  const SharedPtr<ITexture>& MaterialManager::getDefaultAlbedoTexture() const
  {
    return m_whiteTexture;
  }

  const SharedPtr<ITexture>& MaterialManager::getDefaultNormalTexture() const
  {
    return m_defaultNormalTexture;
  }

  const SharedPtr<ITexture>& MaterialManager::getDefaultSpecularTexture() const
  {
    return m_whiteTexture;
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
      throw RuntimeErrorException(
        String::Format(
          "Maximum number of materials (%u) exceeded; cannot allocate new material IDs.",
          static_cast<UInt32>(std::numeric_limits<UInt16>::max())
        )
      );
    }

    return s_nextMaterialId++;
  }

  void MaterialManager::createDefaultTextures()
  {
    m_whiteTexture = m_textureManager.createTexture();
    m_whiteTexture->initialize(1, 1, 4, Color::White()); // 1x1 white texture
    coreAssertions::AssertTextureIsValid(m_whiteTexture, "Default white texture");

    m_defaultNormalTexture = m_textureManager.createTexture();
    m_defaultNormalTexture->initialize(1, 1, 4, Color(0.5f, 0.5f, 1.0f, 1.0f)); // 1x1 normal texture (0.5, 0.5, 1.0)
    coreAssertions::AssertTextureIsValid(m_defaultNormalTexture, "Default normal texture");
  }

  SharedPtr<ITexture> MaterialManager::getTextureFromPath(const Path& texturePath)
  {
    if (texturePath.empty())
      return nullptr;

    SharedPtr<ITexture> texture = m_textureManager.createTextureFromFile(texturePath);
    if (!texture)
    {
      LogService::Error(
        String::Format(
          "Failed to load texture from path '%s'.",
          texturePath.string().c_str()
        )
      );

      return nullptr;
    }

    if (!texture->isValid())
    {
      LogService::Error(
        String::Format(
          "Texture from path '%s' is invalid.",
          texturePath.string().c_str()
        )
      );

      return nullptr;
    }

    return texture;
  }
}
