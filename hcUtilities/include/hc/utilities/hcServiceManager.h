#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"

namespace hc
{
  /**
   * @brief Manages the registration and retrieval of service instances by type.
   *
   * This class provides a type-safe mechanism for registering, accessing, and
   * clearing services. Services are owned by the manager using UniquePtr.
   *
   * @tparam BaseType The base type for all services managed.
   */
  template<typename BaseType>
  class ServiceManager
  {
  public:
    static_assert(
      std::has_virtual_destructor_v<BaseType>,
      "BaseType must have a virtual destructor"
    );

    ServiceManager() = default;
    virtual ~ServiceManager() = default;

    /**
     * @brief Registers a service of the specified concrete type.
     * 
     * @tparam ConcreteType The concrete service type.
     * 
     * @param service Unique pointer to the service instance.
     */
    template<typename ConcreteType>
    void registerService(UniquePtr<ConcreteType> service);

    /**
     * @brief Checks if a service of the specified concrete type is registered.
     * 
     * @tparam ConcreteType The concrete service type.
     * 
     * @return True if the service exists, false otherwise.
     */
    template<typename ConcreteType>
    bool hasService() const;

    /**
     * @brief Retrieves a reference to the registered service of the specified
     * type.
     * 
     * @tparam ConcreteType The concrete service type.
     * 
     * @return Reference to the service instance.
     * 
     * @throws std::runtime_error if the service is not found.
     */
    template<typename ConcreteType>
    ConcreteType& getService();

    /**
     * @brief Removes all registered services.
     */
    void clear()
    {
      m_services.clear();
    }

    /**
     * @brief Retrieves all registered services.
     */
    const UnorderedMap<TypeIndex, UniquePtr<BaseType>>& getAllServices() const
    {
      return m_services;
    }

  private:
    UnorderedMap<TypeIndex, UniquePtr<BaseType>> m_services;
  };

  template<typename BaseType>
  template<typename ConcreteType>
  void ServiceManager<BaseType>::registerService(UniquePtr<ConcreteType> service)
  {
    if (!service)
    {
      throw RuntimeErrorException(
        String::Format(
          "Cannot register a null service of type: %s.",
          typeid(ConcreteType).name()
        )
      );
    }

    static_assert(
      std::is_base_of_v<BaseType, ConcreteType>,
      "ConcreteType must derive from BaseType"
      );

    TypeIndex typeIndex(typeid(ConcreteType));
    m_services[typeIndex] = std::move(service);
  }

  template<typename BaseType>
  template<typename ConcreteType>
  bool ServiceManager<BaseType>::hasService() const
  {
    TypeIndex typeIndex(typeid(ConcreteType));
    return m_services.find(typeIndex) != m_services.end();
  }

  template<typename BaseType>
  template<typename ConcreteType>
  ConcreteType& ServiceManager<BaseType>::getService()
  {
    TypeIndex typeIndex(typeid(ConcreteType));
    auto it = m_services.find(typeIndex);
    if (it == m_services.end())
    {
      throw RuntimeErrorException(
        String::Format(
          "Service of the requested type: %s is not registered.",
          typeid(ConcreteType).name()
        )
      );
    }

    if (!it->second)
    {
      throw RuntimeErrorException(
        String::Format(
          "Service of the requested type: %s is null.",
          typeid(ConcreteType).name()
        )
      );
    }

    return *static_cast<ConcreteType*>(it->second.get());
  }
}
