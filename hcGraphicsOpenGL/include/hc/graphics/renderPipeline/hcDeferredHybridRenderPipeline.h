#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/frameBuffer/hcOpenGlGBuffer.h"
#include "hc/graphics/renderPass/hcDeferredGeometryRenderPass.h"
#include "hc/graphics/renderPass/hcDeferredLightingRenderPass.h"
#include "hc/graphics/renderPass/hcForwardOpaqueRenderPass.h"
#include "hc/graphics/renderPass/hcForwardTransparentRenderPass.h"
#include "hc/graphics/renderPass/hcSkyboxRenderPass.h"

namespace hc
{
  struct FrameRenderContext;

  class DeferredHybridRenderPipeline
  {
  public:
    DeferredHybridRenderPipeline();
    ~DeferredHybridRenderPipeline();

    void initialize(
      const Rect<UInt32>& viewportRect,
      IShaderProgramManager& shaderProgramManager
    );
    void clearGBuffer();
    void updateViewportSize(const Rect<UInt32>& viewportRect);
    OpenGlGBuffer& getGBuffer();
    void execute(
      const Vector<DrawCommand>& drawCommands,
      const FrameRenderContext& frameRenderContext
    );
    void destroy();

  private:
    DeferredGeometryRenderPass m_deferredGeometryRenderPass;
    DeferredLightingRenderPass m_deferredLightingRenderPass;
    ForwardOpaqueRenderPass m_forwardOpaqueRenderPass;
    ForwardTransparentRenderPass m_forwardTransparentRenderPass;
    SkyboxRenderPass m_skyboxRenderPass;
    OpenGlGBuffer m_gBuffer;
    Vector<DrawCommand> m_deferredOpaqueCommands;
    Vector<DrawCommand> m_forwardOpaqueCommands;
    Vector<DrawCommand> m_forwardTransparentCommands;
    bool m_initialized;

    static void SplitDrawCommandsByRenderPass(
      const Vector<DrawCommand>& drawCommands,
      Vector<DrawCommand>& deferredOpaqueCommands,
      Vector<DrawCommand>& forwardOpaqueCommands,
      Vector<DrawCommand>& forwardTransparentCommands
    );
    void copyDepthBufferToCurrentRenderTarget(IFrameBuffer* currentRenderTarget);
    void assertIsInitialized() const;
  };
}
