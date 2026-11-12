#pragma once

#include "hc/scene/gameObject/components/factories/hcATypedComponentFactory.h"

namespace hc
{
  /**
   * @brief Default factory for creating components of a specific type.
   *
   * Provides a simple implementation of ATypedComponentFactory that creates
   * instances of the specified component type using its default constructor.
   *
   * @tparam ComponentType The type of component to create. Must derive from
   * IComponent.
   */
  template<typename ComponentType>
  class DefaultComponentFactory : public ATypedComponentFactory<ComponentType>
  {
  public:
    /**
     * @brief Constructs a default component factory.
     *
     * Initializes the factory for creating instances of the specified component
     * type.
     */
    DefaultComponentFactory() = default;

    /**
     * @brief Destroys the default component factory.
     */
    ~DefaultComponentFactory() override = default;

    /**
     * @brief Creates a new instance of the component type.
     *
     * @return Unique pointer to the newly created component instance.
     */
    UniquePtr<ComponentType> create() const override
    {
      return MakeUnique<ComponentType>();
    }
  };
}
