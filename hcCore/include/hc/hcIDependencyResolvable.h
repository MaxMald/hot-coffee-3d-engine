#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class DependencyContainer;

  /**
   * @brief Interface for objects that require dependency resolution.
   *
   * Implement this interface in classes that need their dependencies resolved
   * via the dependency container.
   */
  class HC_CORE_EXPORT IDependencyResolvable
  {
  public:
    virtual ~IDependencyResolvable() = default;

    /**
     * @brief Resolves dependencies for the object using the provided container.
     * @param container Reference to the dependency container.
     */
    virtual void resolveDependencies(DependencyContainer& container) = 0;
  };
}
