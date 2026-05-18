#include "hc/assets/materialDescriptor/hcBlinnPhongMaterialDescriptor.h"

namespace hc
{
  BlinnPhongMaterialDescriptor::BlinnPhongMaterialDescriptor(
    const Path& path,
    const Color& color,
    const Path& colorImagePath,
    const Path& normalImagePath,
    const Path& specularImagePath
  ) :
    AMaterialDescriptor(path),
    m_color(color),
    m_albedoImagePath(colorImagePath),
    m_normalImagePath(normalImagePath),
    m_specularImagePath(specularImagePath)
  {}

  shadingType::Type BlinnPhongMaterialDescriptor::getShaderType() const
  {
    return shadingType::BlinnPhong;
  }

  void BlinnPhongMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_albedoImagePath.empty())
      paths.push_back(m_albedoImagePath);
    if (!m_normalImagePath.empty())
      paths.push_back(m_normalImagePath);
    if (!m_specularImagePath.empty())
      paths.push_back(m_specularImagePath);
  }

  const Color& BlinnPhongMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  const Path& BlinnPhongMaterialDescriptor::getAlbedoImagePath() const
  {
    return m_albedoImagePath;
  }

  const Path& BlinnPhongMaterialDescriptor::getNormalImagePath() const
  {
    return m_normalImagePath;
  }

  const Path& BlinnPhongMaterialDescriptor::getSpecularImagePath() const
  {
    return m_specularImagePath;
  }
}
