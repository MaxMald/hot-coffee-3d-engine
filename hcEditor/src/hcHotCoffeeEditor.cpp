#include "hc/editor/hcHotCoffeeEditor.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcWindowManager.h>
#include <hc/hcIGraphicsManager.h>
#include <hc/hcLogService.h>

#include "hc/editor/hcImguiHandler.h"
#include "hc/editor/hcMainMenuWindow.h"
#include "hc/editor/hcHotCoffeeEngineSettingsFactory.h"

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
    LogService::Instance().subscribe(&m_editorLogger);

    HotCoffeeEngine& engine = HotCoffeeEngine::Instance();

    HotCoffeeEngineSettings settings = hotCoffeeEngineSettingsFactory::createDefault();
    engine.init(settings);

    IGraphicsManager& graphicsManager = engine.getGraphicsManager();
    IWindow& window = engine.getWindowManager().getWindow();

    hcImguiHandler::init(window);

    MainMenuWindow mainMenuWindow(engine, m_editorLogger);

    while (window.isOpen())
    {
      Optional<Event> eventOpt;
      while ((eventOpt = window.pollEvent()))
      {
        if (eventOpt->is<Event::Closed>())
          window.destroy();

        if (hcImguiHandler::processEvent(*eventOpt))
          continue;
      }

      graphicsManager.beginFrame();
      // Render scene here

      hcImguiHandler::beginFrame();
      mainMenuWindow.draw();
      hcImguiHandler::endFrame();

      graphicsManager.endFrame(window);
    }

    hcImguiHandler::destroy();

    LogService::Instance().unsubscribe(&m_editorLogger);
    HotCoffeeEngine::Shutdown();
  }

  HotCoffeeEditor::HotCoffeeEditor() :
    m_started(false),
    m_editorLogger(EDITOR_LOGGER_CAPACITY)
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
    // Shutdown logic goes here
  }
}
