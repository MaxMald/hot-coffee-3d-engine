#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"

namespace hc
{
  OpenGlMesh::OpenGlMesh(IGraphicsManager& graphicsManager) :
    m_id(Id::Create()),
    m_valid(false),
    m_materials(),
    m_subMeshes(),
    m_vao(0), m_vbo(0), m_ebo(0),
    m_drawMode(GL_TRIANGLES),
    m_graphicsManager(graphicsManager)
  {}

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
    assertIsValid();

    Vector3f cameraToModel = renderContext.modelPosition - renderContext.cameraWorldPosition;
    float distanceToCamera = cameraToModel.length();

    const Vector<ModelSubMesh>& subMeshes = m_subMeshes;
    for (const ModelSubMesh& submesh : subMeshes)
      drawModelSubMesh(renderContext, distanceToCamera, submesh);
  }

  void OpenGlMesh::initialize(
    const Model& model,
    const Vector<SharedPtr<IMaterial>>& materials
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Mesh is already initialized");

    try
    {
      createBuffers();
      updateVertexAndIndexBuffers(model.getVertices(), model.getIndices());
    }
    catch (...)
    {
      destroy();
      throw;
    }

    m_sourcePath = model.getPath();
    m_subMeshes = model.getSubMeshes();
    m_materials = materials;
    m_valid = true;
  }

  void OpenGlMesh::initialize(
    const Buffer<Vertex>& vertices,
    const BufferUInt32& indices,
    const Vector<SharedPtr<IMaterial>>& materials
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Mesh is already initialized");

    try
    {
      createBuffers();
      updateVertexAndIndexBuffers(vertices, indices);
    }
    catch (...)
    {
      destroy();
      throw;
    }

    ModelSubMesh defaultSubMesh;
    defaultSubMesh.firstVertexIndex = 0;
    defaultSubMesh.vertexCount = static_cast<UInt32>(vertices.size());
    defaultSubMesh.firstIndexIndex = 0;
    defaultSubMesh.indexCount = static_cast<UInt32>(indices.size());
    defaultSubMesh.materialIndex = 0;

    m_subMeshes = { defaultSubMesh };
    m_materials = materials;
    m_valid = true;
  }

  void OpenGlMesh::initialize(
    const Buffer<Vertex>&vertices,
    const BufferUInt32 & indices,
    const Vector<ModelSubMesh>&subMeshes,
    const Vector<SharedPtr<IMaterial>>&materials
  )
  {
    if (m_valid)
      throw RuntimeErrorException("Mesh is already initialized");

    try
    {
      createBuffers();
      updateVertexAndIndexBuffers(vertices, indices);
    }
    catch (...)
    {
      destroy();
      throw;
    }

    m_subMeshes = subMeshes;
    m_materials = materials;
    m_valid = true;
  }

  void OpenGlMesh::update(const Model& model)
  {
    assertIsValid();
    updateVertexAndIndexBuffers(model.getVertices(), model.getIndices());
    m_subMeshes = model.getSubMeshes();
    m_sourcePath = model.getPath();
  }

  void OpenGlMesh::update(const Buffer<Vertex>& vertices, const BufferUInt32& indices)
  {
    assertIsValid();
    updateVertexAndIndexBuffers(vertices, indices);
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

  SharedPtr<IMaterial> OpenGlMesh::getMaterial(UInt32 index) const
  {
    if (static_cast<SizeT>(index) >= m_materials.size())
      throw RuntimeErrorException(
        String::Format("Material index %d is out of bounds for materials size %zu",
          index, m_materials.size()
        )
      );

    return m_materials[index];
  }

  void OpenGlMesh::destroy()
  {
    if (m_ebo != 0)
    {
      GLint currentEbo = 0;
      glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentEbo);

      if (static_cast<GLuint>(currentEbo) == m_ebo)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      glDeleteBuffers(1, &m_ebo);
      m_ebo = 0;
    }

    if (m_vbo != 0)
    {
      GLint currentVbo = 0;
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVbo);

      if (static_cast<GLuint>(currentVbo) == m_vbo)
        glBindBuffer(GL_ARRAY_BUFFER, 0);

      glDeleteBuffers(1, &m_vbo);
      m_vbo = 0;
    }

    if (m_vao != 0)
    {
      GLint currentVao = 0;
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);

      if (static_cast<GLuint>(currentVao) == m_vao)
        glBindVertexArray(0);

      glDeleteVertexArrays(1, &m_vao);
      m_vao = 0;
    }

    m_sourcePath.clear();
    m_subMeshes.clear();
    m_materials.clear();
    m_valid = false;
  }

  const Vector<SharedPtr<IMaterial>> OpenGlMesh::getMaterials()
  {
    return m_materials;
  }

  drawType::Type OpenGlMesh::getDrawType() const
  {
    return openGlGraphicsUtilities::GetDrawTypeFromOpenGlMode(m_drawMode);
  }

  void OpenGlMesh::setDrawType(drawType::Type drawType)
  {
    m_drawMode = openGlGraphicsUtilities::GetOpenGlDrawModeFromDrawType(drawType);
  }

  bool OpenGlMesh::isValid() const
  {
    return m_valid;
  }

  Path OpenGlMesh::getSourcePath() const
  {
    return m_sourcePath;
  }

  void OpenGlMesh::setSourcePath(const Path& path)
  {
    m_sourcePath = path;
  }

  void OpenGlMesh::bind()
  {
    assertIsValid();
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

  UInt32 OpenGlMesh::getDrawMode() const
  {
    return m_drawMode;
  }

  void OpenGlMesh::assertIsValid() const
  {
    if (!m_valid)
      throw RuntimeErrorException("OpenGlMesh is not valid.");
  }

  void OpenGlMesh::createBuffers()
  {
    glGenVertexArrays(1, &m_vao);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
    glGenBuffers(1, &m_vbo);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
    glGenBuffers(1, &m_ebo);
    openGlGraphicsUtilities::AssertOpenGlHasNoError();
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
      renderContext.transform,
      material,
      distanceToCamera,
      submesh.firstIndexIndex,
      submesh.indexCount,
      renderContext.polygonFillType,
      OpenGlDrawData{ m_vao, m_drawMode }
    );

    m_graphicsManager.draw(command);
  }

  void OpenGlMesh::updateVertexAndIndexBuffers(
    const Buffer<Vertex>& vertices,
    const BufferUInt32& indices
  )
  {
    GLint currentVao = 0;
    GLint currentVbo = 0;
    GLint currentEbo = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVbo);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentEbo);

    try
    {
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
    }
    catch (...)
    {
      glBindVertexArray(currentVao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentEbo);
      glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
      throw;
    }

    glBindVertexArray(currentVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentEbo);
    glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
  }
}
