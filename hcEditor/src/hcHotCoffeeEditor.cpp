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
    SceneManager& sceneManager = SceneManager::Instance();
    IGraphicsManager& graphicsManager = HotCoffeeEngine::GetGraphicsManager();
    IWindow& window = HotCoffeeEngine::GetWindowManager().getWindow();
    
    EditorViewsManager::Prepare();
    EditorViewsManager::Initialize();

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

        if (EditorViewsManager::ProcessEvent(*eventOpt))
          continue;
      }

      graphicsManager.beginFrame();
      sceneManager.draw();
      EditorViewsManager::Draw();
      graphicsManager.endFrame(window);
    }

    EditorViewsManager::Shutdown();
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
    SceneManager::Instance().createScene("Editor Scene");
    SceneManager::Instance().setActiveScene("Editor Scene");
  }
}
