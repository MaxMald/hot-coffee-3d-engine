#include "hc/graphics/resource/material/hcHairMaterial.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/resource/dataBlock/hcDataBlockStructures.h"
#include "hc/graphics/resource/dataBlock/hcIDataBlockManager.h"
#include "hc/assets/materialDescriptor/hcHairMaterialDescriptor.h"

namespace hc
{
  HairMaterial::HairMaterial(UInt16 materialId) :
    AMaterial(materialId, "No Name", materialRenderMode::Type::Opaque, 0.0f, false),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_specularPrimaryColor(1.0f, 1.0f, 1.0f, 1.0f),
    m_specularSecondaryColor(1.0f, 1.0f, 1.0f, 1.0f),
    m_shininess(16.0f),
    m_specularPrimaryShift(0.0f),
    m_specularSecondaryShift(0.0f),
    m_specularWidth(1.0f),
    m_specularStrength(1.0f),
    m_albedoTexture(nullptr),
    m_normalTexture(nullptr),
    m_specularTexture(nullptr),
    m_deferredGeometryShaderProgram(nullptr),
    m_forwardSpecularShaderProgram(nullptr),
    m_forwardTransparentShaderProgram(nullptr)
  {
  }

  HairMaterial::~HairMaterial()
  {
  }

  void HairMaterial::destroy()
  {
    m_albedoTexture.reset();
    m_normalTexture.reset();
    m_specularTexture.reset();
    m_deferredGeometryShaderProgram.reset();
    m_forwardSpecularShaderProgram.reset();
    m_forwardTransparentShaderProgram.reset();
  }

  materialType::Type HairMaterial::getMaterialType() const
  {
    return materialType::Type::Hair;
  }

  void HairMaterial::bind(
    renderPassType::Type renderPass,
    IDataBlockManager& dataBlockManager
  )
  {
    assertIsValid();

    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_specularTexture, "Specular");

    // Bind the appropriate shader program based on the render pass type
    if (renderPassType::Type::DeferredGeometry == renderPass)
      bindDeferredGeometryPass();
    else if (renderPassType::Type::HairForwardSpecular == renderPass)
      bindForwardSpecularPass();
    else if (renderPassType::Type::ForwardTransparent == renderPass)
      bindForwardTransparentPass();
    else
      throw InvalidArgumentException(
        String::Format(
          "Unsupported render pass type for Hair material: %d",
          static_cast<Int32>(renderPass)
        )
      );

    // Upload and bind material properties
    dataBlockStructure::MaterialHair materialData;
    materialData.color = m_color.vec4;
    materialData.primarySpecularColor = m_specularPrimaryColor.vec4;
    materialData.secondarySpecularColor = m_specularSecondaryColor.vec4;

    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      materialData.alphaCutoff = m_alphaCutoutThreshold;
    else
      materialData.alphaCutoff = 0.0f;

