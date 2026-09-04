#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowMapManager.h"

#include <hc/graphics/hcDrawCommandUtilities.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>

namespace hc
{
  OpenGlLightShadowMapManager::OpenGlLightShadowMapManager(
    IDataBlockManager& dataBlockManager
  ) :
    m_shadowDrawCommands(),
    m_shadowMapShaderProgram(nullptr),
    m_directionalShadowFrameBuffer(),
    m_spotShadowFrameBuffer(),
    m_currentDirectionalShadowMapLayer(0),
    m_currentSpotShadowMapLayer(0),
    m_dataBlockManager(dataBlockManager)
  {}

  OpenGlLightShadowMapManager::~OpenGlLightShadowMapManager()
  {
    destroy();
  }

  void OpenGlLightShadowMapManager::bindShadowTexturesForReading()
  {
    glActiveTexture(GL_TEXTURE0 + SAMPLER_2D_BINDING_POINT_DIRECTIONAL_SHADOW_MAP);
    glBindTexture(
      GL_TEXTURE_2D_ARRAY,
      static_cast<GLuint>(m_directionalShadowFrameBuffer.getTextureArrayId())
    );

    glActiveTexture(GL_TEXTURE0 + SAMPLER_2D_BINDING_POINT_SPOT_LIGHT_SHADOW_MAP);
    glBindTexture(
      GL_TEXTURE_2D_ARRAY,
      static_cast<GLuint>(m_spotShadowFrameBuffer.getTextureArrayId())
    );
  }

  Int32 OpenGlLightShadowMapManager::generateDirectionalLightShadowTexture(
    Vector3f lightPosition,
    Matrix4 lightViewProjectionMatrix,
    const SceneGraph& sceneGraph
  )
  {
    if (hasReachedMaxDirectionalLightShadows())
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
    glViewport(
      0, 0,
      m_directionalShadowFrameBuffer.getWidth(),
      m_directionalShadowFrameBuffer.getHeight()
    );

    // Bind the shadow map shader program for rendering
    m_shadowMapShaderProgram->bind();

    // Upload the light's view-projection matrix to the shader program
    dataBlockStructure::LightViewProjection lightViewProjectionData;
    lightViewProjectionData.lightViewProjectionMatrix = lightViewProjectionMatrix;
    m_dataBlockManager.upload(
      dataBlockType::LightViewProjection,
      &lightViewProjectionData
    );
    m_dataBlockManager.bind(dataBlockType::LightViewProjection);

    bool depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);

    for (const DrawCommand& cmd : m_shadowDrawCommands)
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(cmd.apiDrawData);
      glBindVertexArray(drawData.vao);

      dataBlockStructure::ObjectData objectData;
      objectData.modelMatrix = cmd.modelMatrix.transposed();
      m_dataBlockManager.upload(dataBlockType::Object, &objectData);
      m_dataBlockManager.bind(dataBlockType::Object);

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

  Int32 OpenGlLightShadowMapManager::generateSpotLightShadowTexture(
    Vector3f lightPosition,
    Matrix4 lightViewProjectionMatrix,
    const SceneGraph& sceneGraph
  )
  {
    if (hasReachedMaxSpotLightShadows())
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
    glViewport(
      0, 0,
      m_spotShadowFrameBuffer.getWidth(),
      m_spotShadowFrameBuffer.getHeight()
    );

    // Bind the shadow map shader program for rendering
    m_shadowMapShaderProgram->bind();

    // Upload the light's view-projection matrix to the shader program
    dataBlockStructure::LightViewProjection lightViewProjectionData;
    lightViewProjectionData.lightViewProjectionMatrix = lightViewProjectionMatrix;
    m_dataBlockManager.upload(
      dataBlockType::LightViewProjection,
      &lightViewProjectionData
    );
    m_dataBlockManager.bind(dataBlockType::LightViewProjection);

    bool depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);

    for (const DrawCommand& cmd : m_shadowDrawCommands)
    {
      const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(cmd.apiDrawData);
      glBindVertexArray(drawData.vao);

      dataBlockStructure::ObjectData objectData;
      objectData.modelMatrix = cmd.modelMatrix.transposed();
      m_dataBlockManager.upload(dataBlockType::Object, &objectData);
      m_dataBlockManager.bind(dataBlockType::Object);

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

  void OpenGlLightShadowMapManager::clear()
  {
    m_currentDirectionalShadowMapLayer = 0;
    m_currentSpotShadowMapLayer = 0;
  }

  bool OpenGlLightShadowMapManager::hasReachedMaxDirectionalLightShadows() const
  {
    return m_currentDirectionalShadowMapLayer >= m_directionalShadowFrameBuffer.getNumLayers();
  }

  bool OpenGlLightShadowMapManager::hasReachedMaxSpotLightShadows() const
  {
    return m_currentSpotShadowMapLayer >= m_spotShadowFrameBuffer.getNumLayers();
  }

  void OpenGlLightShadowMapManager::destroy()
  {
    m_shadowMapShaderProgram.reset();
    m_directionalShadowFrameBuffer.destroy();
    m_spotShadowFrameBuffer.destroy();
    m_currentDirectionalShadowMapLayer = 0;
    m_currentSpotShadowMapLayer = 0;
  }

  void OpenGlLightShadowMapManager::initialize(
    IShaderProgramManager& shaderProgramManager,
    UInt32 directionalShadowMapSize,
    UInt32 spotShadowMapSize
  )
  {
    try
    {
      m_directionalShadowFrameBuffer.initialize(
        directionalShadowMapSize,
        directionalShadowMapSize,
        dataBlockStructure::LightShadows::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA
      );

      m_spotShadowFrameBuffer.initialize(
        spotShadowMapSize,
        spotShadowMapSize,
        dataBlockStructure::LightShadows::MAX_SPOT_LIGHTS_SHADOW_DATA
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
}
