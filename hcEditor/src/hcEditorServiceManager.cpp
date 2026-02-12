#include "hc/editor/hcEditorServiceManager.h"

namespace hc::editor
{
  void EditorServiceManager::update(const Time& elapsedTime)
  {
    for (IUpdatableEditorService* service : m_updatableServices)
      service->update(elapsedTime);
  }

  void EditorServiceManager::clear()
  {
    m_updatableServices.clear();

    const UnorderedMap<TypeIndex, UniquePtr<IEditorService>>& services =
      m_serviceManager.getAllServices();

    for (const auto& [typeIndex, service] : services)
      service->destroy();

    m_serviceManager.clear();
  }
}
