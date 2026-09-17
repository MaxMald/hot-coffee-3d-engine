#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlFrameBuffer.h"
#include "hc/graphics/renderPass/hcFinalRenderPass.h"
#include "hc/graphics/renderPipeline/hcDeferredHybridRenderPipeline.h"
#include "hc/graphics/lightShadowManager/hcOpenGlLightShadowMapManager.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlCubeMap;
  class IDataBlockManager;

  class FrameRenderer
  {
  public:
    FrameRenderer(IDataBlockManager& dataBlockManager);
    ~FrameRenderer();

    void initialize(const Rect<UInt32>& viewportRect, IShaderProgramManager& shaderProgramManager);
    void setRenderPipeline(renderPipelineType::Type);
    renderPipelineType::Type getCurrentRenderPipelineType() const;
    void setSkybox(OpenGlCubeMap* skybox);
    void removeSkybox();
    void setRenderTarget(IFrameBuffer* frameBuffer);
    IFrameBuffer* getRenderTarget() const;
    void removeRenderTarget();
    void queueDrawCommand(const DrawCommand& drawCommand);
    void executeDrawCommands();
    void clearDrawCommands();
    Vector<DrawCommand>& getDrawCommandQueue();
    const Vector<DrawCommand>& getDrawCommandQueue() const;
    void sortDrawCommands();
    const Vector<DrawCommand>& getDrawCommands() const;
    OpenGlGBuffer& getGBuffer();
    ILightShadowMapManager& getLightShadowMapManager();
    void clearFrame();
    void destroy();

    void onViewportChanged(const Rect<UInt32>& viewportRect);

  private:
    OpenGlLightShadowMapManager m_lightShadowMapManager;
    DeferredHybridRenderPipeline m_deferredHybridRenderPipeline;
    FinalRenderPass m_finalRenderPass;
    OpenGlFrameBuffer m_frameBufferA;
    Vector<DrawCommand> m_drawCommands;
    OpenGlCubeMap* m_skybox;
    renderPipelineType::Type m_currentRenderPipelineType;
    IFrameBuffer* m_currentRenderTarget;
    bool m_initialized;

    void assertIsInitialized() const;
    void copyDepthBuffer(IFrameBuffer& from, IFrameBuffer* to);
  };
}
