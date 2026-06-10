#include "hc/graphics/renderPipeline/hcForwardRenderPipeline.h"
#include <GL/glew.h>

namespace hc
{
  ForwardRenderPipeline::ForwardRenderPipeline() :
    m_forwardOpaqueRenderPass(),
    m_forwardTransparentRenderPass(),
    m_skyboxRenderPass(),
    m_forwardOpaqueCommands(),
    m_forwardTransparentCommands(),
    m_isInitialized(false)
  {}

  ForwardRenderPipeline::~ForwardRenderPipeline()
  {
    destroy();
  }

  void ForwardRenderPipeline::initialize(IShaderProgramManager& shaderProgramManager)
  {
    if (m_isInitialized)
      throw RuntimeErrorException("ForwardRenderPipeline is already initialized.");

    try
    {
      m_skyboxRenderPass.initialize(
        shaderProgramManager.getBuiltInShaderProgram(builtInShaderProgramType::Skybox)
      );
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        "Failed to initialize ForwardRenderPipeline: " + String(e.what())
      );
    }

    m_isInitialized = true;
  }

  void ForwardRenderPipeline::execute(
    const Vector<DrawCommand>& drawCommands,
    IFrameBuffer * currentRenderTarget
  )
  {
    assertIsInitialized();

    m_forwardOpaqueCommands.clear();
    m_forwardTransparentCommands.clear();

    SplitDrawCommandsByRenderPass(
      drawCommands,
      m_forwardOpaqueCommands,
      m_forwardTransparentCommands
    );

    m_forwardOpaqueRenderPass.execute(m_forwardOpaqueCommands, currentRenderTarget);
    // TODO - skybox pass
    m_forwardTransparentRenderPass.execute(m_forwardTransparentCommands, currentRenderTarget);
  }

  void ForwardRenderPipeline::destroy()
  {
    m_skyboxRenderPass.destroy();
    m_isInitialized = false;
  }

  void ForwardRenderPipeline::SplitDrawCommandsByRenderPass(
    const Vector<DrawCommand>& drawCommands,
    Vector<DrawCommand>& forwardOpaqueCommands,
    Vector<DrawCommand>& forwardTransparentCommands
  )
  {
    for (SizeT i = 0; i < drawCommands.size(); ++i)
    {
      const DrawCommand& cmd = drawCommands[i];

      if (!cmd.material)
        continue;

      if (cmd.material->getRenderMode() == materialRenderMode::Type::Transparent)
        forwardTransparentCommands.push_back(cmd);
      else
        forwardOpaqueCommands.push_back(cmd);
    }
  }

  void ForwardRenderPipeline::assertIsInitialized() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("ForwardRenderPipeline is not initialized.");
  }
}
