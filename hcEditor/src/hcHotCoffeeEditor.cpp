#include "hc/editor/hcHotCoffeeEditor.h"

#include "hc/hcHotCoffeeEngine.h"
#include "hc/hcWindowManager.h"
#include "hc/hcIGraphicsManager.h"

namespace hc::editor
{
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

    HotCoffeeEngineSettings settings;
    settings.windowSettings.title = "HotCoffee Editor";

    HotCoffeeEngine& engine = HotCoffeeEngine::Instance();
    engine.init(settings);

    IGraphicsManager& graphicsManager = engine.getGraphicsManager();
    WindowManager& windowManager = engine.getWindowManager();
    
    SharedPtr<IWindow> window = windowManager.getWindow();
    if (!window)
      throw RuntimeErrorException("Failed to get main window.");

    while (window->isOpen())
    {
      Optional<Event> eventOpt;
      while ((eventOpt = window->pollEvent()))
      { 
        if (eventOpt->is<Event::Closed>())
        {
          window->destroy();
        }
      }

      graphicsManager.beginFrame();
      // Render
      graphicsManager.endFrame(*window);
    }

    HotCoffeeEngine::Shutdown();
  }

  HotCoffeeEditor::HotCoffeeEditor()
    : m_started(false)
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
