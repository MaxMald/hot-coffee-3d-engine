#pragma once

#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/scene/gameObject/components/factories/hcATypedComponentFactory.h"

namespace hc
{
  class IMeshManager;

  /**
   * Factory for creating mesh component instances.
   *
   * Provides a specialized factory for creating MeshComponent objects
   * with access to the mesh manager for loading and managing meshes.
   */
  class MeshComponentFactory : public ATypedComponentFactory<MeshComponent>
  {
  public:
    /**
     * @brief Constructs a mesh component factory.
     *
     * @param meshManager Reference to the mesh manager for loading and
     *                    managing mesh resources.
     */
    MeshComponentFactory(IMeshManager& meshManager);
    ~MeshComponentFactory() override = default;

    /**
     * Creates a new mesh component instance.
     *
     * @return A unique pointer to the newly created mesh component.
     */
    UniquePtr<MeshComponent> create() const override;

  private:
    IMeshManager& m_meshManager;
  };
}
