#include "hc/hcSfmlWindow.h"

#include <SFML/Window.hpp>

namespace hc
{
  SfmlWindow::SfmlWindow()
    : m_sfmlWindow(nullptr)
  {
  }

  SfmlWindow::~SfmlWindow()
  {
    destroy();
  }

  void SfmlWindow::create(const WindowSettings& settings)
  {
    if (m_sfmlWindow)
      destroy();

    sf::VideoMode videoMode(
      {
        settings.width,
        settings.height
      }
    );

    m_sfmlWindow = MakeShared<sf::Window>(
      videoMode,
      settings.title.c_str(),
      sf::Style::Default
    );

    m_sfmlWindow->setPosition(
      sf::Vector2i(settings.x, settings.y)
    );
  }

  void SfmlWindow::destroy()
  {
    if (m_sfmlWindow)
    {
      m_sfmlWindow->close();
      m_sfmlWindow.reset();
    }
  }

  Vector2u SfmlWindow::getSize() const
  {
    if (m_sfmlWindow)
    {
      auto size = m_sfmlWindow->getSize();
      return Vector2u(size.x, size.y);
    }

    return Vector2u(0, 0);
  }

  void SfmlWindow::setSize(const Vector2u& size)
  {
    if (m_sfmlWindow)
      m_sfmlWindow->setSize(sf::Vector2u(size.x, size.y));
  }

  Vector2i SfmlWindow::getPosition() const
  {
    if (m_sfmlWindow)
    {
      auto pos = m_sfmlWindow->getPosition();
      return Vector2i(pos.x, pos.y);
    }

    return Vector2i(0, 0);
  }

  void SfmlWindow::setPosition(const Vector2i& position)
  {
    if (m_sfmlWindow)
      m_sfmlWindow->setPosition(sf::Vector2i(position.x, position.y));
  }

  bool SfmlWindow::isOpen() const
  {
    return m_sfmlWindow ? m_sfmlWindow->isOpen() : false;
  }
}
