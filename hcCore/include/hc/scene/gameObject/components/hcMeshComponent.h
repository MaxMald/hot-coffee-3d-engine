#pragma once

#include "hc/scene/gameObject/components/hcABaseComponent.h"
#include "hc/graphics/hcIDrawable.h"

namespace hc
{
  class IMesh;
  class IMeshManager;
  class IAssetManager;

  /**
   * Component that holds and manages a mesh for rendering.
   * 
   * MeshComponent provides the ability to attach a mesh to a game object,
   * making it renderable in the scene. It implements both the component
   * interface and the drawable interface for rendering integration.
   */
  class HC_CORE_EXPORT MeshComponent : 
    public ABaseComponent,
    public IDrawable
  {
  public:
    /**
     * Constructs a mesh component with no mesh attached.
     */
    MeshComponent(IMeshManager& meshManager, IAssetManager& assetManager);

    ~MeshComponent() override;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    /**
     * Draws the mesh using the provided render context.
     * 
     * @param renderContext The rendering context containing camera and
     *                      scene information.
     */
    void draw(const RenderContext& renderContext) override;

    /**
     * Sets the mesh to be rendered by this component.
     * 
     * @param mesh Shared pointer to the mesh to render.
     */
    void setMesh(SharedPtr<IMesh> mesh);

    /**
     * Gets the mesh currently attached to this component.
     * 
     * @return Shared pointer to the current mesh, or nullptr if no mesh
     *         is attached.
     */
    SharedPtr<IMesh> getMesh() const;

  private:
    SharedPtr<IMesh> m_mesh;
    IMeshManager& m_meshManager;
    IAssetManager& m_assetManager;
  };
}
