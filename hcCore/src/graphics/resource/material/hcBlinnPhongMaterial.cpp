#include "hc/graphics/resource/material/hcBlinnPhongMaterial.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"
#include "hc/graphics/resource/dataBlock/hcIDataBlockManager.h"
#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"

namespace hc
{
  BlinnPhongMaterial::BlinnPhongMaterial(UInt16 materialId) :
    AMaterial(materialId, "No Name", materialRenderMode::Type::Opaque, 0.0f, false),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_shininess(16.0f),
    m_albedoTexture(nullptr),
    m_normalTexture(nullptr),
    m_specularTexture(nullptr),
    m_forwardShaderProgram(nullptr),
    m_deferredGeometryShaderProgram(nullptr)
  {}

  BlinnPhongMaterial::~BlinnPhongMaterial()
  {}

  void BlinnPhongMaterial::destroy()
  {
    m_albedoTexture.reset();
    m_normalTexture.reset();
    m_specularTexture.reset();
    m_forwardShaderProgram.reset();
    m_deferredGeometryShaderProgram.reset();
  }

  materialType::Type BlinnPhongMaterial::getMaterialType() const
  {
    return materialType::Type::BlinnPhong;
  }

  void BlinnPhongMaterial::bind(
    renderPassType::Type renderPass,
    IDataBlockManager& dataBlockManager
  )
  {
    assertIsValid();

    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_specularTexture, "Specular");
    coreAssertions::AssertShaderProgramIsValid(
      m_forwardShaderProgram,
      "Blinn-Phong forward shader program"
    );
    coreAssertions::AssertShaderProgramIsValid(
      m_deferredGeometryShaderProgram,
      "Blinn-Phong deferred geometry shader program"
    );

    // Bind the appropriate shader program based on the render pass type
    if (renderPassType::Type::Forward == renderPass
      || renderPassType::Type::ForwardTransparent == renderPass)
    {
      m_forwardShaderProgram->bind();
    }
    else if (renderPassType::Type::DeferredGeometry == renderPass)
    {
      m_deferredGeometryShaderProgram->bind();
    }
    else
    {
      throw InvalidArgumentException(
        String::Format(
          "Unsupported render pass type for Blinn-Phong material: %d",
          static_cast<Int32>(renderPass)
        )
      );
    }

    // Bind textures
    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
    m_specularTexture->bind(2);

    // Upload and bind material properties
    dataBlockStructure::MaterialBlinnPhong materialData;
    materialData.color = m_color;
    materialData.shininess = m_shininess;
    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      materialData.alphaCutoff = m_alphaCutoutThreshold;
    else
      materialData.alphaCutoff = 0.0f;

    dataBlockManager.upload(dataBlockType::Type::MaterialBlinnPhong, &materialData);
    dataBlockManager.bind(dataBlockType::Type::MaterialBlinnPhong);
  }

  void BlinnPhongMaterial::unbind()
  {
    // TODO
  }

  bool BlinnPhongMaterial::isValid() const
  {
    return m_forwardShaderProgram != nullptr && m_forwardShaderProgram->isValid() &&
           m_deferredGeometryShaderProgram != nullptr && m_deferredGeometryShaderProgram->isValid();
  }

  void BlinnPhongMaterial::initialize(
    const BlinnPhongMaterialDescriptor& descriptor,
    const SharedPtr<ITexture>& albedoTexture,
    const SharedPtr<ITexture>& normalTexture,
    const SharedPtr<ITexture>& specularTexture,
    const SharedPtr<IShaderProgram>& forwardShaderProgram,
    const SharedPtr<IShaderProgram>& deferredGeometryShaderProgram  
  )
  {
    if (!forwardShaderProgram)
      throw InvalidArgumentException(
        "Forward shader program cannot be null when initializing Blinn-Phong material."
      );

    if (!deferredGeometryShaderProgram)
      throw InvalidArgumentException(
        "Deferred geometry shader program cannot be null when initializing Blinn-Phong material."
      );

    coreAssertions::AssertTextureIsValid(albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(specularTexture, "Specular");

    m_color = descriptor.getColor();
    m_name = descriptor.getName();
    m_shininess = descriptor.getShininess();
    m_alphaCutoutThreshold = descriptor.getAlphaCutoutThreshold();
    m_doubleSided = descriptor.isDoubleSided();
    m_renderMode = descriptor.getRenderMode();

    m_albedoTexture = albedoTexture;
    m_normalTexture = normalTexture;
    m_specularTexture = specularTexture;
    m_forwardShaderProgram = forwardShaderProgram;
    m_deferredGeometryShaderProgram = deferredGeometryShaderProgram;
  }

  const Color& BlinnPhongMaterial::getColor() const
  {
    return m_color;
  }

  void BlinnPhongMaterial::setColor(const Color& color)
  {
    m_color = color;
  }

  float BlinnPhongMaterial::getShininess() const
  {
    return m_shininess;
  }

  void BlinnPhongMaterial::setShininess(float shininess)
  {
    m_shininess = Math::Clamp(shininess, 1.0f, 256.0f);
  }

  const SharedPtr<ITexture>& BlinnPhongMaterial::getAlbedoTexture() const
  {
    return m_albedoTexture;
  }

  void BlinnPhongMaterial::setAlbedoTexture(const SharedPtr<ITexture>& albedoTexture)
  {
    m_albedoTexture = albedoTexture;
  }

  const SharedPtr<ITexture>& BlinnPhongMaterial::getNormalTexture() const
  {
    return m_normalTexture;
  }

  void BlinnPhongMaterial::setNormalTexture(const SharedPtr<ITexture>& normalTexture)
  {
    m_normalTexture = normalTexture;
  }

  const SharedPtr<ITexture>& BlinnPhongMaterial::getSpecularTexture() const
  {
    return m_specularTexture;
  }

  void BlinnPhongMaterial::setSpecularTexture(const SharedPtr<ITexture>& specularTexture)
  {
    m_specularTexture = specularTexture;
  }

  void BlinnPhongMaterial::assertIsValid() const
  {
    if (!isValid())
      throw RuntimeErrorException(
        "Blinn-Phong material is not valid."
      );
  }
}
