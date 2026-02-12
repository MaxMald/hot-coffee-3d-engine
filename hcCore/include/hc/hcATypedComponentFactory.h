#pragma once

#include "hc/hcIComponent.h"
#include "hc/hcIComponentFactory.h"

namespace hc
{
  /**
   * @brief Abstract base class for typed component factories.
   *
   * Provides an interface for factories that create components of a specific
   * type. Ensures that only types derived from IComponent can be used.
   *
   * @tparam ComponentType The type of component this factory creates.
   */
  template<typename ComponentType>
  class ATypedComponentFactory : public IComponentFactory
  {
    static_assert(
      std::derived_from<ComponentType, IComponent>,
      "ComponentType must be derived from IComponent"
      );

  public:
    virtual ~ATypedComponentFactory() = default;

    /**
     * @brief Creates a new instance of the component.
     * 
     * @return UniquePtr<ComponentType> The created component instance.
     */
    virtual UniquePtr<ComponentType> createComponent() const = 0;

  protected:
    ATypedComponentFactory() = default;
  };
}
