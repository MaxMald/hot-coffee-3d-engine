#pragma once

#include "hc/hcIComponent.h"
#include "hc/hcIComponentFactory.h"

namespace hc
{
  /**
   * @brief Abstract factory for creating components of a specific type.
   *
   * Template base class for factories that create instances of a specific
   * component type. Ensures the component type derives from IComponent.
   *
   * @tparam ComponentType The type of component to create. Must derive from
   * IComponent.
   */
  template<typename ComponentType>
  class ATypedComponentFactory : public IComponentFactory
  {
    static_assert(std::derived_from<ComponentType, IComponent>);

  public:
    virtual ~ATypedComponentFactory() = default;

    /**
     * @brief Creates a new instance of the component type.
     * @return Unique pointer to the newly created component instance.
     */
    virtual UniquePtr<ComponentType> create() const = 0;

  protected:
    ATypedComponentFactory() = default;
  };
}
