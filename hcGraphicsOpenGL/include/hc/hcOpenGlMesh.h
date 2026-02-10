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
     * @brief Constructs an OpenGlMesh with the given model and materials.
     * 
     * @param model Shared pointer to the model data.
     * @param materials Vector of shared pointers to materials used by the mesh.
     */
    OpenGlMesh(
      SharedPtr<Model> model,
      Vector<SharedPtr<IMaterial>> materials
    );
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
     * @brief Returns the model associated with this mesh.
     * 
     * @return Shared pointer to the model.
     */
    SharedPtr<Model> getModel() const override;

    /**
     * @brief Updates the mesh data or state.
     */
    void update() override;

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
     * @brief Binds the mesh's VAO for rendering.
     */
    void bind();

    /**
     * @brief Unbinds the mesh's VAO.
     */
    void unbind();

    /**
     * @brief Returns the OpenGL VAO handle.
     * @return VAO identifier.
     */
    UInt32 getVao() const;

  private:
    Id m_id;
    SharedPtr<Model> m_model;
    Vector<SharedPtr<IMaterial>> m_materials;
    UInt32 m_vao;
    UInt32 m_vbo;
    UInt32 m_ebo;

    void drawModelSubMesh(
      const RenderContext& renderContext,
      const ModelSubMesh& submesh
    );
  };
}
