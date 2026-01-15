#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/hcIWindow.h"

namespace sf
{
  class Window;
}

namespace hc
{
  class SfmlWindow : public IWindow
  {
  public:
    SfmlWindow();
    virtual ~SfmlWindow();

    void create(const WindowSettings& settings) override;
    void destroy() override;

    Vector2u getSize() const override;
    void setSize(const Vector2u& size) override;
    Vector2i getPosition() const override;
    void setPosition(const Vector2i& position) override;
    bool isOpen() const override;

  private:
    SharedPtr<sf::Window> m_sfmlWindow;
  };
}
