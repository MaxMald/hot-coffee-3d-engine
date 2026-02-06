#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlTextureManager;
  class OpenGlMaterialManager;

  class OpenGlMesh : public IMesh
  {
  public:
    OpenGlMesh(
      SharedPtr<Model> model,
      OpenGlMaterialManager* materialManager
    );
    ~OpenGlMesh() override;

    const Id& getId() const override;
    void draw(const RenderContext& renderContext) override;
    SharedPtr<Model> getModel() const override;
    void update() override;
    void destroy() override;
    const Vector<SharedPtr<IMaterial>> getMaterials() override;

    void bind();
    void unbind();
    UInt32 getVao() const;

  private:
    Id m_id;
    SharedPtr<Model> m_model;
    Vector<SharedPtr<IMaterial>> m_materials;
    UInt32 m_vao;
    UInt32 m_vbo;
    UInt32 m_ebo;

    void createMaterials(OpenGlMaterialManager* materialManager);
    void drawModelSubMesh(const RenderContext& renderContext, const ModelSubMesh& submesh);
  };
}
