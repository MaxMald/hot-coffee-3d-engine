#include "hc/editor/hcHotCoffeeEditor.h"

#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/hcEditorViewsRegistry.h"
#include "hc/editor/hcEditorServiceManagerRegistry.h"

namespace hc::editor
{
  HotCoffeeEditor::HotCoffeeEditor() :
    m_engine(),
    m_serviceManager(),
    m_editorLogHistory(),
    m_viewsManager(),
    m_initialized(false)
  {
  }

  ProcessResult HotCoffeeEditor::initialize()
  {
    if (m_initialized)
      return ProcessResult(false, "HotCoffeeEditor is already initialized.");

    try
    {
      LogService::Prepare();
      LogService::Instance().subscribe(&m_editorLogHistory);

      ProcessResult processResult = m_engine.initialize(
        hotCoffeeEngineSettingsFactory::createDefault()
      );

      if (!processResult.success)
      {
        destroy();
        return processResult;
      }

      prepareEditorScene();
      prepareEditorServices();
      prepareEditorViews();
    }
    catch (const std::exception& e)
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

    SceneManager& sceneManager = m_engine.getSceneManager();
    IGraphicsManager& graphicsManager = m_engine.getGraphicsManager();
    IWindow& window = m_engine.getWindowManager().getWindow();

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

    m_engine.destroy();

    if (LogService::HasInstance())
      LogService::Instance().unsubscribe(&m_editorLogHistory);

    m_initialized = false;
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    m_engine.getSceneManager().createScene("Editor Scene");
    m_engine.getSceneManager().setActiveScene("Editor Scene");
  }

  void HotCoffeeEditor::prepareEditorServices()
  {
    editorServiceManagerRegistry::registerServices(m_serviceManager);
  }

  void HotCoffeeEditor::prepareEditorViews()
  {
    m_viewsManager.initialize(m_engine.getWindowManager().getWindow());
    editorViewsRegistry::registerDefaultViews(
      m_engine,
      m_viewsManager,
      m_serviceManager,
      m_editorLogHistory
    );
  }
}
