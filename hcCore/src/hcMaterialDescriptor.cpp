#include "hc/hcMaterialDescriptor.h"

namespace hc
{
  MaterialDescriptor::MaterialDescriptor(shadingType::Type type) :
    m_shaderType(type)
  {
  }

  shadingType::Type MaterialDescriptor::getShaderType() const
  {
    return m_shaderType;
  }
}
