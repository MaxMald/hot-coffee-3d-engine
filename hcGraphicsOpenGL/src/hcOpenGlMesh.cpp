#include "hc/hcOpenGlMesh.h"
#include <GL/glew.h>

namespace hc
{
  OpenGlMesh::OpenGlMesh(SharedPtr<Model> model) :
    m_vao(0),
    m_vbo(0),
    m_ebo(0),
    m_model(model)
  {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    if (m_model)
      update();
  }

  OpenGlMesh::~OpenGlMesh()
  {
    destroy();
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

    // Example: set up vertex attributes (adjust to your vertex format)
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    // glEnableVertexAttribArray(0);

    // Esto creo que lo hace el material?

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
}
