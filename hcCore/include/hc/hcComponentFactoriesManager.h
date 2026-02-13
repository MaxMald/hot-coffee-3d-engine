#pragma once

#include "hc/hcATypedComponentFactory.h"

namespace hc
{
  /**
   * @brief Manages registration and retrieval of component factories.
   *
   * The ComponentFactoriesManager is responsible for registering, storing, and
   * providing access to factories that create component instances.
   *
   * @note This class is non-copyable.
   */
  class ComponentFactoriesManager : public NonCopyable
  {
  public:
    ComponentFactoriesManager() = default;
    ~ComponentFactoriesManager() = default;

    /**
     * @brief Registers a factory for a specific component type.
     *
     * Transfers ownership of the factory to the manager. If a factory for the
     * type already exists, it will be replaced.
     *
     * @tparam ComponentType The type of component the factory creates.
     * 
     * @param factory Unique pointer to the factory instance.
     *
     * @throws InvalidArgumentException if the factory pointer is null.
     */
    template <typename ComponentType>
    void registerFactory(UniquePtr<ATypedComponentFactory<ComponentType>> factory);

    /**
     * @brief Retrieves the factory for a specific component type.
     *
     * @tparam ComponentType The type of component.
     *
     * @return Pointer to the typed factory, or nullptr if not found.
     *
     * @throws RuntimeErrorException if the factory exists but is null or of the
     * wrong type.
     */
    template <typename ComponentType>
    ATypedComponentFactory<ComponentType>* getFactory() const;

    /**
     * @brief Checks if a factory for the specified component type is registered.
     *
     * @tparam ComponentType The type of component.
     * 
     * @return True if a factory is registered, false otherwise.
     */
    template <typename ComponentType>
    bool hasFactory() const;

    /**
     * @brief Creates a new component instance using the registered factory.
     *
     * @tparam ComponentType The type of component to create.
     *
     * @return Unique pointer to the created component instance.
     *
     * @throws RuntimeErrorException if the component type is not default
     * constructible and no factory is registered, or if the factory fails to
     * create an instance.
     */
    template <typename ComponentType>
    UniquePtr<ComponentType> createComponent() const;

    /**
     * @brief Removes all registered factories and clears the manager.
     */
    void clear();

  private:
    /**
     * @brief Internal map of factories, keyed by component type.
     */
    UnorderedMap<TypeIndex, UniquePtr<IComponentFactory>> m_factories;

    /**
     * @brief Creates a component using its default constructor.
     *
     * Used as a fallback if no factory is registered.
     *
     * @tparam ComponentType The type of component.
     * 
     * @return Unique pointer to the created component.
     *
     * @throws RuntimeErrorException if the component type is not default
     * constructible.
     */
    template <typename ComponentType>
    UniquePtr<ComponentType> createWithDefaultConstructor() const;
  };

  template <typename ComponentType>
  void ComponentFactoriesManager::registerFactory(
    UniquePtr<ATypedComponentFactory<ComponentType>> factory
  )
  {
    if (!factory)
      throw InvalidArgumentException("Factory pointer cannot be null.");

    m_factories[typeid(ComponentType)] = std::move(factory);
  }

  template <typename ComponentType>
  ATypedComponentFactory<ComponentType>* ComponentFactoriesManager::getFactory() const
  {
    auto it = m_factories.find(typeid(ComponentType));
    if (it == m_factories.end())
      return nullptr;

    if (it->second == nullptr)
      throw RuntimeErrorException("Desired factory is null");

    ATypedComponentFactory<ComponentType>* typedFactory =
      dynamic_cast<ATypedComponentFactory<ComponentType>*>(it->second.get());

    if (!typedFactory)
      throw RuntimeErrorException("Factory type mismatch for requested component type.");

    return typedFactory;
  }

  template <typename ComponentType>
  bool ComponentFactoriesManager::hasFactory() const
  {
    return m_factories.find(typeid(ComponentType)) != m_factories.end();
  }

  template <typename ComponentType>
  UniquePtr<ComponentType> ComponentFactoriesManager::createComponent() const
  {
    ATypedComponentFactory<ComponentType>* factory = getFactory<ComponentType>();
    if (!factory)
      return createWithDefaultConstructor<ComponentType>();

    UniquePtr<ComponentType> component = factory->create();
    if (!component)
      throw RuntimeErrorException("Factory failed to create component instance.");

    return component;
  }

  template <typename ComponentType>
  UniquePtr<ComponentType> ComponentFactoriesManager::createWithDefaultConstructor() const
  {
    if constexpr (std::is_default_constructible_v<ComponentType>)
    {
      return MakeUnique<ComponentType>();
    }
    else
    {
      String typeName = typeid(ComponentType).name();
      throw RuntimeErrorException(
        String::Format(
          "Component type '%s' is not default constructible and does not have an associated component factory.",
          typeName.c_str()
        )
      );
    }
  }
}
