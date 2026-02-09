#include "hc/hcABaseGraphicResource.h"

namespace hc
{
  const Id& ABaseGraphicResource::getId() const
  {
    return m_id;
  }

  ABaseGraphicResource::ABaseGraphicResource() :
    m_id(Id::Create())
  {
  }

  ABaseGraphicResource::ABaseGraphicResource(const Id& id) :
    m_id(id)
  {
  }
}
