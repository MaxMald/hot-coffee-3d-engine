#include "hc/serialization/hcJsonSerializer.h"
#include "hc/serialization/hcMaterialDescriptorJsonSerializer.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  void JsonSerializer::onPrepare()
  {
    registerSerializer<AMaterialDescriptor>(
      MakeUnique<MaterialDescriptorJsonSerializer>()
    );
  }

  void JsonSerializer::onShutdown()
  {
    m_serializers.clear();
  }
}
