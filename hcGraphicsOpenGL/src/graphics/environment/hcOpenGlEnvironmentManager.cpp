#include "hc/graphics/environment/hcOpenGlEnvironmentManager.h"

#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc::graphics
{
  OpenGlEnvironmentManager::OpenGlEnvironmentManager(IDataBlockManager& dataBlockManager) :
    m_dataBlockManager(dataBlockManager),
    m_environmentCubeMap(nullptr),
    m_diffuseIrradianceCubeMap(nullptr),
    m_prefilteredEnvironmentCubeMap(nullptr),
    m_brdfLutTexture(nullptr),
    m_blackImage(1, 1, textureFormatType::RGB8, colorSpaceType::Linear, Color(0.0f)),
    m_blackCubeMap(),
    m_fbo(0), m_rbo(0),
    m_isInitialized(false)
  {
  }

  OpenGlEnvironmentManager::~OpenGlEnvironmentManager()
  {
    destroy();
  }

  void OpenGlEnvironmentManager::initialize(
    SharedPtr<IShaderProgram> brdfLutGeneratorShaderProgram
  )
  {
    destroy();

    try
    {
      CubeMapInitializationSettings cubeSettings;
      cubeSettings.faceSize = static_cast<UInt16>(m_blackImage.getHeight());
      cubeSettings.format = m_blackImage.getFormat();
      cubeSettings.colorSpace = m_blackImage.getColorSpace();

      m_blackCubeMap.initialize(
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage,
        cubeSettings
      );

      generateBrdfLutTexture(brdfLutGeneratorShaderProgram);
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        "Failed to initialize black cube map for environment textures: " + String(e.what())
      );
    }

    m_isInitialized = true;
  }

  void OpenGlEnvironmentManager::setEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for environment textures.");
    m_environmentCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::setDiffuseIrradianceCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for diffuse irradiance textures.");
    m_diffuseIrradianceCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::setPrefilteredEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for prefiltered environment textures.");
    m_prefilteredEnvironmentCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::clearEnvironmentCubeMap()
  {
    m_environmentCubeMap.reset();
  }

  void OpenGlEnvironmentManager::clearDiffuseIrradianceCubeMap()
  {
    m_diffuseIrradianceCubeMap.reset();
  }

  void OpenGlEnvironmentManager::clearPrefilteredEnvironmentCubeMap()
  {
    m_prefilteredEnvironmentCubeMap.reset();
  }

  void OpenGlEnvironmentManager::bindEnvironmentResources() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("Environment manager is not initialized.");
    
    if (m_brdfLutTexture != nullptr && m_brdfLutTexture->isValid())
      m_brdfLutTexture->bind(SLOT_ENVIRONMENT_BRDF_LUT_TEXTURE);
    else
      throw RuntimeErrorException("BRDF LUT texture is not initialized or valid.");

    if (m_environmentCubeMap != nullptr && m_environmentCubeMap->isValid())
      m_environmentCubeMap->bind(SLOT_ENVIRONMENT_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_CUBE_MAP);

    if (m_diffuseIrradianceCubeMap != nullptr && m_diffuseIrradianceCubeMap->isValid())
      m_diffuseIrradianceCubeMap->bind(SLOT_ENVIRONMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);

    if (m_prefilteredEnvironmentCubeMap != nullptr && m_prefilteredEnvironmentCubeMap->isValid())
      m_prefilteredEnvironmentCubeMap->bind(SLOT_ENVIRONMENT_PREFILTERED_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_PREFILTERED_CUBE_MAP);
  }

  void OpenGlEnvironmentManager::destroy()
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

    m_environmentCubeMap.reset();
    m_diffuseIrradianceCubeMap.reset();
    m_prefilteredEnvironmentCubeMap.reset();
    m_brdfLutTexture.reset();
    m_blackCubeMap.destroy();
    m_isInitialized = false;
  }

  void OpenGlEnvironmentManager::generateBrdfLutTexture(
    SharedPtr<IShaderProgram> brdfLutGeneratorShaderProgram
  )
  {
    if (!brdfLutGeneratorShaderProgram || !brdfLutGeneratorShaderProgram->isValid())
      throw InvalidArgumentException("Invalid BRDF LUT generator shader program provided.");

    // ---- LUT Texture creation

    const UInt32 lutSize = 512;
    m_brdfLutTexture = MakeShared<OpenGlTexture>();

    TextureInitializationSettings settings;
    settings.width = lutSize;
    settings.height = lutSize;
    settings.colorSpace = colorSpaceType::Linear;
    settings.format = textureFormatType::RGB16F;
    settings.minFilter = filterMode::Linear;
    settings.magFilter = filterMode::Linear;
    settings.wrap = wrapMode::ClampToEdge;
    settings.mipmapMode = mipmapMode::None;

    m_brdfLutTexture->initialize(settings);

    // ---- Framebuffer and Renderbuffer setup

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
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, lutSize, lutSize);
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

    // ---- Render the BRDF LUT using the provided shader program

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    try
    {
      brdfLutGeneratorShaderProgram->bind();

      dataBlockStructure::CubeMapGenerator generatorData;
      generatorData.sampleCount = 1024;
      m_dataBlockManager.upload(dataBlockType::CubeMapGenerator, &generatorData);
      m_dataBlockManager.bind(dataBlockType::CubeMapGenerator);

      glViewport(0, 0, lutSize, lutSize);
      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        static_cast<GLuint>(m_brdfLutTexture->getTextureId()),
        0
      );
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    catch (const Exception& ex)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
      glViewport(
        viewport[0], viewport[1],
        static_cast<GLsizei>(viewport[2]), static_cast<GLsizei>(viewport[3])
      );

      destroy();

      throw RuntimeErrorException(
        String::Format(
          "Failed to generate BRDF LUT texture: %s",
          ex.what()
        )
      );
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, currentReadFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentDrawFrameBuffer);
    glViewport(
      viewport[0], viewport[1],
      static_cast<GLsizei>(viewport[2]), static_cast<GLsizei>(viewport[3])
    );
  }
}
