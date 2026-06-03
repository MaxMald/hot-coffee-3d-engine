#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlTextureManager;
  class OpenGlMaterialManager;

  /**
   * @brief Represents a mesh implementation for OpenGL rendering.
   */
  class OpenGlMesh : public IMesh
  {
  public:
    /**
     * @brief Constructs an OpenGlMesh with the given graphics manager.
     *
     * @param graphicsManager Reference to the graphics manager for resource management.
     */
    OpenGlMesh(IGraphicsManager& graphicsManager);
    ~OpenGlMesh() override;

    /**
     * @brief Returns the unique identifier of the mesh.
     *
     * @return Reference to the mesh Id.
     */
    const Id& getId() const override;

    /**
     * @brief Draws the mesh using the provided render context.
     *
     * @param renderContext The rendering context for the draw call.
     */
    void draw(const RenderContext& renderContext) override;

    /**
     * @copydoc IMesh::initialize(const Model&, const Vector<SharedPtr<IMaterial>>&)
     */
    void initialize(
      const Model& model,
      const Vector<SharedPtr<IMaterial>>& materials
    ) override;

    /**
     * @copydoc IMesh::initialize(const Buffer<Vertex>&, const BufferUInt32&, const Vector<SharedPtr<IMaterial>>&)
     */
    void initialize(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<SharedPtr<IMaterial>>& materials
    ) override;

    /**
     * @copydoc IMesh::initialize(const Buffer<Vertex>&, const BufferUInt32&, const Vector<ModelSubMesh>&, const Vector<SharedPtr<IMaterial>>&)
     */
    void initialize(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices,
      const Vector<ModelSubMesh>& subMeshes,
      const Vector<SharedPtr<IMaterial>>& materials
    ) override;

    /**
     * @copydoc IMesh::update(const Model&)
     */
    void update(const Model& model) override;

    /**
     * @copydoc IMesh::update(const Buffer<Vertex>&, const BufferUInt32&)
     */
    void update(const Buffer<Vertex>& vertices, const BufferUInt32& indices) override;

    /**
     * @brief Sets the material at the specified index for this mesh.
     *
     * @param index The index of the material to set.
     * @param material Shared pointer to the material to associate with this mesh.
     */
    void setMaterial(UInt32 index, SharedPtr<IMaterial> material) override;

    /**
     * @copydoc IMesh::getMaterial
     */
    SharedPtr<IMaterial> getMaterial(UInt32 index) const override;

    /**
     * @brief Returns the number of materials associated with this mesh.
     *
     * @return The number of materials.
     */
    SizeT getMaterialsSize() const override;

    /**
     * @brief Destroys the mesh and releases OpenGL resources.
     */
    void destroy() override;

    /**
     * @brief Returns the materials associated with this mesh.
     *
     * @return Vector of shared pointers to materials.
     */
    const Vector<SharedPtr<IMaterial>> getMaterials() override;

    /**
     * @copydoc IMesh::getDrawType
     */
    drawType::Type getDrawType() const override;

    /**
     * @copydoc IMesh::setDrawType
     */
    void setDrawType(drawType::Type drawType) override;

    /**
     * @copydoc IGraphicResource::isValid
     */
    bool isValid() const override;

    /**
     * @copydoc IGraphicResource::getSourcePath
     */
    Path getSourcePath() const override;

    /**
     * @copydoc IGraphicResource::setSourcePath
     */
    void setSourcePath(const Path& path) override;

    /**
     * @brief Binds the mesh's VAO for rendering.
     */
    void bind();

    /**
     * @brief Unbinds the mesh's VAO.
     */
    void unbind();

    /**
     * @brief Returns the OpenGL VAO handle.
     *
     * @return VAO identifier.
     */
    UInt32 getVao() const;

    /**
     * @brief Returns the OpenGL draw mode.
     *
     * @return Draw mode identifier.
     */
    UInt32 getDrawMode() const;

  private:
    Id m_id;
    bool m_valid;
    Path m_sourcePath;
    Vector<SharedPtr<IMaterial>> m_materials;
    Vector<ModelSubMesh> m_subMeshes;
    UInt32 m_vao;
    UInt32 m_vbo;
    UInt32 m_ebo;
    UInt32 m_drawMode;
    IGraphicsManager& m_graphicsManager;

    void assertIsValid() const;
    void createBuffers();
    void drawModelSubMesh(
      const RenderContext& renderContext,
      float distanceToCamera,
      const ModelSubMesh& submesh
    );
    void updateVertexAndIndexBuffers(
      const Buffer<Vertex>& vertices,
      const BufferUInt32& indices
    );
  };
}
