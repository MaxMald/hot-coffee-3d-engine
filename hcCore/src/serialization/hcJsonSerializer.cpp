#include "hc/serialization/hcJsonSerializer.h"

#include "hc/serialization/hcMaterialDescriptorJsonSerializer.h"
#include "hc/assets/hcMaterialDescriptor.h"

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
