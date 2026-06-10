#include "hc/graphics/renderPass/hcSkyboxRenderPass.h"

#include <GL/glew.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"
#include "hc/graphics/cubeMap/hcOpenGlCubeMap.h"

namespace hc
{
  SkyboxRenderPass::SkyboxRenderPass() :
    m_skyboxShaderProgram(nullptr),
    m_isInitialized(false),
    m_boxVao(0), m_boxVbo(0)
  {}

  SkyboxRenderPass::~SkyboxRenderPass()
  {
    destroy();
  }

  void SkyboxRenderPass::initialize(const SharedPtr<IShaderProgram>&skyboxShaderProgram)
  {
    if (m_isInitialized)
      throw RuntimeErrorException("SkyboxRenderPass is already initialized.");

    if (!skyboxShaderProgram || !skyboxShaderProgram->isValid())
      throw InvalidArgumentException("Invalid shader program provided for skybox render pass initialization.");

    m_skyboxShaderProgram = skyboxShaderProgram;

    float skyboxVertices[] = {
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
       1.0f,  1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f
    };

    GLint currentVao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
    GLint currentVbo = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVbo);

    try
    {
      glGenVertexArrays(1, &m_boxVao);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
      glBindVertexArray(m_boxVao);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glGenBuffers(1, &m_boxVbo);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();

      glBindBuffer(GL_ARRAY_BUFFER, m_boxVbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      destroy();
      glBindVertexArray(currentVao);
      glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
    }

    glBindVertexArray(currentVao);
    glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
    m_isInitialized = true;
  }

  void SkyboxRenderPass::execute(OpenGlCubeMap& cubeMap)
  {
    assertIsInitialized();

    if (!cubeMap.isValid())
      throw RuntimeErrorException("Invalid cube map provided to skybox render pass.");

    GLint currentTextureCubeMap = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &currentTextureCubeMap);

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    GLint currentActiveTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTexture);

    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);

    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint currentCullFaceMode = 0;
    glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFaceMode);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    try
    {
      m_skyboxShaderProgram->bind();

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GLuint>(cubeMap.getId()));
      glBindVertexArray(m_boxVao);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    catch (...)
    {
      glBindVertexArray(0);
      glActiveTexture(static_cast<GLenum>(currentActiveTexture));
      glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GLuint>(currentTextureCubeMap));
      glDepthFunc(currentDepthFunc);
      glDepthMask(depthMaskEnabled);

      if (!cullFaceEnabled)
        glDisable(GL_CULL_FACE);
      glCullFace(static_cast<GLenum>(currentCullFaceMode));

      throw;
    }

    glBindVertexArray(0);
    glActiveTexture(static_cast<GLenum>(currentActiveTexture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, static_cast<GLuint>(currentTextureCubeMap));
    glDepthFunc(currentDepthFunc);
    glDepthMask(depthMaskEnabled);

    if (!cullFaceEnabled)
      glDisable(GL_CULL_FACE);
    glCullFace(static_cast<GLenum>(currentCullFaceMode));
  }

  void SkyboxRenderPass::destroy()
  {
    if (m_boxVbo != 0)
    {
      GLint currentVbo = 0;
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVbo);

      if (m_boxVbo == static_cast<GLuint>(currentVbo))
        glBindBuffer(GL_ARRAY_BUFFER, 0);

      glDeleteBuffers(1, &m_boxVbo);
      m_boxVbo = 0;
    }

    if (m_boxVao != 0)
    {
      GLint currentVao = 0;
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);

      if (m_boxVao == static_cast<GLuint>(currentVao))
        glBindVertexArray(0);

      glDeleteVertexArrays(1, &m_boxVao);
      m_boxVao = 0;
    }

    m_skyboxShaderProgram = nullptr;
    m_isInitialized = false;
  }

  void SkyboxRenderPass::assertIsInitialized() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("SkyboxRenderPass is not initialized.");
  }
}
