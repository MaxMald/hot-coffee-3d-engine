#include "hc/hcJsonSerializer.h"

#include "hc/hcMaterialDescriptorJsonSerializer.h"
#include "hc/hcMaterialDescriptor.h"

namespace hc
{
  void JsonSerializer::onPrepare()
  {
    registerSerializer<MaterialDescriptor>(
      MakeUnique<MaterialDescriptorJsonSerializer>()
    );
  }

  void JsonSerializer::onShutdown()
  {
    m_serializers.clear();
  }
}
