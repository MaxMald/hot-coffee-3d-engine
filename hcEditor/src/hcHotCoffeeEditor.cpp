#include "hc/editor/hcHotCoffeeEditor.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcWindowManager.h>
#include <hc/hcIGraphicsManager.h>
#include <hc/hcLogService.h>
#include <hc/hcSceneManager.h>

#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/hcEditorDependenciesRegister.h"
#include "hc/editor/hcEditorViewsManager.h"

namespace hc::editor
{
  static constexpr UInt32 EDITOR_LOGGER_CAPACITY = 50;

  HotCoffeeEditor* HotCoffeeEditor::_Instance = nullptr;

  HotCoffeeEditor& HotCoffeeEditor::Instance()
  {
    if (_Instance == nullptr)
    {
      throw RuntimeErrorException(
        "HotCoffeeEditor instance is not prepared. Call HotCoffeeEditor::Prepare() before accessing the instance."
      );
    }

    return *_Instance;
  }
  void HotCoffeeEditor::Prepare()
  {
    if (_Instance == nullptr)
    {
      _Instance = new HotCoffeeEditor();
      _Instance->onPrepare();
    }
  }
  void HotCoffeeEditor::Shutdown()
  {
    if (_Instance != nullptr)
    {
      _Instance->onShutdown();
      _Instance = nullptr;
    }
  }

  void HotCoffeeEditor::start()
  {
    if (m_started)
      return;
    m_started = true;

    HotCoffeeEngine::Prepare();
    prepareEditorLogger();
    initEngine();
    registerDependencies();
    resolveDependencies();
    prepareEditorScene();
    runMainLoop();
    unsubscribeEditorLogger();
    HotCoffeeEngine::Shutdown();
  }

  HotCoffeeEditor::HotCoffeeEditor() :
    m_started(false),
    m_editorLogger(nullptr)
  {
  }

  HotCoffeeEditor::~HotCoffeeEditor()
  {
  }

  void HotCoffeeEditor::onPrepare()
  {
    // Preparation logic goes here
  }

  void HotCoffeeEditor::onShutdown()
  {
    m_dependencyContainer.clear();
  }

  void HotCoffeeEditor::prepareEditorLogger()
  {
    m_editorLogger = MakeShared<EditorLogger>(EDITOR_LOGGER_CAPACITY);
    LogService::Instance().subscribe(m_editorLogger.get());
  }

  void HotCoffeeEditor::unsubscribeEditorLogger()
  {
    LogService::Instance().unsubscribe(m_editorLogger.get());
  }

  void HotCoffeeEditor::registerDependencies()
  {
    editorDependenciesRegister::registerDependencies(m_dependencyContainer);
    m_dependencyContainer.registerInstance<EditorLogger>(m_editorLogger);
  }

  void HotCoffeeEditor::resolveDependencies()
  {
    m_dependencyContainer.resolveAllDependencies();

    m_editorViewsManager = m_dependencyContainer.resolve<EditorViewsManager>();
  }

  void HotCoffeeEditor::initEngine()
  {
    HotCoffeeEngineSettings settings = hotCoffeeEngineSettingsFactory::createDefault();
    HotCoffeeEngine::Instance().init(settings);
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    HotCoffeeEngine::Instance().getSceneManager().createScene("Editor Scene");
    HotCoffeeEngine::Instance().getSceneManager().setActiveScene("Editor Scene");
  }

  void HotCoffeeEditor::runMainLoop()
  {
    IGraphicsManager& graphicsManager = HotCoffeeEngine::Instance().getGraphicsManager();
    IWindow& window = HotCoffeeEngine::Instance().getWindowManager().getWindow();

    m_editorViewsManager->initialize();
    while (window.isOpen())
    {
      Optional<Event> eventOpt;
      while ((eventOpt = window.pollEvent()))
      {
        if (eventOpt->is<Event::Closed>())
          window.destroy();

        if (m_editorViewsManager->processEvent(*eventOpt))
          continue;
      }

      graphicsManager.beginFrame();
      // Render scene here
      m_editorViewsManager->draw();
      graphicsManager.endFrame(window);
    }
    m_editorViewsManager->shutdown();
  }
}
