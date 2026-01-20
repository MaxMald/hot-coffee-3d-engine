#include "hc/hcUnlitMaterialDescriptor.h"

namespace hc
{
  UnlitMaterialDescriptor::UnlitMaterialDescriptor() :
    MaterialDescriptor(shaderType::Unlit),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_mainImageKey()
  {
  }

  UnlitMaterialDescriptor::~UnlitMaterialDescriptor()
  {
  }

  void UnlitMaterialDescriptor::setColor(const Color& color)
  {
    m_color = color;
  }

  const Color& UnlitMaterialDescriptor::getColor()
  {
    return m_color;
  }

  void UnlitMaterialDescriptor::setMainImageKey(const String& key)
  {
    m_mainImageKey = key;
  }

  const String& UnlitMaterialDescriptor::getMainImageKey()
  {
    return m_mainImageKey;
  }
}
