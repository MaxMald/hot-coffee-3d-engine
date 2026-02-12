#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcATypedComponentFactory.h"

namespace hc
{
  /**
   * @brief Manages registration and creation of component factories.
   *
   * Responsible for registering component factories, creating components using
   * registered factories, and checking the existence of factories for specific
   * component types.
   */
  class HC_CORE_EXPORT ComponentFactoriesManager : public NonCopyable
  {
  public:
    ComponentFactoriesManager() = default;
    ~ComponentFactoriesManager() = default;

    /**
     * @brief Creates a component of the specified type.
     *
     * If a factory is registered for the type, it is used to create the
     * component. Otherwise, attempts to create the component using its default
     * constructor.
     *
     * @tparam ComponentType The type of component to create.
     * 
     * @return UniquePtr<ComponentType> The created component instance.
     * 
     * @throws RuntimeErrorException If the factory is null or not derived from
     *         ATypedComponentFactory, or if the component type is not default
     *         constructible.
     */
    template<typename ComponentType>
    UniquePtr<ComponentType> createComponent() const
    {
      static_assert(
        std::derived_from<ComponentType, IComponent>,
        "ComponentType must be derived from IComponent"
        );

      if (!hasComponentFactory<ComponentType>())
        return createWithDefaultConstructor<ComponentType>();

      IComponentFactory* factoryPtr = getComponentFactory<ComponentType>();
      if (!factoryPtr)
      {
        throw RuntimeErrorException(
          "Component factory for type: " + String(typeid(ComponentType).name()) + " is null."
        );
      }

      ATypedComponentFactory<ComponentType>* typedFactory 
        = dynamic_cast<ATypedComponentFactory<ComponentType>*>(factoryPtr);

      if (!typedFactory)
      {
        throw RuntimeErrorException(
          "Registered component factory for type: " + String(typeid(ComponentType).name()) + " does not derive from ATypedComponentFactory."
        );
      }

      return typedFactory->createComponent();
    }

    /**
     * @brief Registers a factory for a specific component type.
     *
     * @tparam ComponentType The type of component for which the factory is registered.
     * @param factory Unique pointer to the factory instance.
     * @throws InvalidArgumentException If the factory is null.
     */
    template<typename ComponentType>
    void registerComponentFactory(UniquePtr<ATypedComponentFactory<ComponentType>> factory)
    {
      static_assert(
        std::derived_from<ComponentType, IComponent>,
        "ComponentType must be derived from IComponent"
      );

      if (!factory)
      {
        throw InvalidArgumentException(
          "Factory cannot be null when registering a component factory."
        );
      }

      TypeIndex typeIndex = TypeIndex::get<ComponentType>();
      m_componentFactories[typeIndex] = std::move(factory);
    }

    /**
     * @brief Checks if a factory exists for the specified component type.
     *
     * @tparam ComponentType The type of component to check.
     * @return true if a factory is registered, false otherwise.
     */
    template<typename ComponentType>
    bool hasComponentFactory() const
    {
      TypeIndex typeIndex = TypeIndex::get<ComponentType>();
      return m_componentFactories.contains(typeIndex);
    }

    /**
     * @brief Clears all registered component factories.
     */
    void clearFactories()
    {
      m_componentFactories.clear();
    }

  private:
    /**
     * @brief Stores registered component factories indexed by their type.
     */
    UnorderedMap<TypeIndex, UniquePtr<IComponentFactory>> m_componentFactories;

    /**
     * @brief Creates a component using its default constructor.
     *
     * @tparam ComponentType The type of component to create.
     * @return UniquePtr<ComponentType> The created component instance.
     * @throws RuntimeErrorException If the component type is not default constructible.
     */
    template<typename ComponentType>
    UniquePtr<ComponentType> createWithDefaultConstructor()
    {
      static_assert(
        std::derived_from<ComponentType, IComponent>,
        "ComponentType must be derived from IComponent"
        );

      if constexpr (!std::is_default_constructible_v<ComponentType>)
      {
        throw RuntimeErrorException(
          "Component type: " + std::string(typeid(ComponentType).name()) + " is not default constructible. Cannot create component without a registered factory."
        );
      }
      else
      {
        return MakeUnique<ComponentType>();
      }
    }

    template<typename ComponentType>
    IComponentFactory* getComponentFactory() const
    {
      TypeIndex typeIndex = TypeIndex::get<ComponentType>();
      auto it = m_componentFactories.find(typeIndex);
      if (it == m_componentFactories.end())
      {
        return nullptr;
      }

      return it->second.get();
    }
  };
}
