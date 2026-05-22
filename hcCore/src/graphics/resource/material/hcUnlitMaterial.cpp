#include "hc/graphics/resource/material/hcUnlitMaterial.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"
#include "hc/graphics/hcCameraMatrices.h"
#include "hc/graphics/resource/texture/hcITexture.h"

namespace hc
{
  UnlitMaterial::UnlitMaterial(UInt16 materialId) :
    AMaterial(materialId, materialRenderMode::Type::Opaque, 0.0f, false),
    m_color(0.5f, 0.5f, 0.5f, 1.0f)
  {}

  UnlitMaterial::~UnlitMaterial()
  {}

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
    const CameraMatrices& cameraMatrices,
    renderPassType::Type renderPass
  )
  {
    if (renderPass != renderPassType::Type::Forward)
      throw RuntimeErrorException(
        "UnlitMaterial::bind - UnlitMaterial only supports Forward render pass."
      );

    if (!m_shaderProgram)
      throw RuntimeErrorException(
        "UnlitMaterial::bind - Shader program is not set for this material."
      );

    m_shaderProgram->bind();

    m_shaderProgram->setUniform("uProjection", cameraMatrices.projectionMatrix);
    m_shaderProgram->setUniform("uView", cameraMatrices.viewMatrix);
    m_shaderProgram->setUniform("uColor", getColor());

    if (m_renderMode == materialRenderMode::Type::AlphaCutout)
      m_shaderProgram->setUniform("uAlphaCutoff", m_alphaCutoutThreshold);
    else
      m_shaderProgram->setUniform("uAlphaCutoff", 0.0f);

    if (m_mainTexture)
    {
      m_shaderProgram->setUniform("uUseTexture", true);
      m_mainTexture->bind(0);
      m_shaderProgram->setUniformTexture("uTexture", 0);
    }
    else
    {
      // TODO Bind to a default white texture
      m_shaderProgram->setUniform("uUseTexture", false);
    }
  }

  void UnlitMaterial::updateModelMatrix(const Matrix4& modelMatrix)
  {
    m_shaderProgram->setUniform("uModel", modelMatrix);
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
    if (!shaderProgram)
      throw RuntimeErrorException(
        "UnlitMaterial::initialize - Shader program cannot be null."
      );

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
