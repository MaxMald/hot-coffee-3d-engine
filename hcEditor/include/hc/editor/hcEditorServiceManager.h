#pragma once

#include "hc/editor/hcIEditorService.h"
#include "hc/editor/hcIUpdatableEditorService.h"

namespace hc::editor
{
  /**
   * @brief Manages the registration and retrieval of editor services.
   *
   * This class provides a type-safe interface for registering, accessing, and
   * clearing editor services. Services must inherit from IEditorService.
   */
  class EditorServiceManager : public NonCopyable
  {
  public:
    EditorServiceManager() = default;
    ~EditorServiceManager() = default;

    /**
     * @brief Registers a service of the specified type.
     *
     * @tparam ServiceType The concrete service type, must derive from
     * IEditorService.
     *
     * @param service Unique pointer to the service instance.
     */
    template<typename ServiceType>
    void registerService(UniquePtr<ServiceType> service);

    /**
     * @brief Retrieves a reference to the registered service of the specified
     * type.
     *
     * @tparam ServiceType The concrete service type.
     *
     * @return Reference to the service instance.
     *
     * @throws std::runtime_error if the service is not found.
     */
    template<typename ServiceType>
    ServiceType& getService();

    /**
     * @brief Checks if a service of the specified type is registered.
     *
     * @tparam ServiceType The concrete service type.
     *
     * @return True if the service exists, false otherwise.
     */
    template<typename ServiceType>
    bool hasService() const;

    /**
     * @brief Updates all registered services that implement the
     * IUpdatableEditorService interface.
     *
     * This method should be called once per frame or tick, allowing updatable
     * services to perform time-dependent operations.
     *
     * @param elapsedTime The time elapsed since the last update.
     */
    void update(const Time& elapsedTime);

    /**
     * @brief Removes all registered services.
     */
    void clear();

  private:
    ServiceManager<IEditorService> m_serviceManager;
    Vector<IUpdatableEditorService*> m_updatableServices;

    template<typename ServiceType>
    void tryAddUpdatableService(ServiceType* service);
  };

  template<typename ServiceType>
  void EditorServiceManager::registerService(
    UniquePtr<ServiceType> service
  )
  {
    if (!service)
      throw InvalidArgumentException("Service pointer cannot be null");

    if (m_serviceManager.hasService<ServiceType>())
    {
      LogService::Error(
        String::Format(
          "Service of type %s is already registered. Registration skipped.",
          typeid(ServiceType).name()
        )
      );

      return;
    }

    tryAddUpdatableService(service.get());

    m_serviceManager.registerService<ServiceType>(std::move(service));
  }

  template<typename ServiceType>
  ServiceType& EditorServiceManager::getService()
  {
    return m_serviceManager.getService<ServiceType>();
  }

  template<typename ServiceType>
  bool EditorServiceManager::hasService() const
  {
    return m_serviceManager.hasService<ServiceType>();
  }

  template<typename ServiceType>
  void EditorServiceManager::tryAddUpdatableService(ServiceType* service)
  {
    if (!service)
      return;

    IUpdatableEditorService* updatableService =
      dynamic_cast<IUpdatableEditorService*>(service);

    if (updatableService)
      m_updatableServices.push_back(updatableService);
  }
}
