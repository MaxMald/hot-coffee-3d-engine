#include "hc/graphics/resource/material/hcPBRMaterial.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"
#include "hc/graphics/resource/dataBlock/hcIDataBlockManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"

namespace hc
{
  PBRMaterial::PBRMaterial(UInt16 materialIndex) :
    AMaterial(materialIndex, "No Name", materialRenderMode::Type::Opaque, 0.0f, false),
    m_sourcePath(),
    m_baseColor(Color::White()),
    m_metallic(0.0f),
    m_roughness(0.0f),
    m_ior(1.5f),
    m_albedoTexture(nullptr),
    m_normalTexture(nullptr),
    m_omrTexture(nullptr),
    m_deferredGeometryShaderProgram(nullptr),
    m_forwardShaderProgram(nullptr)
  {
  }

  PBRMaterial::~PBRMaterial()
  {
  }

  void PBRMaterial::destroy()
  {
    m_sourcePath.clear();
    m_albedoTexture.reset();
    m_normalTexture.reset();
    m_omrTexture.reset();
    m_deferredGeometryShaderProgram.reset();
    m_forwardShaderProgram.reset();
  }

  materialType::Type PBRMaterial::getMaterialType() const
  {
    return materialType::Type::PBR;
  }

  void PBRMaterial::bind(
    renderPassType::Type renderPass,
    IDataBlockManager& dataBlockManager
  )
  {
    assertIsValid();
    if (renderPass == renderPassType::Type::DeferredGeometry)
    {
      bindForDeferredGeometryPass(dataBlockManager);
      return;
    }
    else if (renderPass == renderPassType::Type::Forward)
    {
      bindForForwardPass(dataBlockManager);
      return;
    }
    else
    {
      throw InvalidArgumentException(
        String::Format(
          "Unsupported render pass type for PBR material: {}",
          static_cast<Int32>(renderPass)
        )
      );
    }
  }

  void PBRMaterial::unbind()
  {
    // TODO: Implement unbind logic for PBRMaterial if necessary
  }

  bool PBRMaterial::isValid() const
  {
    return m_deferredGeometryShaderProgram != nullptr && m_deferredGeometryShaderProgram->isValid();
  }

  void PBRMaterial::initialize(
    const MaterialDescriptor& descriptor,
    const SharedPtr<ITexture>& albedoTexture,
    const SharedPtr<ITexture>& normalTexture,
    const SharedPtr<ITexture>& omrTexture,
    const SharedPtr<IShaderProgram>& deferredGeometryShaderProgram,
    const SharedPtr<IShaderProgram>& forwardShaderProgram
  )
  {
    coreAssertions::AssertShaderProgramIsValid(
      deferredGeometryShaderProgram,
      "PBR deferred geometry shader program"
    );
    coreAssertions::AssertShaderProgramIsValid(
      forwardShaderProgram,
      "PBR forward shader program"
    );
    coreAssertions::AssertTextureIsValid(albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(omrTexture, "OMR");

    const assets::materialDescriptor::PBRData* pbrData = descriptor.getIfPBRData();
    if (pbrData == nullptr)
      throw InvalidArgumentException("Material descriptor does not contain PBR data.");

    m_sourcePath = descriptor.path;
    m_name = descriptor.name;
    setAlphaCutoutThreshold(descriptor.alphaCutoutThreshold);
    m_doubleSided = descriptor.doubleSided;
    m_renderMode = descriptor.renderMode;
    m_baseColor = pbrData->baseColor;
    setMetallic(pbrData->metallic);
    setRoughness(pbrData->roughness);
    setIOR(pbrData->ior);
    m_albedoTexture = albedoTexture;
    m_normalTexture = normalTexture;
    m_omrTexture = omrTexture;
    m_deferredGeometryShaderProgram = deferredGeometryShaderProgram;
    m_forwardShaderProgram = forwardShaderProgram;
  }

  void PBRMaterial::assertIsValid() const
  {
    if (!isValid())
      throw RuntimeErrorException(
        "PBRMaterial is not valid. Ensure that the material is properly initialized."
      );
  }

  void PBRMaterial::bindForDeferredGeometryPass(IDataBlockManager& dataBlockManager)
  {
    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_omrTexture, "OMR");
    coreAssertions::AssertShaderProgramIsValid(
      m_deferredGeometryShaderProgram,
      "PBR deferred geometry shader program"
    );

    m_deferredGeometryShaderProgram->bind();

    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
    m_omrTexture->bind(2);

    dataBlockStructure::MaterialPBR materialData;
    materialData.baseColor = m_baseColor;
    materialData.metallic = m_metallic;
    materialData.roughness = m_roughness;
    materialData.ior = m_ior;
    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      materialData.alphaCutoff = m_alphaCutoutThreshold;
    else
      materialData.alphaCutoff = 0.0f;

    dataBlockManager.upload(dataBlockType::Type::MaterialPBR, &materialData);
  }

  void PBRMaterial::bindForForwardPass(IDataBlockManager& dataBlockManager)
  {
    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_omrTexture, "OMR");
    coreAssertions::AssertShaderProgramIsValid(
      m_forwardShaderProgram,
      "PBR forward shader program"
    );

    m_forwardShaderProgram->bind();

    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
    m_omrTexture->bind(2);

    dataBlockStructure::MaterialPBR materialData;
    materialData.baseColor = m_baseColor;
    materialData.metallic = m_metallic;
    materialData.roughness = m_roughness;
    materialData.ior = m_ior;
    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      materialData.alphaCutoff = m_alphaCutoutThreshold;
    else
      materialData.alphaCutoff = 0.0f;

    dataBlockManager.upload(dataBlockType::Type::MaterialPBR, &materialData);
  }
}
