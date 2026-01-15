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

  SharedPtr<IWindow> WindowManager::getWindow() const
  {
    return m_window;
  }

  SharedPtr<IWindow> WindowManager::createWindow(const WindowSettings& settings)
  {
    if (!m_windowFactory)
    {
      throw RuntimeErrorException(
        "WindowFactory dependency not resolved in WindowManager."
      );
    }

    m_window = m_windowFactory->createWindow(settings);
    return m_window;
  }

  void WindowManager::destroyWindow()
  {
    if (m_window)
    {
      m_window->destroy();
      m_window = nullptr;
    }
  }
}
