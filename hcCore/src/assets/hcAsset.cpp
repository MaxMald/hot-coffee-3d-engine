#include "hc/assets/hcAsset.h"

namespace hc
{
  Asset::Asset(const Path& path) :
    m_path(path),
    m_id(Id::Create())
  {
  }

  const Path& Asset::getPath() const
  {
    return m_path;
  }

  void Asset::setPath(const Path& path)
  {
    m_path = path;
  }

  Id Asset::getId() const
  {
    return m_id;
  }
}
