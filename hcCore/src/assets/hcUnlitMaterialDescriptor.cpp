#include "hc/assets/hcUnlitMaterialDescriptor.h"

namespace hc
{
  UnlitMaterialDescriptor::UnlitMaterialDescriptor() :
    MaterialDescriptor(shadingType::Unlit),
    m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_mainImagePath()
  {
  }

  UnlitMaterialDescriptor::~UnlitMaterialDescriptor()
  {
  }

  void UnlitMaterialDescriptor::setColor(const Color& color)
  {
    m_color = color;
  }

  const Color& UnlitMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  void UnlitMaterialDescriptor::setMainImagePath(const Path& path)
  {
    m_mainImagePath = path;
  }

  const Path& UnlitMaterialDescriptor::getMainImagePath() const
  {
    return m_mainImagePath;
  }

  Vector<Path> UnlitMaterialDescriptor::getImagesPaths() const
  {
    if (m_mainImagePath.empty()) 
    {
      return Vector<Path>();
    } else 
    {
      return Vector<Path>{ m_mainImagePath };
    }
  }
}
