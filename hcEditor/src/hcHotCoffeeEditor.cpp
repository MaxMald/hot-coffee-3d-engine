#include "hc/editor/hcHotCoffeeEditor.h"

#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"

namespace hc::editor
{ 
  void HotCoffeeEditor::Initialize()
  {
    HotCoffeeEditor::Instance().initialize();
  }

  void HotCoffeeEditor::Run()
  {
    HotCoffeeEditor::Instance().run();
  }

  HotCoffeeEditor::HotCoffeeEditor() :
    m_initialized(false)
  {
  }

  void HotCoffeeEditor::initialize()
  {
    if (m_initialized)
      return;
    m_initialized = true;
    
    initEngine();
    prepareEditorScene();
  }

  void HotCoffeeEditor::run()
  {
    SceneManager& sceneManager = HotCoffeeEngine::GetSceneManager();
    IGraphicsManager& graphicsManager = HotCoffeeEngine::GetGraphicsManager();
    IWindow& window = HotCoffeeEngine::GetWindowManager().getWindow();
    
    m_viewsManager.initialize(window);

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

    m_viewsManager.clear();
  }

  void HotCoffeeEditor::onPrepare()
  {
    HotCoffeeEngine::Prepare();
    EditorLogger::Prepare();
    ProjectManager::Prepare();
    GameObjectSelectionService::Prepare();
  }

  void HotCoffeeEditor::onShutdown()
  {
    GameObjectSelectionService::Shutdown();
    ProjectManager::Shutdown();
    EditorLogger::Shutdown();
    HotCoffeeEngine::Shutdown();
  }

  void HotCoffeeEditor::initEngine()
  {
    HotCoffeeEngineSettings settings = hotCoffeeEngineSettingsFactory::createDefault();
    HotCoffeeEngine::Initialize(settings);
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    HotCoffeeEngine::GetSceneManager().createScene("Editor Scene");
    HotCoffeeEngine::GetSceneManager().setActiveScene("Editor Scene");
  }
}
