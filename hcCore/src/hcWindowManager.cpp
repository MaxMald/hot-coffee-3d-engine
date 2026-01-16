#include "hc/hcWindowManager.h"
#include "hc/hcIWindowFactory.h"
#include "hc/hcIWindow.h"
#include "hc/hcDependencyContainer.h"

namespace hc
{
  WindowManager::WindowManager()
  {
  }

  WindowManager::~WindowManager()
  {
  }

  void WindowManager::resolveDependencies(DependencyContainer& container)
  {
    m_windowFactory = container.resolve<IWindowFactory>();
  }

  IWindow& WindowManager::getWindow() const
  {
    if (!m_window)
      throw RuntimeErrorException("No window has been created yet.");

    return *m_window;
  }

  IWindow& WindowManager::createWindow(const WindowSettings& settings)
  {
    if (!m_windowFactory)
    {
      throw RuntimeErrorException(
        "WindowFactory dependency not resolved in WindowManager."
      );
    }

    m_window = m_windowFactory->createWindow(settings);
    return *m_window;
  }
}
