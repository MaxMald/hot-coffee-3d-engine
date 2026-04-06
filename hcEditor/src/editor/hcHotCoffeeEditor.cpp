#include "hc/editor/hcHotCoffeeEditor.h"
#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/views/hcEditorViewsRegistry.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"

namespace hc::editor
{
  HotCoffeeEditor::HotCoffeeEditor() :
    m_engine(),
    m_editorLogHistory(),
    m_serviceManager(),
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

      m_engine.addGameLoopListener(this);
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
      throw RuntimeErrorException(
        "HotCoffeeEditor is not initialized. Call initialize() before running the editor."
      );

    m_engine.run();
  }

  void HotCoffeeEditor::destroy()
  {
    if (!m_initialized)
      return;

    m_viewsManager.destroy();
    m_serviceManager.destroy();
    m_engine.removeGameLoopListener(this);
    m_engine.destroy();

    if (LogService::HasInstance())
      LogService::Instance().unsubscribe(&m_editorLogHistory);

    m_initialized = false;
  }

  bool HotCoffeeEditor::onEvent(const Event& event)
  {
    return m_viewsManager.processEvent(event);
  }

  void HotCoffeeEditor::onBeforeSceneUpdate(const Time& elapsedTime)
  {
    m_serviceManager.update(elapsedTime);
    m_viewsManager.update(elapsedTime);
  }

  void HotCoffeeEditor::onBeforeSceneRender()
  {
    // TODO draw the scene manually
  }

  void HotCoffeeEditor::onAfterSceneRender()
  {
    m_viewsManager.draw(m_engine.getElapsedTime());
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    m_engine.getSceneManager().createScene<Scene>(
      EditorSceneNames::CONTENT_SCENE
    );
  }

  void HotCoffeeEditor::prepareEditorServices()
  {
    Scene* editorScene = m_engine.getSceneManager().getScene(
      EditorSceneNames::CONTENT_SCENE
    );

    editorServiceManagerRegistry::registerServices(
      m_engine,
      m_serviceManager,
      editorScene
    );

    m_serviceManager.prepareServices();
  }

  void HotCoffeeEditor::prepareEditorViews()
  {
    m_viewsManager.initialize(
      &(m_engine.getWindowManager().getWindow())
    );

    editorViewsRegistry::registerDefaultViews(
      m_engine,
      m_viewsManager,
      m_serviceManager,
      m_editorLogHistory
    );
  }
}
