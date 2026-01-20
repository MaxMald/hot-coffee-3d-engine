#include "hc/hcUnlitMaterial.h"
#include "hc/hcUnlitMaterialDescriptor.h"

namespace hc
{
  UnlitMaterial::UnlitMaterial()
  {
  }

  UnlitMaterial::~UnlitMaterial()
  {
  }

  shaderType::Type UnlitMaterial::getShaderType() const
  {
    return shaderType::Unlit;
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
    return m_descriptor->getColor();
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
