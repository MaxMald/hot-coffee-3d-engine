#include "hc/hcHotCoffeeEngine.h"

namespace hc
{
  HotCoffeeEngine* HotCoffeeEngine::_Instance = nullptr;

  HotCoffeeEngine& HotCoffeeEngine::Instance()
  {
    if (_Instance == nullptr)
    {
      throw RuntimeErrorException(
        "HotCoffeeEngine instance is not prepared. Call HotCoffeeEngine::Prepare() before accessing the instance."
      );
    }
    return *_Instance;
  }

  void HotCoffeeEngine::Prepare()
  {
    if (_Instance == nullptr)
    {
      _Instance = new HotCoffeeEngine();
      _Instance->onPrepare();
    }
  }

  void HotCoffeeEngine::Shutdown()
  {
    if (_Instance != nullptr)
    {
      _Instance->onShutdown();
      _Instance = nullptr;
    }
  }

  void HotCoffeeEngine::start()
  {

  }

  void HotCoffeeEngine::onPrepare()
  {
  }

  void HotCoffeeEngine::onShutdown()
  {
  }

  HotCoffeeEngine::HotCoffeeEngine()
  {
  }

  HotCoffeeEngine::~HotCoffeeEngine()
  {
  }
}
