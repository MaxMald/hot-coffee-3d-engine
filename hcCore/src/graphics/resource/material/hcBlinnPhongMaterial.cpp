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
    m_albedoTexture(nullptr),
    m_normalTexture(nullptr),
    m_specularTexture(nullptr),
    m_shaderProgram(nullptr)
  {}

  BlinnPhongMaterial::~BlinnPhongMaterial()
  {}

  void BlinnPhongMaterial::destroy()
  {
    m_albedoTexture.reset();
    m_normalTexture.reset();
    m_specularTexture.reset();
    m_shaderProgram.reset();
  }

  shadingType::Type BlinnPhongMaterial::getShaderType() const
  {
    return shadingType::Type::BlinnPhong;
  }

  void BlinnPhongMaterial::bind(
    const CameraRenderData& cameraRenderData,
    renderPassType::Type renderPass
  )
  {
    assertIsValid();

    if (renderPassType::Type::Forward != renderPass)
    {
      // NOTE:
      // For now, Blinn-Phong material only supports forward rendering pass.

      // TODO:
      // Implement support for deferred rendering pass.

      throw InvalidArgumentException(
        "Blinn-Phong material only supports forward rendering pass."
      );
    }

    // TODO:
    // Send light data to shader.

    throw RuntimeErrorException(
      "Not implemented."
    );

    coreAssertions::AssertShaderProgramIsValid(m_shaderProgram, "Blinn-Phong shader program");
    coreAssertions::AssertTextureIsValid(m_albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(m_normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(m_specularTexture, "Specular");

    m_shaderProgram->bind();

    m_shaderProgram->setUniform("uProjection", cameraRenderData.projectionMatrix);
    m_shaderProgram->setUniform("uView", cameraRenderData.viewMatrix);
    m_shaderProgram->setUniform("uColor", getColor());
    m_shaderProgram->setUniform("uCameraPosition", cameraRenderData.cameraWorldPosition);

    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      m_shaderProgram->setUniform("uAlphaCutoff", m_alphaCutoutThreshold);
    else
      m_shaderProgram->setUniform("uAlphaCutoff", 0.0f);
        
    m_albedoTexture->bind(0);
    m_shaderProgram->setUniformTexture("uAlbedo", 0);
    m_normalTexture->bind(1);
    m_shaderProgram->setUniformTexture("uNormalMap", 1);
    m_specularTexture->bind(2);
    m_shaderProgram->setUniformTexture("uSpecularMap", 2);
  }

  void BlinnPhongMaterial::updateModelMatrix(const Matrix4 & modelMatrix)
  {
    m_shaderProgram->setUniform("uModel", modelMatrix);
  }

  void BlinnPhongMaterial::unbind()
  {
    // TODO
  }

  bool BlinnPhongMaterial::isValid() const
  {
    return m_shaderProgram != nullptr && m_shaderProgram->isValid();
  }

  void BlinnPhongMaterial::initialize(
    const BlinnPhongMaterialDescriptor& descriptor,
    const SharedPtr<ITexture>& albedoTexture,
    const SharedPtr<ITexture>& normalTexture,
    const SharedPtr<ITexture>& specularTexture,
    const SharedPtr<IShaderProgram>& shaderProgram
  )
  {
    if (!shaderProgram)
      throw InvalidArgumentException(
        "Shader program cannot be null when initializing Blinn-Phong material."
      );

    coreAssertions::AssertTextureIsValid(albedoTexture, "Albedo");
    coreAssertions::AssertTextureIsValid(normalTexture, "Normal");
    coreAssertions::AssertTextureIsValid(specularTexture, "Specular");

    m_color = descriptor.getColor();
    m_alphaCutoutThreshold = descriptor.getAlphaCutoutThreshold();
    m_doubleSided = descriptor.isDoubleSided();
    m_renderMode = descriptor.getRenderMode();

    m_albedoTexture = albedoTexture;
    m_normalTexture = normalTexture;
    m_specularTexture = specularTexture;
    m_shaderProgram = shaderProgram;
  }

  const Color& BlinnPhongMaterial::getColor() const
  {
    return m_color;
  }

  void BlinnPhongMaterial::setColor(const Color& color)
  {
    m_color = color;
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
