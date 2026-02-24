#include "hc/assets/hcAsset.h"

namespace hc
{
  Asset::Asset() :
    m_path(),
    m_id(Id::Create())
  {
  }

  Asset::~Asset()
  {
  }

  const Path& Asset::getPath() const
  {
    return m_path;
  }

  Id Asset::getId() const
  {
    return m_id;
  }
}
