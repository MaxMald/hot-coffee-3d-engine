#include "hc/hcJsonSerializer.h"

#include "hc/hcMaterialDescriptorJsonSerializer.h"
#include "hc/hcMaterialDescriptor.h"

namespace hc
{
  hc::JsonSerializer* hc::JsonSerializer::s_instance = nullptr;

  JsonSerializer& JsonSerializer::Instance()
  {
    if (s_instance == nullptr)
    {
      throw RuntimeErrorException(
        "JsonSerializer instance is not prepared. Call JsonSerializer::Prepare() before accessing the instance."
      );
    }

    return *s_instance;
  }

  void JsonSerializer::Prepare()
  {
    if (s_instance == nullptr)
    {
      s_instance = new JsonSerializer();
      s_instance->onPrepare();
    }
  }

  void JsonSerializer::Shutdown()
  {
    if (s_instance != nullptr)
    {
      delete s_instance;
      s_instance = nullptr;
    }
  }

  JsonSerializer::JsonSerializer()
  {
  }

  void JsonSerializer::onPrepare()
  {
    registerSerializer<MaterialDescriptor>(
      MakeUnique<MaterialDescriptorJsonSerializer>()
    );
  }
}
