#include "hc/graphics/renderPass/hcHairForwardSpecularRenderPass.h"
#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowMapManager.h"

#include <GL/glew.h>
#include <hc/graphics/resource/dataBlock/hcIDataBlockManager.h>

namespace hc
{
  HairForwardSpecularRenderPass::HairForwardSpecularRenderPass(
    OpenGlLightShadowMapManager& lightShadowMapManager,
    IDataBlockManager& dataBlockManager
  ) :
    m_lightShadowMapManager(lightShadowMapManager),
    m_dataBlockManager(dataBlockManager)
  {}

  HairForwardSpecularRenderPass::~HairForwardSpecularRenderPass()
  {}

  void HairForwardSpecularRenderPass::execute(
    const Vector<DrawCommand>& drawCommands,
    IFrameBuffer* currentRenderTarget
  )
  {
    if (currentRenderTarget)
    {
      if (!currentRenderTarget->isValid())
        throw RuntimeErrorException("Invalid framebuffer set as render target.");
      currentRenderTarget->bind();
    }

    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    if (!depthTestEnabled)
      glEnable(GL_DEPTH_TEST);

    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    if (!blendEnabled)
      glEnable(GL_BLEND);

    GLint blendFunction[4];
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendFunction[0]);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendFunction[1]);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendFunction[2]);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendFunction[3]);
    glBlendFunc(GL_ONE, GL_ONE);

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);
    glDepthFunc(GL_EQUAL);

    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    glDepthMask(GL_FALSE);

    for (const DrawCommand& command : drawCommands)
    {
      if (command.material->isDoubleSided())
      {
        executeTwoSidedDrawCommand(command);
      }
      else // One-sided opaque object
      {
        const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

        glBindVertexArray(drawData.vao);
        command.material->bind(
          renderPassType::Type::HairForwardSpecular,
          m_dataBlockManager
        );

        // Update the model matrix uniform in the material
        dataBlockStructure::ObjectData objectData;
        objectData.modelMatrix = command.modelMatrix.transposed();
        m_dataBlockManager.upload(dataBlockType::Object, &objectData);
        m_dataBlockManager.bind(dataBlockType::Object);

        m_lightShadowMapManager.bindShadowTexturesForReading();

        glDrawElements(
          static_cast<GLenum>(drawData.drawMode),
          static_cast<GLsizei>(command.indexCount),
          GL_UNSIGNED_INT,
          reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
        );

        command.material->unbind();
        glBindVertexArray(0);
      }
    }

    if (currentRenderTarget)
      currentRenderTarget->unbind();

    // Restore states

    glBlendFuncSeparate(
      static_cast<GLenum>(blendFunction[0]),
      static_cast<GLenum>(blendFunction[1]),
      static_cast<GLenum>(blendFunction[2]),
      static_cast<GLenum>(blendFunction[3])
    );

    if (!depthTestEnabled)
      glDisable(GL_DEPTH_TEST);

    if (!blendEnabled)
      glDisable(GL_BLEND);

    glDepthMask(depthMaskEnabled);
    glDepthFunc(currentDepthFunc);
  }

  void HairForwardSpecularRenderPass::executeTwoSidedDrawCommand(const DrawCommand& command)
  {
    GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    GLint cullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

    glDisable(GL_CULL_FACE);

    const OpenGlDrawData& drawData = std::get<OpenGlDrawData>(command.apiDrawData);

    glBindVertexArray(drawData.vao);
    command.material->bind(
      renderPassType::Type::HairForwardSpecular,
      m_dataBlockManager
    );

    // Update the model matrix uniform in the material
    dataBlockStructure::ObjectData objectData;
    objectData.modelMatrix = command.modelMatrix.transposed();
    m_dataBlockManager.upload(dataBlockType::Object, &objectData);
    m_dataBlockManager.bind(dataBlockType::Object);

    m_lightShadowMapManager.bindShadowTexturesForReading();

    glDrawElements(
      static_cast<GLenum>(drawData.drawMode),
      static_cast<GLsizei>(command.indexCount),
      GL_UNSIGNED_INT,
      reinterpret_cast<void*>(command.firstIndex * sizeof(UInt32))
    );

    command.material->unbind();
    glBindVertexArray(0);

    if (cullFaceEnabled)
      glEnable(GL_CULL_FACE);

    glCullFace(static_cast<GLenum>(cullFaceMode));
  }
}
