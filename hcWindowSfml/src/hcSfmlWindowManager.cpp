#include "hc/hcSfmlWindowManager.h"
#include "hc/hcSfmlWindow.h"

namespace hc
{
  SfmlWindowManager::SfmlWindowManager() = default;
  SfmlWindowManager::~SfmlWindowManager() = default;

  bool SfmlWindowManager::createWindow(const WindowSettings& settings)
  {
    if (m_window)
    {
      LogService::Warning("SfmlWindowManager::createWindow: Window already created.");
      return false;
    }

    m_window = MakeUnique<SfmlWindow>();
    m_window->create(settings);
    return true;
  }

  IWindow& SfmlWindowManager::getWindow() const
  {
    if (!m_window)
    {
      LogService::Error("SfmlWindowManager::getWindow: No window has been created.");
      throw RuntimeErrorException("No window has been created.");
    }

    return *m_window;
  }

  void SfmlWindowManager::destroy()
  {
    if (m_window)
    {
      m_window->destroy();
      m_window.reset();
    }
  }
}