    materialData.shininess = m_shininess;
    materialData.primarySpecularShift = m_specularPrimaryShift;
    materialData.secondarySpecularShift = m_specularSecondaryShift;
    materialData.specularWidth = m_specularWidth;
    materialData.specularStrength = m_specularStrength;
    dataBlockManager.upload(dataBlockType::Type::MaterialHair, &materialData);
    dataBlockManager.bind(dataBlockType::Type::MaterialHair);
  }

  void HairMaterial::unbind()
  {
    // TODO
  }

  bool HairMaterial::isValid() const
  {
    return m_forwardSpecularShaderProgram != nullptr
      && m_forwardSpecularShaderProgram->isValid()
      && m_forwardTransparentShaderProgram != nullptr
      && m_forwardTransparentShaderProgram->isValid()
      && m_deferredGeometryShaderProgram != nullptr
      && m_deferredGeometryShaderProgram->isValid();
  }

  void HairMaterial::initialize(
    const HairMaterialDescriptor& descriptor,
    const SharedPtr<ITexture>& albedoTexture,
    const SharedPtr<ITexture>& normalTexture,
    const SharedPtr<ITexture>& specularTexture,
    const SharedPtr<IShaderProgram>& deferredGeometryShaderProgram,
    const SharedPtr<IShaderProgram>& forwardSpecularProgram,
    const SharedPtr<IShaderProgram>& forwardTransparentProgram
  )
  {
    coreAssertions::AssertShaderProgramIsValid(deferredGeometryShaderProgram, "Hair deferred geometry shader program");
    coreAssertions::AssertShaderProgramIsValid(forwardSpecularProgram, "Hair forward specular shader program");
    coreAssertions::AssertShaderProgramIsValid(forwardTransparentProgram, "Hair forward transparent shader program");
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
    m_deferredGeometryShaderProgram = deferredGeometryShaderProgram;
    m_forwardSpecularShaderProgram = forwardSpecularProgram;
    m_forwardTransparentShaderProgram = forwardTransparentProgram;
  }

  const Color& HairMaterial::getColor() const
  {
    return m_color;
  }

  void HairMaterial::setColor(const Color& color)
  {
    m_color = color;
  }

  float HairMaterial::getSpecularStrength() const
  {
    return m_specularStrength;
  }

  void HairMaterial::setSpecularStrength(float strength)
  {
    m_specularStrength = Math::Clamp(strength, 0.0f, 1.0f);
  }

  const Color& HairMaterial::getSpecularPrimaryColor() const
  {
    return m_specularPrimaryColor;
  }

  void HairMaterial::setSpecularPrimaryColor(const Color& color)
  {
    m_specularPrimaryColor = color;
  }

  const Color& HairMaterial::getSpecularSecondaryColor() const
  {
    return m_specularSecondaryColor;
  }

  void HairMaterial::setSpecularSecondaryColor(const Color& color)
  {
    m_specularSecondaryColor = color;
  }

  float HairMaterial::getSpecularPrimaryShift() const
  {
    return m_specularPrimaryShift;
  }

  void HairMaterial::setSpecularPrimaryShift(float shift)
  {
    m_specularPrimaryShift = Math::Clamp(shift, -4.0f, 4.0f);
  }

  float HairMaterial::getSpecularSecondaryShift() const
  {
    return m_specularSecondaryShift;
  }

  void HairMaterial::setSpecularSecondaryShift(float shift)
  {
    m_specularSecondaryShift = Math::Clamp(shift, -4.0f, 4.0f);
  }

  float HairMaterial::getSpecularWidth() const
  {
    return m_specularWidth;
  }

  void HairMaterial::setSpecularWidth(float width)
  {
    m_specularWidth = Math::Clamp(width, 0.0f, 1.0f);
  }

  float HairMaterial::getShininess() const
  {
    return m_shininess;
  }

  void HairMaterial::setShininess(float shininess)
  {
    m_shininess = Math::Clamp(shininess, 1.0f, 256.0f);
  }

  const SharedPtr<ITexture>& HairMaterial::getAlbedoTexture() const
  {
    return m_albedoTexture;
  }

  void HairMaterial::setAlbedoTexture(const SharedPtr<ITexture>& albedoTexture)
  {
    m_albedoTexture = albedoTexture;
  }

  const SharedPtr<ITexture>& HairMaterial::getNormalTexture() const
  {
    return m_normalTexture;
  }

  void HairMaterial::setNormalTexture(const SharedPtr<ITexture>& normalTexture)
  {
    m_normalTexture = normalTexture;
  }

  const SharedPtr<ITexture>& HairMaterial::getSpecularTexture() const
  {
    return m_specularTexture;
  }

  void HairMaterial::setSpecularTexture(const SharedPtr<ITexture>& specularTexture)
  {
    m_specularTexture = specularTexture;
  }

  void HairMaterial::assertIsValid() const
  {
    if (!isValid())
      throw RuntimeErrorException(
        "Hair material is not valid."
      );
  }

  void HairMaterial::bindDeferredGeometryPass()
  {
    coreAssertions::AssertShaderProgramIsValid(
      m_deferredGeometryShaderProgram,
      "Hair deferred geometry shader program"
    );

    m_deferredGeometryShaderProgram->bind();

    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
  }

  void HairMaterial::bindForwardSpecularPass()
  {
    coreAssertions::AssertShaderProgramIsValid(
      m_forwardSpecularShaderProgram,
      "Hair forward specular shader program"
    );

    m_forwardSpecularShaderProgram->bind();

    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
    m_specularTexture->bind(2);
  }

  void HairMaterial::bindForwardTransparentPass()
  {
    coreAssertions::AssertShaderProgramIsValid(
      m_forwardTransparentShaderProgram,
      "Hair forward transparent shader program"
    );

    m_forwardTransparentShaderProgram->bind();

    m_albedoTexture->bind(0);
    m_normalTexture->bind(1);
  }
}
