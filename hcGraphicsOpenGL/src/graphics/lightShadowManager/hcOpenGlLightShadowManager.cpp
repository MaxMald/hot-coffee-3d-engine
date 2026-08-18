#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowManager.h"
#include <hc/graphics/hcDrawCommandUtilities.h>

namespace hc
{
  OpenGlLightShadowManager::OpenGlLightShadowManager() :
    m_lightShadowUBO(),
    m_shadowDrawCommands(),
    m_shadowMapShaderProgram(nullptr),
    m_directionalShadowFrameBuffer(),
    m_spotShadowFrameBuffer(),
    m_currentDirectionalShadowMapLayer(0),
    m_currentSpotShadowMapLayer(0)
  {}

  OpenGlLightShadowManager::~OpenGlLightShadowManager()
  {
    destroy();
  }

  void OpenGlLightShadowManager::uploadShadowDataToGPU()
  {
    m_lightShadowUBO.upload(m_lightShadowFrameData);
  }

  void OpenGlLightShadowManager::bindShadowTexturesForReading(
    UInt8 directionalTextureArrayUnit,
    UInt8 spotTextureArrayUnit
  )
  {
    glActiveTexture(GL_TEXTURE0 + directionalTextureArrayUnit);
    glBindTexture(
      GL_TEXTURE_2D_ARRAY,
      static_cast<GLuint>(m_directionalShadowFrameBuffer.getTextureArrayId())
    );

    glActiveTexture(GL_TEXTURE0 + spotTextureArrayUnit);
    glBindTexture(
      GL_TEXTURE_2D_ARRAY,
      static_cast<GLuint>(m_spotShadowFrameBuffer.getTextureArrayId())
    );
  }

  void OpenGlLightShadowManager::destroy()
  {
    m_shadowMapShaderProgram.reset();
    m_lightShadowUBO.destroy();
    m_directionalShadowFrameBuffer.destroy();
    m_spotShadowFrameBuffer.destroy();
    m_currentDirectionalShadowMapLayer = 0;
    m_currentSpotShadowMapLayer = 0;
  }

  void OpenGlLightShadowManager::initialize(
    UInt32 bindingPoint,
    IShaderProgramManager& shaderProgramManager,
    UInt32 directionalShadowMapSize,
    UInt32 spotShadowMapSize
  )
  {
    try
    {
      m_lightShadowUBO.initialize(LightShadowFrameData{});
      m_lightShadowUBO.bindBase(bindingPoint);

      m_directionalShadowFrameBuffer.initialize(
        directionalShadowMapSize,
        directionalShadowMapSize,
        LightShadowFrameData::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA
      );

      m_spotShadowFrameBuffer.initialize(
        spotShadowMapSize,
        spotShadowMapSize,
        LightShadowFrameData::MAX_SPOT_LIGHTS_SHADOW_DATA
      );

      m_shadowMapShaderProgram = shaderProgramManager.getBuiltInShaderProgram(
        builtInShaderProgramType::ShadowMap
      );
    }
    catch (...)
    {
      destroy();
      throw;
    }

    m_currentDirectionalShadowMapLayer = 0;
    m_currentSpotShadowMapLayer = 0;
  }

  Int32 OpenGlLightShadowManager::generateDirectionalLightShadowTexture(
    Vector3f lightPosition,
    Matrix4 lightViewProjectionMatrix,
    const SceneGraph& sceneGraph
  )
  {
    if (m_currentDirectionalShadowMapLayer >= m_directionalShadowFrameBuffer.getNumLayers())
      return -1;

    // Gather draw commands for the scene from the perspective of the light

    // TODO
    //
    // We should filter the scene objects to only include those that are within the
    // light's frustum and can cast shadows. This could be a scene graph's frustum culling
    // operation or a spatial partitioning query.

    RenderContext renderContext;
    renderContext.cameraWorldPosition = lightPosition;
    renderContext.transform = Matrix4::Identity();
    renderContext.modelPosition = Vector3f(0.0f, 0.0f, 0.0f);

    m_shadowDrawCommands.clear();
    sceneGraph.draw(renderContext, m_shadowDrawCommands);

    DrawCommandUtilities::SortDrawCommands(m_shadowDrawCommands);

    // Render the scene to the shadow framebuffer using the gathered draw commands
    m_directionalShadowFrameBuffer.bind(m_currentDirectionalShadowMapLayer);

    // Set the viewport to match the shadow map texture size
    GLint viewportRect[4];
    glGetIntegerv(GL_VIEWPORT, viewportRect);
    glViewport(0, 0, m_directionalShadowFrameBuffer.getWidth(), m_directionalShadowFrameBuffer.getHeight());

    m_shadowMapShaderProgram->bind();
    m_shadowMapShaderProgram->setUniform("uLightViewProjection", lightViewProjectionMatrix);

    bool depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);

