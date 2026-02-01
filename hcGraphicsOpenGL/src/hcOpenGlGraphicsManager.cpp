#include "hc/hcOpenGlGraphicsManager.h"

#include <GL/glew.h>
#include "hc/hcOpenGlShader.h"
#include "hc/hcOpenGlShaderProgram.h"
#include "hc/hcBuiltInShaders.h"
#include "hc/hcOpenGlTexture.h"

namespace hc
{
  OpenGlGraphicsManager::OpenGlGraphicsManager()
  {
  }

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {
  }

  void OpenGlGraphicsManager::init()
  {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      throw RuntimeErrorException(
        "Failed to initialize GLEW: " +
        String(reinterpret_cast<const char*>(glewGetErrorString(err)))
      );
    }

    // Set up basic OpenGL state
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  }

  void OpenGlGraphicsManager::draw(IDrawable& drawable)
  {
    drawable.draw(*this);
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGlGraphicsManager::endFrame(IWindow& window)
  {
    window.swapBuffers();
  }

  ITextureManager& OpenGlGraphicsManager::getTextureManager()
  {
    return m_textureManager;
  }

  IMaterialManager& OpenGlGraphicsManager::getMaterialManager()
  {
    return m_materialManager;
  }

  IShaderManager& OpenGlGraphicsManager::getShaderManager()
  {
    return m_shaderManager;
  }

  SharedPtr<IShaderProgram> OpenGlGraphicsManager::createUnlitShaderProgram() const
  {
    auto vertexShader = MakeShared<OpenGlShader>(
      shaderStageType::Vertex,
      builtInShaders::UnlitVertex
    );
    vertexShader->compile();
    if (!vertexShader->isCompiled())
      return nullptr;

    auto fragmentShader = MakeShared<OpenGlShader>(
      shaderStageType::Fragment,
      builtInShaders::UnlitFragment
    );
    fragmentShader->compile();
    if (!fragmentShader->isCompiled())
      return nullptr;

    auto program = MakeShared<OpenGlShaderProgram>();
    program->attachShader(vertexShader);
    program->attachShader(fragmentShader);

    if (!program->link())
      return nullptr;

    return program;
  }

  void OpenGlGraphicsManager::resolveDependencies(DependencyContainer& container)
  {
    m_textureManager.initialize(
      container.resolve<AssetManager>()
    );
    m_materialManager.initialize(
      container.resolve<AssetManager>(),
      &m_textureManager
    );
  }

  void OpenGlGraphicsManager::destroy()
  {
  }
}
