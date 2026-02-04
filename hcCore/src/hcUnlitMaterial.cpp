#include "hc/hcUnlitMaterial.h"
#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcIShaderProgram.h"
#include "hc/hcCameraMatrices.h"
#include "hc/hcITexture.h"

namespace hc
{
  UnlitMaterial::UnlitMaterial()
  {
  }

  UnlitMaterial::~UnlitMaterial()
  {
  }

  shadingType::Type UnlitMaterial::getShaderType() const
  {
    return shadingType::Unlit;
  }

  void UnlitMaterial::bind(const CameraMatrices& cameraMatrices)
  {
    m_shaderProgram->bind();

    m_shaderProgram->setUniform("uProjection", cameraMatrices.projectionMatrix);
    m_shaderProgram->setUniform("uView", cameraMatrices.viewMatrix);
    m_shaderProgram->setUniform("uColor", getColor());

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
  }

  void UnlitMaterial::initialize(
    const SharedPtr<UnlitMaterialDescriptor>& descriptor, 
    const SharedPtr<ITexture>& mainTexture
  )
  {
    m_descriptor = descriptor;
    m_mainTexture = mainTexture;
  }

  const Color& UnlitMaterial::getColor() const
  {
    if (m_descriptor)
      return m_descriptor->getColor();
    else
      return Color::White();
  }

  const SharedPtr<ITexture>& UnlitMaterial::getMainTexture() const
  {
    return m_mainTexture;
  }

  const SharedPtr<UnlitMaterialDescriptor> UnlitMaterial::getDescriptor() const
  {
    return m_descriptor;
  }

}
