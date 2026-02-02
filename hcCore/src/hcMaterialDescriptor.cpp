#include "hc/hcMaterialDescriptor.h"

namespace hc
{
  MaterialDescriptor::MaterialDescriptor(shaderType::Type type) :
    m_shaderType(type)
  {
  }

  shaderType::Type MaterialDescriptor::getShaderType() const
  {
    return m_shaderType;
  }
}
