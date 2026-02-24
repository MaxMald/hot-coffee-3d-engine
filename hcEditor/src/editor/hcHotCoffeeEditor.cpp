#include "hc/editor/hcHotCoffeeEditor.h"
#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"
#include "hc/editor/scenes/hcEditorScene.h"

namespace hc::editor
{
  constexpr const char* DEFAULT_EDITOR_SCENE_NAME = "Editor Scene";

  HotCoffeeEditor::HotCoffeeEditor() :
    m_engine(),
    m_editorLogHistory(),
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

    m_engine.run(DEFAULT_EDITOR_SCENE_NAME);
  }

  void HotCoffeeEditor::destroy()
  {
    m_engine.destroy();

    if (LogService::HasInstance())
      LogService::Instance().unsubscribe(&m_editorLogHistory);

    m_initialized = false;
  }

  void HotCoffeeEditor::prepareEditorScene()
  {
    m_engine.getSceneManager().createScene<EditorScene>(
      DEFAULT_EDITOR_SCENE_NAME,
      m_engine,
      m_editorLogHistory
    );
  }
}
