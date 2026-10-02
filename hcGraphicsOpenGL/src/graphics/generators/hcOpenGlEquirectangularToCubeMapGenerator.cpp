#include "hc/graphics/generators/hcOpenGlEquirectangularToCubeMapGenerator.h"

#include <hc/graphics/resource/dataBlock/hcDataBlockStructures.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>
#include "hc/graphics/hcOpenGlGraphicsUtilities.h"
#include "hc/graphics/resource/texture/hcOpenGlTexture.h"
#include "hc/graphics/resource/cubeMap/hcOpenGlCubeMap.h"

namespace hc::graphics::generators
{
  static const Matrix4 CUBEMAP_CAPTURE_PROJECTION = Matrix4::Perspective(
    90.0f * Math::DegToRad,
    1.0f, 0.1f, 10.0f
  ).transpose();

  static const Array<Matrix4, 6> CUBEMAP_CAPTURE_VIEWS = {
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)).transpose(),
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f)).transpose(),
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f)).transpose(),
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0.0f,0.0f, 1.0f)).transpose(),
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 1.0f, 0.0f)).transpose(),
    Matrix4::LookAt(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f)).transpose()
  };

  OpenGlEquirectangularToCubeMapGenerator::OpenGlEquirectangularToCubeMapGenerator(
    IDataBlockManager& dataBlockManager
  ) : m_dataBlockManager(dataBlockManager),
    m_equirectangularToCubemapShaderProgram(nullptr),
    m_fbo(0), m_rbo(0), m_currentRenderbufferSize(0),
    m_boxVao(0), m_boxVbo(0),
    m_isValid(false)
  {
  }

  OpenGlEquirectangularToCubeMapGenerator::~OpenGlEquirectangularToCubeMapGenerator()
  {
  }

  SharedPtr<ICubeMap> OpenGlEquirectangularToCubeMapGenerator::generate(
    const ITexture& equirectangularTexture,
    UInt32 faceSize
  )
  {
    if (!equirectangularTexture.isValid())
      throw InvalidArgumentException("Invalid equirectangular texture provided.");
    if (faceSize == 0)
      throw InvalidArgumentException("Face size must be greater than zero.");
    if (m_equirectangularToCubemapShaderProgram == nullptr)
      throw RuntimeErrorException("Equirectangular to cubemap shader program is not initialized.");
    if (!m_equirectangularToCubemapShaderProgram->isValid())
      throw RuntimeErrorException("Equirectangular to cubemap shader program is not valid.");

    SharedPtr<OpenGlCubeMap> cubeMap = MakeShared<OpenGlCubeMap>();
    try
    {
      cubeMap->initialize(
        faceSize,
        equirectangularTexture.getTextureFormat(),
        equirectangularTexture.getColorSpace()
      );
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
      String::Format(
        "Failed to initialize cube map for equirectangular to cubemap conversion: %s",
        e.what()
      ));
    }

    if (m_currentRenderbufferSize != faceSize)
      resize(faceSize);

    GLint currentActiveTexture0;
    GLint currentBindTexture;
    GLint viewport[4];
    glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTexture0);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentBindTexture);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);
    GLint currentVao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);

    try
    {
      m_equirectangularToCubemapShaderProgram->bind();
      equirectangularTexture.bind(0);

      dataBlockStructure::Camera cameraData;
      cameraData.projectionMatrix = CUBEMAP_CAPTURE_PROJECTION;
      cameraData.cameraWorldPosition = Vector3f(0.0f, 0.0f, 0.0f);

      glViewport(
        static_cast<GLint>(0), static_cast<GLint>(0),
        static_cast<GLsizei>(faceSize),
        static_cast<GLsizei>(faceSize)
      );
      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glDepthFunc(GL_LEQUAL);

      for (UInt32 faceIndex = 0; faceIndex < 6; ++faceIndex)
      {
        cameraData.viewMatrix = CUBEMAP_CAPTURE_VIEWS[faceIndex];
        m_dataBlockManager.upload(dataBlockType::Camera, &cameraData);
        m_dataBlockManager.bind(dataBlockType::Camera);

        glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          GL_COLOR_ATTACHMENT0,
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
          cubeMap->getId(), 0
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(m_boxVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }
    catch (const Exception& e)
    {
      glBindVertexArray(currentVao);
      glDepthFunc(currentDepthFunc);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);

      glViewport(viewport[0], viewport[1],
        static_cast<GLsizei>(viewport[2]), static_cast<GLsizei>(viewport[3])
      );
      glActiveTexture(currentActiveTexture0);
      glBindTexture(GL_TEXTURE_2D, currentBindTexture);

      throw RuntimeErrorException(
        String::Format(
          "Failed during equirectangular to cubemap conversion: %s",
          e.what()
        ));
    }

    glBindVertexArray(currentVao);
    glDepthFunc(currentDepthFunc);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);

    glViewport(viewport[0], viewport[1],
      static_cast<GLsizei>(viewport[2]), static_cast<GLsizei>(viewport[3])
    );
    glActiveTexture(currentActiveTexture0);
    glBindTexture(GL_TEXTURE_2D, currentBindTexture);

    return cubeMap;
  }

  void OpenGlEquirectangularToCubeMapGenerator::destroy()
  {
    if (m_rbo != 0)
    {
      GLint currentRenderbuffer = 0;
      glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);
      if (currentRenderbuffer == static_cast<GLint>(m_rbo))
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

      glDeleteRenderbuffers(1, &m_rbo);
      m_rbo = 0;
    }

    if (m_fbo != 0)
    {
      GLint currentReadFramebuffer = 0;
      glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFramebuffer);
      if (currentReadFramebuffer == static_cast<GLint>(m_fbo))
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

      GLint currentDrawFramebuffer = 0;
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFramebuffer);
      if (currentDrawFramebuffer == static_cast<GLint>(m_fbo))
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

      glDeleteFramebuffers(1, &m_fbo);
      m_fbo = 0;
    }

    if (m_boxVbo != 0)
    {
      GLint currentVbo = 0;
      glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVbo);
      if (currentVbo == static_cast<GLint>(m_boxVbo))
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDeleteBuffers(1, &m_boxVbo);
      m_boxVbo = 0;
    }

    if (m_boxVao != 0)
    {
      GLint currentVao = 0;
      glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
      if (currentVao == static_cast<GLint>(m_boxVao))
        glBindVertexArray(0);
      glDeleteVertexArrays(1, &m_boxVao);
      m_boxVao = 0;
    }

    m_equirectangularToCubemapShaderProgram.reset();
    m_currentRenderbufferSize = 0;
    m_isValid = false;
  }

  void OpenGlEquirectangularToCubeMapGenerator::initialize(
    SharedPtr<IShaderProgram> equirectangularToCubeMapShaderProgram
  )
  {
    destroy();

    if (equirectangularToCubeMapShaderProgram == nullptr
      || !equirectangularToCubeMapShaderProgram->isValid())
      throw InvalidArgumentException(
        "Invalid shader program provided for equirectangular to cubemap conversion."
      );

    try
    {
      createBox();
      createFramebuffer();
    }
    catch (...)
    {
      destroy();
      throw;
    }
    
    m_equirectangularToCubemapShaderProgram = equirectangularToCubeMapShaderProgram;
    m_currentRenderbufferSize = 1;
    m_isValid = true;
  }

  void OpenGlEquirectangularToCubeMapGenerator::resize(UInt32 faceSize)
  {
    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
      glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
      glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
        faceSize, faceSize
      );

      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw RuntimeErrorException(
          String::Format(
            "OpenGlEquirectangularToCubeMapGenerator::resize: Failed to resize framebuffer. Size: %u",
            faceSize)
        );
    }
    catch (...)
    {
      glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      destroy();
      throw;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
  }

  void OpenGlEquirectangularToCubeMapGenerator::createBox()
  {
    float boxVertices[] = {
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
    GLint currentVbo = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVao);
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
      glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      openGlGraphicsUtilities::AssertOpenGlHasNoError();
    }
    catch (...)
    {
      destroy();
      glBindVertexArray(currentVao);
      glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
      throw;
    }

    glBindVertexArray(currentVao);
    glBindBuffer(GL_ARRAY_BUFFER, currentVbo);
  }

  void OpenGlEquirectangularToCubeMapGenerator::createFramebuffer()
  {
    GLint currentReadFrameBuffer = 0;
    GLint currentDrawFrameBuffer = 0;
    GLint currentRenderbuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &currentReadFrameBuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentDrawFrameBuffer);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);

    try
    {
      glGenFramebuffers(1, &m_fbo);
      glGenRenderbuffers(1, &m_rbo);

      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1, 1);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    }
    catch (...)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
      destroy();
      throw;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
  }
}