    for (const DrawCommand& cmd : m_shadowDrawCommands)
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(cmd.apiDrawData);
      glBindVertexArray(drawData.vao);

      m_shadowMapShaderProgram->setUniform("uModel", cmd.modelMatrix);

      glDrawElements(
        static_cast<GLenum>(drawData.drawMode),
        static_cast<GLsizei>(cmd.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(cmd.firstIndex * sizeof(UInt32))
      );
    }

    if (!depthTestEnabled)
      glDisable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    m_directionalShadowFrameBuffer.unbind();
    m_shadowDrawCommands.clear();

    // restore viewport
    glViewport(viewportRect[0], viewportRect[1], viewportRect[2], viewportRect[3]);

    Int32 shadowMapLayerIndex = static_cast<Int32>(m_currentDirectionalShadowMapLayer);
    ++m_currentDirectionalShadowMapLayer;
    return shadowMapLayerIndex;
  }

  Int32 OpenGlLightShadowManager::generateSpotLightShadowTexture(
    Vector3f lightPosition,
    Matrix4 lightViewProjectionMatrix,
    const SceneGraph& sceneGraph
  )
  {
    if (m_currentSpotShadowMapLayer >= m_spotShadowFrameBuffer.getNumLayers())
      return -1;

    // Gather draw commands for the scene from the perspective of the light

    // TODO
    //
    // We should filter the scene objects to only include those that are within the
    // light's frustum and can cast shadows. This could be a scene graph's frustum culling
    // operation or a spatial partitioning query.

    RenderContext renderContext;
    renderContext.cameraWorldPosition = lightPosition;
    renderContext.transform = Matrix4::Identity();
    renderContext.modelPosition = Vector3f(0.0f, 0.0f, 0.0f);

    m_shadowDrawCommands.clear();
    sceneGraph.draw(renderContext, m_shadowDrawCommands);

    DrawCommandUtilities::SortDrawCommands(m_shadowDrawCommands);

    // Render the scene to the shadow framebuffer using the gathered draw commands
    m_spotShadowFrameBuffer.bind(m_currentSpotShadowMapLayer);

    // Set the viewport to match the shadow map texture size
    GLint viewportRect[4];
    glGetIntegerv(GL_VIEWPORT, viewportRect);
    glViewport(0, 0, m_spotShadowFrameBuffer.getWidth(), m_spotShadowFrameBuffer.getHeight());

    m_shadowMapShaderProgram->bind();
    m_shadowMapShaderProgram->setUniform("uLightViewProjection", lightViewProjectionMatrix);

    bool depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);

    for (const DrawCommand& cmd : m_shadowDrawCommands)
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(cmd.apiDrawData);
      glBindVertexArray(drawData.vao);

      m_shadowMapShaderProgram->setUniform("uModel", cmd.modelMatrix);

      glDrawElements(
        static_cast<GLenum>(drawData.drawMode),
        static_cast<GLsizei>(cmd.indexCount),
        GL_UNSIGNED_INT,
        reinterpret_cast<void*>(cmd.firstIndex * sizeof(UInt32))
      );
    }

    if (!depthTestEnabled)
      glDisable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    m_spotShadowFrameBuffer.unbind();
    m_shadowDrawCommands.clear();

    // restore viewport
    glViewport(viewportRect[0], viewportRect[1], viewportRect[2], viewportRect[3]);

    Int32 shadowMapLayerIndex = static_cast<Int32>(m_currentSpotShadowMapLayer);
    ++m_currentSpotShadowMapLayer;
    return shadowMapLayerIndex;
  }

  void OpenGlLightShadowManager::onClear()
  {
    m_currentDirectionalShadowMapLayer = 0;
    m_currentSpotShadowMapLayer = 0;
  }
}
