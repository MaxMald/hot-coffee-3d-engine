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

    /**
     * @copydoc IDrawable::draw
     */
    void draw(
      const RenderContext& renderContext,
      Vector<DrawCommand>& drawCommands
    ) const override;

    /**
     * Sets the mesh to be rendered by this component.
     *
     * @param mesh Shared pointer to the mesh to render.
     * @param sourcePath Optional path to the source file from which the mesh was loaded.
     * This is used for serialization purposes. Could be absolute or relative to the asset
     * manager's root path.
     */
    void setMesh(SharedPtr<IMesh> mesh, const Path& sourcePath = "");

    /**
     * Gets the mesh currently attached to this component.
     *
     * @return Shared pointer to the current mesh, or nullptr if no mesh
     *         is attached.
     */
    inline SharedPtr<IMesh> getMesh() const
    {
      return m_mesh;
    }

    /**
     * Gets the source path of the mesh, if it was loaded from a file.
     *
     * @return The path to the source file of the mesh, or an empty path
     *         if the mesh was not loaded from a file.
     */
    inline const Path& getSourcePath() const
    {
      return m_sourcePath;
    }

    /**
     * Clears the mesh from this component, removing any attached mesh.
     */
    void clear();

  protected:
    void onSerialize(io::BinaryWriter& writer) const override;
    void onDeserialize(io::BinaryReader& reader) override;
    UInt16 getDerivedVersion() const override;

  private:
    Path m_sourcePath;
    SharedPtr<IMesh> m_mesh;
    IMeshManager& m_meshManager;
    IAssetManager& m_assetManager;
  };
}
