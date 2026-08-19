#include "hc/serialization/hcJsonSerializer.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  void JsonSerializer::onPrepare()
  {
  }

  void JsonSerializer::onShutdown()
  {
    m_serializers.clear();
  }
}
