#include "hc/assets/materialDescriptor/hcHairMaterialDescriptor.h"

namespace hc
{
  HairMaterialDescriptor::HairMaterialDescriptor(
    const Path& path,
    const String& name,
    const Color& color,
    float shininess,
    const Path& albedoImagePath,
    const Path& normalImagePath,
    const Path& specularImagePath
  ) :
    AMaterialDescriptor(path, name),
    m_color(color),
    m_shininess(Math::Clamp(shininess, 1.0f, 256.0f)),
    m_albedoImagePath(albedoImagePath),
    m_normalImagePath(normalImagePath),
    m_specularImagePath(specularImagePath)
  {
  }

  materialType::Type HairMaterialDescriptor::getType() const
  {
    return materialType::Hair;
  }

  void HairMaterialDescriptor::getImagesPaths(Vector<Path>& paths) const
  {
    if (!m_albedoImagePath.empty())
      paths.push_back(m_albedoImagePath);
    if (!m_normalImagePath.empty())
      paths.push_back(m_normalImagePath);
    if (!m_specularImagePath.empty())
      paths.push_back(m_specularImagePath);
  }

  const Color& HairMaterialDescriptor::getColor() const
  {
    return m_color;
  }

  float HairMaterialDescriptor::getShininess() const
  {
    return m_shininess;
  }

  const Path& HairMaterialDescriptor::getAlbedoImagePath() const
  {
    return m_albedoImagePath;
  }

  const Path& HairMaterialDescriptor::getNormalImagePath() const
  {
    return m_normalImagePath;
  }

  const Path& HairMaterialDescriptor::getSpecularImagePath() const
  {
    return m_specularImagePath;
  }
}
