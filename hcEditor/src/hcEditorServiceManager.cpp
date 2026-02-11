#include "hc/editor/hcEditorServiceManager.h"

namespace hc::editor
{
  void EditorServiceManager::clear()
  {
    const UnorderedMap<TypeIndex, UniquePtr<IEditorService>>& services =
      m_serviceManager.getAllServices();

    for (const auto& [typeIndex, service] : services)
      service->destroy();

    m_serviceManager.clear();
  }
}
