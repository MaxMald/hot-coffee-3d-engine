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
     * @brief Registers an instance of type T, constructed with the given arguments.
     *
     * @tparam T Type to register.
     * @tparam Args Constructor argument types.
     * @param args Arguments forwarded to T's constructor.
     */
    template<typename T, typename... Args>
    void registerType(Args&&... args)
    {
      auto instance = MakeShared<T>(std::forward<Args>(args)...);
      m_instances[typeid(T)] = instance;
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

      return nullptr;
    }

    /**
     * @brief Calls resolveDependencies on all registered resolvable objects.
     * Only objects derived from IDependencyResolvable are affected.
     */
    void resolveAllDependencies()
    {
      for (auto& [type, instance] : m_instances)
      {
        auto resolvable = std::dynamic_pointer_cast<IDependencyResolvable>(instance);

        if (resolvable)
          resolvable->resolveDependencies(*this);
      }
    }

  private:
    UnorderedMap<TypeIndex, SharedPtr<void>> m_instances;
  };
}
