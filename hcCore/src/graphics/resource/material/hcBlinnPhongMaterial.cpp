#include "hc/graphics/resource/material/hcBlinnPhongMaterial.h"

#include "hc/utilities/hcCoreAssertions.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/hcCameraMatrices.h"
#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"

namespace hc
{
  BlinnPhongMaterial::BlinnPhongMaterial(UInt16 materialId) :
    AMaterial(materialId, materialRenderMode::Type::Opaque, 0.0f, false),
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

  shadingType::Type BlinnPhongMaterial::getShaderType() const
  {
    return shadingType::Type::BlinnPhong;
  }

  void BlinnPhongMaterial::bind(renderPassType::Type renderPass)
  {
    assertIsValid();

    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_specularTexture, "Specular");

    if (renderPassType::Type::Forward == renderPass)
      bindForwardPass();
    else if (renderPassType::Type::DeferredGeometry == renderPass)
      bindDeferredGeometryPass();
    else
      throw InvalidArgumentException(
        String::Format(
          "Unsupported render pass type for Blinn-Phong material: %d",
          static_cast<Int32>(renderPass)
        )
      );
  }

  void BlinnPhongMaterial::updateModelMatrix(
    const Matrix4& modelMatrix,
    renderPassType::Type renderPass
  )
  {
    if (renderPassType::Type::Forward == renderPass)
    {
      coreAssertions::AssertShaderProgramIsValid(
        m_forwardShaderProgram,
        "Blinn-Phong forward shader program"
      );

      m_forwardShaderProgram->setUniform("uModel", modelMatrix);
    }      
    else if (renderPassType::Type::DeferredGeometry == renderPass)
    {
      coreAssertions::AssertShaderProgramIsValid(
        m_deferredGeometryShaderProgram,
        "Blinn-Phong deferred geometry shader program"
      );

      m_deferredGeometryShaderProgram->setUniform("uModel", modelMatrix);
    }
    else
      throw InvalidArgumentException(
        String::Format(
          "Unsupported render pass type for Blinn-Phong material: %d",
          static_cast<Int32>(renderPass)
        )
      );
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

  void BlinnPhongMaterial::bindForwardPass()
  {
    coreAssertions::AssertShaderProgramIsValid(
      m_forwardShaderProgram,
      "Blinn-Phong forward shader program"
    );

    m_forwardShaderProgram->bind();

    m_forwardShaderProgram->setUniform("uColor", m_color);
    m_forwardShaderProgram->setUniform("uShininess", m_shininess);

    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      m_forwardShaderProgram->setUniform("uAlphaCutoff", m_alphaCutoutThreshold);
    else
      m_forwardShaderProgram->setUniform("uAlphaCutoff", 0.0f);

    m_albedoTexture->bind(0);
    m_forwardShaderProgram->setUniformTexture("uAlbedo", 0);
    m_normalTexture->bind(1);
    m_forwardShaderProgram->setUniformTexture("uNormalMap", 1);
    m_specularTexture->bind(2);
    m_forwardShaderProgram->setUniformTexture("uSpecularMap", 2);
  }

  void BlinnPhongMaterial::bindDeferredGeometryPass()
  {
    coreAssertions::AssertShaderProgramIsValid(
      m_deferredGeometryShaderProgram,
      "Blinn-Phong deferred geometry shader program"
    );

    m_deferredGeometryShaderProgram->bind();

    m_deferredGeometryShaderProgram->setUniform("uColor", m_color);
    m_deferredGeometryShaderProgram->setUniform("uShininess", m_shininess);

    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      m_deferredGeometryShaderProgram->setUniform("uAlphaCutoff", m_alphaCutoutThreshold);
    else
      m_deferredGeometryShaderProgram->setUniform("uAlphaCutoff", 0.0f);

    m_albedoTexture->bind(0);
    m_deferredGeometryShaderProgram->setUniformTexture("uAlbedo", 0);
    m_normalTexture->bind(1);
    m_deferredGeometryShaderProgram->setUniformTexture("uNormalMap", 1);
    m_specularTexture->bind(2);
    m_deferredGeometryShaderProgram->setUniformTexture("uSpecularMap", 2);
  }
}
