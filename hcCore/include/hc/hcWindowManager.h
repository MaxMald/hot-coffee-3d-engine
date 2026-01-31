#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcWindowSettings.h"

namespace hc
{
  class IWindowFactory;
  class IWindow;

  class HC_CORE_EXPORT WindowManager :
    public IDependencyResolvable,
    public NonCopyable
  {
  public:

    WindowManager();
    ~WindowManager() override;

    IWindow& getWindow() const;

    void resolveDependencies(DependencyContainer& container) override;

    IWindow& createWindow(const WindowSettings& settings);

  private:
    SharedPtr<IWindowFactory> m_windowFactory;
    SharedPtr<IWindow> m_window;
  };
}
