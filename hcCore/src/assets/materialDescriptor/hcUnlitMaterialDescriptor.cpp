#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"

namespace hc
{
  UnlitMaterialDescriptor::UnlitMaterialDescriptor() :
    AMaterialDescriptor(""),
    m_color(0.5f, 0.5f, 0.5f, 1.0f),
    m_mainImagePath("")
  {}

  UnlitMaterialDescriptor::UnlitMaterialDescriptor(
    const Path& path,
    const String& name,
    const Color& color,
    const Path& mainImagePath
  ) :
    AMaterialDescriptor(path, name),
    m_color(color),
    m_mainImagePath(mainImagePath)
  {}

  materialType::Type UnlitMaterialDescriptor::getType() const
  {
    return materialType::Unlit;
  }

  void UnlitMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_mainImagePath.empty())
      paths.push_back(m_mainImagePath);
  }

  const Color& UnlitMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  const Path& UnlitMaterialDescriptor::getMainImagePath() const
  {
    return m_mainImagePath;
  }
}
