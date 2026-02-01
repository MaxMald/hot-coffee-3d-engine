#pragma once

#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  class OpenGlShaderProgram;
  class OpenGlGraphicsManager;

  class OpenGlShaderProgramManager : public IShaderProgramManager
  {
  public:
    OpenGlShaderProgramManager();
    ~OpenGlShaderProgramManager() override;

    SharedPtr<IShaderProgram> getUnlit() override;
    void clear() override;

  private:
    IShaderManager* m_shaderManager;
    SharedPtr<OpenGlShaderProgram> m_unlitShaderProgram;

    void initialize(IShaderManager* shaderManager);
    void createPredefinedShaderPrograms();
    void createUnlitShaderProgram();

    friend class OpenGlGraphicsManager;
  };
}
