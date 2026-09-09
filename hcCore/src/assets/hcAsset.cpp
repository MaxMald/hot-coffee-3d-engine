#include "hc/assets/hcAsset.h"

namespace hc
{
  Asset::Asset() :
    path(""),
    m_uuid(UUID::Generate())
  {}

  Asset::Asset(const Path& path) :
    path(path),
    m_uuid(UUID::Generate())
  {
  }

  const UUID& Asset::getUUID() const
  {
    return m_uuid;
  }
}
