#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlCubeMap;

  class SkyboxRenderPass
  {
  public:
    SkyboxRenderPass();
    ~SkyboxRenderPass();

    void initialize(const SharedPtr<IShaderProgram>& skyboxShaderProgram);
    void execute(OpenGlCubeMap* cubeMap, IFrameBuffer* frameBuffer);
    void destroy();

  private:
    SharedPtr<IShaderProgram> m_skyboxShaderProgram;
    bool m_isInitialized;
    UInt32 m_boxVao;
    UInt32 m_boxVbo;

    void assertIsInitialized() const;
  };
}
