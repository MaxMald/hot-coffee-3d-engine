#include "hc/hcUnlitMaterial.h"
#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcIShaderProgram.h"
#include "hc/hcCameraMatrices.h"
#include "hc/hcITexture.h"

namespace hc
{
  UnlitMaterial::UnlitMaterial() :
    m_id(Id::Create())
  {
  }

  UnlitMaterial::~UnlitMaterial()
  {
  }

  const Id& UnlitMaterial::getId() const
  {
    return m_id;
  }

  void UnlitMaterial::destroy()
  {
    m_shaderProgram.reset();
    m_descriptor.reset();
    m_mainTexture.reset();
  }

  shadingType::Type UnlitMaterial::getShaderType() const
  {
    return shadingType::Unlit;
  }

  void UnlitMaterial::bind(const CameraMatrices& cameraMatrices)
  {
    if (!m_shaderProgram)
      return;

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

  SharedPtr<MaterialDescriptor> UnlitMaterial::getDescriptor() const
  {
    return m_descriptor;
  }

  void UnlitMaterial::initialize(
    const SharedPtr<IShaderProgram>& shaderProgram,
    const SharedPtr<UnlitMaterialDescriptor>& descriptor,
    const SharedPtr<ITexture>& mainTexture
  )
  {
    m_shaderProgram = shaderProgram;
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
}
