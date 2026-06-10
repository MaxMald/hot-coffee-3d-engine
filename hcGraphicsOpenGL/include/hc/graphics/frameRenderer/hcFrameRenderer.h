#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/renderPipeline/hcForwardRenderPipeline.h"
#include "hc/graphics/renderPipeline/hcDeferredHybridRenderPipeline.h"
#include "hc/graphics/ubos/hcUniformBufferObject.h"

namespace hc
{
  class OpenGlGBuffer;
  class OpenGlCubeMap;

  class FrameRenderer
  {
  public:
    FrameRenderer();
    ~FrameRenderer();

    void initialize(const Rect<UInt32>& viewportRect, IShaderProgramManager& shaderProgramManager);
    void setRenderPipeline(renderPipelineType::Type);
    renderPipelineType::Type getCurrentRenderPipelineType() const;
    void uploadCameraFrameData(const CameraFrameData& cameraFrameData);
    void uploadLightFrameData(const LightFrameData& lightFrameData);
    void setSkybox(const SharedPtr<OpenGlCubeMap>& skybox);
    void removeSkybox();
    void setRenderTarget(IFrameBuffer* frameBuffer);
    IFrameBuffer* getRenderTarget() const;
    void removeRenderTarget();
    void queueDrawCommand(const DrawCommand& drawCommand);
    void execute();
    OpenGlGBuffer& getGBuffer();
    void clearFrame();
    void destroy();

    void onViewportChanged(const Rect<UInt32>& viewportRect);

  private:
    ForwardRenderPipeline m_forwardRenderPipeline;
    DeferredHybridRenderPipeline m_deferredHybridRenderPipeline;
    LightFrameUBO m_lightFrameUBO;
    CameraFrameUBO m_cameraFrameUBO;
    Vector<DrawCommand> m_drawCommands;
    SharedPtr<OpenGlCubeMap> m_skybox;
    renderPipelineType::Type m_currentRenderPipelineType;
    IFrameBuffer* m_currentRenderTarget;
    bool m_initialized;

    void assertIsInitialized() const;
  };
}
