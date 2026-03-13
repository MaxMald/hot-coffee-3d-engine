#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"
#include <GL/glew.h>

namespace hc
{
  OpenGlMesh::OpenGlMesh(
    SharedPtr<Model> model,
    Vector<SharedPtr<IMaterial>> materials,
    IGraphicsManager& graphicsManager
  ) :
    m_id(Id::Create()),
    m_vao(0),
    m_vbo(0),
    m_ebo(0),
    m_model(model),
    m_materials(materials),
    m_graphicsManager(graphicsManager)
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

    float distanceToCamera = (renderContext.cameraPosition - renderContext.modelPosition).length();

    const Vector<ModelSubMesh>& subMeshes = m_model->getSubMeshes();
    for (const ModelSubMesh& submesh : subMeshes)
      drawModelSubMesh(renderContext, distanceToCamera, submesh);
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

  SizeT OpenGlMesh::getMaterialsSize() const
  {
    return m_materials.size();
  }

  void OpenGlMesh::setMaterial(UInt32 index, SharedPtr<IMaterial> material)
  {
    if (static_cast<SizeT>(index) >= m_materials.size())
      throw RuntimeErrorException(
        String::Format("Material index %d is out of bounds for materials size %zu",
          index, m_materials.size()
        )
      );

    m_materials[index] = material;
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
    float distanceToCamera,
    const ModelSubMesh& submesh
  )
  {
    SharedPtr<IMaterial> material;

    if (submesh.materialIndex < m_materials.size())
      material = m_materials[submesh.materialIndex];

    if (!material)
    {
      LogService::Error(
        String::Format(
          "OpenGlMesh::drawModelSubMesh: Submesh with material index %d has no material assigned, skipping draw call.",
          submesh.materialIndex
        )
      );

      return;
    }

    DrawCommand command;
    command.initialize(
      renderContext.cameraMatrices,
      renderContext.transform,
      material,
      distanceToCamera,
      submesh.firstIndexIndex,
      submesh.indexCount,
      OpenGlDrawData{ m_vao }
    );

    m_graphicsManager.draw(command);
  }
}
