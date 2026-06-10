#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/renderPass/hcForwardOpaqueRenderPass.h"
#include "hc/graphics/renderPass/hcForwardTransparentRenderPass.h"
#include "hc/graphics/renderPass/hcSkyboxRenderPass.h"

namespace hc
{
  class ForwardRenderPipeline
  {
  public:
    ForwardRenderPipeline();
    ~ForwardRenderPipeline();

    void initialize(IShaderProgramManager& shaderProgramManager);
    void execute(
      const Vector<DrawCommand>& drawCommands,
      IFrameBuffer* currentRenderTarget
    );
    void destroy();

  private:
    ForwardOpaqueRenderPass m_forwardOpaqueRenderPass;
    ForwardTransparentRenderPass m_forwardTransparentRenderPass;
    SkyboxRenderPass m_skyboxRenderPass;
    Vector<DrawCommand> m_forwardOpaqueCommands;
    Vector<DrawCommand> m_forwardTransparentCommands;
    bool m_isInitialized;

    static void SplitDrawCommandsByRenderPass(
      const Vector<DrawCommand>& drawCommands,
      Vector<DrawCommand>& forwardOpaqueCommands,
      Vector<DrawCommand>& forwardTransparentCommands
    );

    void assertIsInitialized() const;
  };
}
