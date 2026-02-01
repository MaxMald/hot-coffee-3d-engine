#include "hc/hcMaterialDescriptor.h"

namespace hc
{
  MaterialDescriptor::MaterialDescriptor(shaderType::Type type) :
    m_shaderType(type),
    m_id(Id::Create())
  {
  }

  shaderType::Type MaterialDescriptor::getShaderType() const
  {
    return m_shaderType;
  }

  Id MaterialDescriptor::getId() const
  {
    return m_id;
  }
}
