#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  IGraphicResource::IGraphicResource() :
    m_uuid(UUID::Generate())
  {}

  IGraphicResource::~IGraphicResource()
  {}

  const UUID& IGraphicResource::getUUID() const
  {
    return m_uuid;
  }
}
