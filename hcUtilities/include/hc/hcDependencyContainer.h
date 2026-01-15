#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcIDependencyResolvable.h"

namespace hc
{
  /**
   * @brief Simple dependency container for registering and resolving types.
   *
   * Allows registration of any type. Only classes derived from IDependencyResolvable
   * will have their dependencies resolved via resolveAllDependencies.
   */
  class DependencyContainer
  {
  public:
    DependencyContainer() = default;
    ~DependencyContainer() = default;

    /**
     * @brief Registers an instance of type T.
     * 
     * @tparam T Type to register.
     * 
     * @param instance SharedPtr<T> instance to register.
     */
    template<typename T>
    void registerInstance(const SharedPtr<T>& instance)
    {
      m_instances[typeid(T)] = instance;

      // If T is derived from IDependencyResolvable, also store in resolvables
      if constexpr (std::is_base_of_v<IDependencyResolvable, T>)
      {
        m_resolvables.push_back(
          std::static_pointer_cast<IDependencyResolvable>(instance)
        );
      }
    }

    /**
     * @brief Registers an instance as a specific interface type.
     *
     * @tparam Interface Interface type to register as.
     * @tparam T Concrete type of the instance.
     * 
     * @param instance SharedPtr<T> instance to register.
     */
    template<typename Interface, typename T>
    void registerInstanceAsInterface(const SharedPtr<T>& instance)
    {
      static_assert(std::is_base_of_v<Interface, T>, "T must derive from Interface");
      m_instances[typeid(Interface)] = std::static_pointer_cast<Interface>(instance);

      // If Interface is IDependencyResolvable, add to resolvables
      if constexpr (std::is_base_of_v<IDependencyResolvable, Interface>)
      {
        m_resolvables.push_back(std::static_pointer_cast<IDependencyResolvable>(instance));
      }
    }

    /**
     * @brief Resolves and returns the registered instance of type T.
     *
     * @tparam T Type to resolve.
     * @return SharedPtr<T> Registered instance, or nullptr if not found.
     */
    template<typename T>
    SharedPtr<T> resolve()
    {
      auto it = m_instances.find(typeid(T));
      if (it != m_instances.end())
        return std::static_pointer_cast<T>(it->second);

      throw RuntimeErrorException("Type not registered in DependencyContainer.");
    }

    /**
     * @brief Calls resolveDependencies on all registered resolvable objects.
     * Only objects derived from IDependencyResolvable are affected.
     */
    void resolveAllDependencies()
    {
      for (auto& resolvable : m_resolvables)
      {
        if (resolvable)
          resolvable->resolveDependencies(*this);
      }
    }

    /**
     * @brief Clears all registered instances and resolvables.
     */
    void clear()
    {
      m_instances.clear();
      m_resolvables.clear();
    }

  private:
    UnorderedMap<TypeIndex, SharedPtr<void>> m_instances;
    Vector<SharedPtr<IDependencyResolvable>> m_resolvables;
  };
}
