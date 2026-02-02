#include "hc/hcOpenGlShaderProgramManager.h"
#include "hc/hcOpenGlShaderProgram.h"
#include "hc/hcOpenGlShader.h"
#include "hc/hcBuiltInShaders.h"

namespace hc
{
  OpenGlShaderProgramManager::OpenGlShaderProgramManager()
    : m_shaderManager(nullptr)
  {
  }

  OpenGlShaderProgramManager::~OpenGlShaderProgramManager()
  {
  }

  SharedPtr<IShaderProgram> OpenGlShaderProgramManager::getUnlit()
  {
    return m_unlitShaderProgram;
  }

  void OpenGlShaderProgramManager::clear()
  {
    m_unlitShaderProgram.reset();
  }

  void OpenGlShaderProgramManager::initialize(IShaderManager* shaderManager)
  {
    m_shaderManager = shaderManager;
    createPredefinedShaderPrograms();
  }

  void OpenGlShaderProgramManager::createPredefinedShaderPrograms()
  {
    createUnlitShaderProgram();
  }

  void OpenGlShaderProgramManager::createUnlitShaderProgram()
  {
    auto vertexShader = MakeShared<OpenGlShader>(
      shaderStageType::Vertex,
      builtInShaders::VertexShader
    );
    vertexShader->compile();
    if (!vertexShader->isCompiled())
      return;

    auto fragmentShader = MakeShared<OpenGlShader>(
      shaderStageType::Fragment,
      builtInShaders::UnlitFragment
    );
    fragmentShader->compile();
    if (!fragmentShader->isCompiled())
      return;

    auto program = MakeShared<OpenGlShaderProgram>();
    program->attachShader(vertexShader);
    program->attachShader(fragmentShader);

    if (!program->link())
      return;

    m_unlitShaderProgram = program;
  }
}
