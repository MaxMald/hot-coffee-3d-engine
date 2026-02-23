#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"
#include <GL/glew.h>

namespace hc
{
  OpenGlMesh::OpenGlMesh(
    SharedPtr<Model> model,
    Vector<SharedPtr<IMaterial>> materials
  ) :
    m_id(Id::Create()),
    m_vao(0),
    m_vbo(0),
    m_ebo(0),
    m_model(model),
    m_materials(materials)
  {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    if (!m_model)
      return;

    update();
  }

  OpenGlMesh::~OpenGlMesh()
  {
    destroy();
  }

  const Id& OpenGlMesh::getId() const 
  {
    return m_id;
  }

  void OpenGlMesh::draw(const RenderContext& renderContext)
  {
    if (!m_model)
      return;

    glBindVertexArray(m_vao);

    const Vector<ModelSubMesh>& subMeshes = m_model->getSubMeshes();
    for (const ModelSubMesh& submesh : subMeshes)
      drawModelSubMesh(renderContext, submesh);

    glBindVertexArray(0);
  }

  SharedPtr<Model> OpenGlMesh::getModel() const
  {
    return m_model;
  }

  void OpenGlMesh::update()
  {
    if (!m_model)
      return;

    const Buffer<Vertex>& vertices = m_model->getVertices();
    const BufferUInt32& indices = m_model->getIndices();

    glBindVertexArray(m_vao);

    SizeT vertexBufferSize = vertices.size() * sizeof(Vertex);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    SizeT indexBufferSize = indices.size() * sizeof(UInt32);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices.data(), GL_STATIC_DRAW);

    // Attribute 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Attribute 1: normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Attribute 2: tangent
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(2);

    // Attribute 3: texCoord
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(3);

    // Attribute 4: color
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

  }

  void OpenGlMesh::destroy()
  {
    if (m_ebo != 0)
    {
      glDeleteBuffers(1, &m_ebo);
      m_ebo = 0;
    }

    if (m_vbo != 0)
    {
      glDeleteBuffers(1, &m_vbo);
      m_vbo = 0;
    }

    if (m_vao != 0)
    {
      glDeleteVertexArrays(1, &m_vao);
      m_vao = 0;
    }
  }

  const Vector<SharedPtr<IMaterial>> OpenGlMesh::getMaterials()
  {
    return m_materials;
  }

  void OpenGlMesh::bind()
  {
    if (m_vao)
      glBindVertexArray(m_vao);
  }

  void OpenGlMesh::unbind()
  {
    glBindVertexArray(0);
  }

  UInt32 OpenGlMesh::getVao() const
  {
    return m_vao;
  }

  void OpenGlMesh::drawModelSubMesh(
    const RenderContext& renderContext,
    const ModelSubMesh& submesh
  )
  {
    SharedPtr<IMaterial> material;

    if (submesh.materialIndex < m_materials.size())
      material = m_materials[submesh.materialIndex];

    if (material)
    {
      material->bind(renderContext.cameraMatrices);
      material->updateModelMatrix(renderContext.transform);
    }

    glDrawElements(
      GL_TRIANGLES,
      static_cast<GLsizei>(submesh.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(submesh.firstIndexIndex * sizeof(UInt32))
    );

    if (material)
      material->unbind();
  }
}
