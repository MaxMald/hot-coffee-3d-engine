#include "hc/hcSfmlWindowFactory.h"
#include "hc/hcSfmlWindow.h"

namespace hc
{
  SfmlWindowFactory::SfmlWindowFactory()
  {
  }
  
  SfmlWindowFactory::~SfmlWindowFactory()
  {
  }

  SharedPtr<IWindow> SfmlWindowFactory::createWindow(const WindowSettings& settings)
  {
    SharedPtr<IWindow> window = MakeShared<SfmlWindow>();
    window->create(settings);
    return window;
  }
}
