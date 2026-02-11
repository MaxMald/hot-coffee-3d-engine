#include "hc/editor/hcHotCoffeeEditor.h"

#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/hcEditorViewsRegistry.h"
#include "hc/editor/hcEditorServiceManagerRegistry.h"

namespace hc::editor
{ 
  HotCoffeeEditor::HotCoffeeEditor() :
    m_initialized(false),
    m_viewsManager(),
    m_editorLogHistory(),
    m_serviceManager()
  {
  }

  ProcessResult HotCoffeeEditor::initialize()
  {
    if (m_initialized)
      return ProcessResult(false, "HotCoffeeEditor is already initialized.");
    
    try
    {
      HotCoffeeEngine::Prepare();
      LogService::Instance().subscribe(&m_editorLogHistory);

      ProcessResult processResult = HotCoffeeEngine::Initialize(
        hotCoffeeEngineSettingsFactory::createDefault()
      );

      if (!processResult.success)
        return processResult;

      prepareEditorScene();
      prepareEditorServices();
      prepareEditorViews();
    }
    catch(const std::exception& e)
    {
      destroy();
      return ProcessResult(false, e.what());
    }

    m_initialized = true;
    return ProcessResult(true);
  }

  void HotCoffeeEditor::run()
  {
    if (!m_initialized)
    {
      throw RuntimeErrorException(
        "HotCoffeeEditor is not initialized. Call initialize() before run()."
      );
    }

    SceneManager& sceneManager = HotCoffeeEngine::GetSceneManager();
    IGraphicsManager& graphicsManager = HotCoffeeEngine::GetGraphicsManager();
    IWindow& window = HotCoffeeEngine::GetWindowManager().getWindow();

    while (window.isOpen())
    {
      Optional<Event> eventOpt;
      while ((eventOpt = window.pollEvent()))
      {
        if (eventOpt->is<Event::Closed>())
        {
          window.destroy();
          return;
        }

        if (m_viewsManager.processEvent(*eventOpt))
          continue;
      }

      graphicsManager.beginFrame();
      sceneManager.draw();
      m_viewsManager.draw();
      graphicsManager.endFrame(window);
    }
  }

  void HotCoffeeEditor::destroy()
  {
    m_viewsManager.clear();
    m_serviceManager.clear();
    LogService::Instance().unsubscribe(&m_editorLogHistory);
    HotCoffeeEngine::Shutdown();
    m_initialized = false;
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    HotCoffeeEngine::GetSceneManager().createScene("Editor Scene");
    HotCoffeeEngine::GetSceneManager().setActiveScene("Editor Scene");
  }

  void HotCoffeeEditor::prepareEditorServices()
  {
    editorServiceManagerRegistry::registerServices(m_serviceManager);
  }

  void HotCoffeeEditor::prepareEditorViews()
  {
    m_viewsManager.initialize(HotCoffeeEngine::GetWindowManager().getWindow());
    editorViewsRegistry::registerDefaultViews(
      HotCoffeeEngine::Instance(),
      m_viewsManager,
      m_serviceManager,
      m_editorLogHistory
    );
  }
}
