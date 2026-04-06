#include "hc/editor/services/hcEditorServiceManager.h"

namespace hc::editor
{
  void EditorServiceManager::prepareServices()
  {
    const UnorderedMap<TypeIndex, UniquePtr<IEditorService>>& services =
      m_serviceManager.getAllServices();

    for (const auto& [typeIndex, service] : services)
      service->prepare();
  }

  void EditorServiceManager::update(const Time& elapsedTime)
  {
    for (IUpdatableEditorService* service : m_updatableServices)
      service->update(elapsedTime);
  }

  void EditorServiceManager::destroy()
  {
    m_updatableServices.clear();

    const UnorderedMap<TypeIndex, UniquePtr<IEditorService>>& services =
      m_serviceManager.getAllServices();

    for (const auto& [typeIndex, service] : services)
      service->destroy();

    m_serviceManager.clear();
  }
}
