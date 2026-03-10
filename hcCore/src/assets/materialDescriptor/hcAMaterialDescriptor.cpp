#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  materialRenderMode::Type AMaterialDescriptor::getRenderMode() const
  {
    return m_renderMode;
  }

  void AMaterialDescriptor::setRenderMode(materialRenderMode::Type renderMode)
  {
    m_renderMode = renderMode;
  }

  AMaterialDescriptor::AMaterialDescriptor(
    const Path& path,
    materialRenderMode::Type renderMode
  ) :
    Asset(path),
    m_renderMode(renderMode)
  {
  }
}
