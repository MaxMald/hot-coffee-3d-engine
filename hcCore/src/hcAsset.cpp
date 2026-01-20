#include "hc/hcAsset.h"

namespace hc
{
  Asset::Asset(const String& key) :
    m_key(key),
    m_path()
  {
  }

  Asset::~Asset()
  {
  }

  const String& Asset::getKey() const
  {
    return m_key;
  }

  const Path& Asset::getPath() const
  {
    return m_path;
  }
}
