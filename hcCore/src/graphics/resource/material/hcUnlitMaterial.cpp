#include "hc/graphics/resource/material/hcUnlitMaterial.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"
#include "hc/graphics/resource/dataBlock/hcIDataBlockManager.h"

namespace hc
{
  UnlitMaterial::UnlitMaterial(UInt16 materialId) :
    AMaterial(materialId, "No Name", materialRenderMode::Type::Opaque, 0.0f, false),
    m_color(0.5f, 0.5f, 0.5f, 1.0f)
  {
  }

  UnlitMaterial::~UnlitMaterial()
  {
  }

  void UnlitMaterial::destroy()
  {
    m_color = Color(0.5f, 0.5f, 0.5f, 1.0f);
    m_shaderProgram.reset();
    m_mainTexture.reset();
  }

  shadingType::Type UnlitMaterial::getShaderType() const
  {
    return shadingType::Unlit;
  }

  void UnlitMaterial::bind(
    renderPassType::Type renderPass,
    IDataBlockManager& dataBlockManager
  )
  {
    if (renderPass != renderPassType::Type::Forward)
      throw RuntimeErrorException(
        "UnlitMaterial::bind - UnlitMaterial only supports Forward render pass."
      );

    coreAssertions::AssertShaderProgramIsValid(m_shaderProgram, "Unlit shader program");
    coreAssertions::AssertTextureIsValid(m_mainTexture, "Main texture");

    m_shaderProgram->bind();

    // Upload and bind material properties
    dataBlockStructure::MaterialUnlit materialData;
    materialData.color = m_color;
    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      materialData.alphaCutoff = m_alphaCutoutThreshold;
    else
      materialData.alphaCutoff = 0.0f;
    dataBlockManager.upload(dataBlockType::Type::MaterialUnlit, &materialData);
    dataBlockManager.bind(dataBlockType::Type::MaterialUnlit);

    // Upload textures
    m_mainTexture->bind(0);
  }

  void UnlitMaterial::unbind()
  {
    // TODO unbind 
  }

  bool UnlitMaterial::isValid() const
  {
    return m_shaderProgram != nullptr && m_shaderProgram->isValid();
  }

  void UnlitMaterial::initialize(
    const UnlitMaterialDescriptor& descriptor,
    const SharedPtr<IShaderProgram>& shaderProgram,
    const SharedPtr<ITexture>& mainTexture
  )
  {
    coreAssertions::AssertShaderProgramIsValid(shaderProgram, "Unlit shader program");
    coreAssertions::AssertTextureIsValid(mainTexture, "Main texture");

    m_name = descriptor.getName();
    m_renderMode = descriptor.getRenderMode();
    m_doubleSided = descriptor.isDoubleSided();
    setAlphaCutoutThreshold(descriptor.getAlphaCutoutThreshold());
    m_color = descriptor.getColor();
    m_shaderProgram = shaderProgram;
    m_mainTexture = mainTexture;
  }

  const Color& UnlitMaterial::getColor() const
  {
    return m_color;
  }

  void UnlitMaterial::setColor(const Color& color)
  {
    m_color = color;
  }

  const SharedPtr<ITexture>& UnlitMaterial::getMainTexture() const
  {
    return m_mainTexture;
  }

  void UnlitMaterial::setMainTexture(const SharedPtr<ITexture>& mainTexture)
  {
    m_mainTexture = mainTexture;
  }
}
