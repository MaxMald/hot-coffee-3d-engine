#include "hc/hcAsset.h"

namespace hc
{
  Asset::Asset() :
    m_path()
  {
  }

  Asset::~Asset()
  {
  }

  const Path& Asset::getPath() const
  {
    return m_path;
  }
}
