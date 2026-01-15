#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcNonCopyable.h"
#include "hc/hcIDependencyResolvable.h"
#include "hc/hcWindowSettings.h"

namespace hc
{
  class IWindowFactory;
  class IWindow;

  class WindowManager : public IDependencyResolvable, public NonCopyable
  {
  public:

    WindowManager();
    ~WindowManager() override;

    SharedPtr<IWindow> getWindow() const;

    void resolveDependencies(DependencyContainer& container) override;
    
    SharedPtr<IWindow> createWindow(const WindowSettings& settings);
    void destroyWindow();

  private:
    SharedPtr<IWindowFactory> m_windowFactory;
    SharedPtr<IWindow> m_window;
  };
}
