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

  Optional<Event> SfmlWindow::pollEvent()
  {
    if (!m_sfmlWindow)
      return Optional<Event>();

    Optional<sf::Event> optSfmlEvent = m_sfmlWindow->pollEvent();
    if (!optSfmlEvent.has_value())
      return Optional<Event>();

    return convertSfmlEvent(optSfmlEvent.value());
  }

  WindowHandle SfmlWindow::getNativeHandle() const
  {
    if (m_sfmlWindow)
      return reinterpret_cast<WindowHandle>(m_sfmlWindow->getNativeHandle());
    return nullptr;
  }

  void SfmlWindow::swapBuffers()
  {
    if (m_sfmlWindow)
      m_sfmlWindow->display();
  }

  Optional<Event> SfmlWindow::convertSfmlEvent(const sf::Event& sfmlEvent) const
  {
    if (sfmlEvent.is<sf::Event::Closed>())
    {
      return Event(Event::Closed{});
    }
    else if (sfmlEvent.is<sf::Event::Resized>())
    {
      const auto& resized = sfmlEvent.getIf<sf::Event::Resized>();
      return Event(Event::Resized{ Vector2u(resized->size.x, resized->size.y) });
    }
    else if (sfmlEvent.is<sf::Event::TextEntered>())
    {
      const auto& text = sfmlEvent.getIf<sf::Event::TextEntered>();
      return Event(Event::TextEntered{ static_cast<Char32>(text->unicode) });
    }
    else if (sfmlEvent.is<sf::Event::KeyPressed>())
    {
      const auto& key = sfmlEvent.getIf<sf::Event::KeyPressed>();
      return Event(Event::KeyPressed{
        static_cast<keyboardKey::Type>(key->code),
        key->alt,
        key->control,
        key->shift,
        key->system
      });
    }
    else if (sfmlEvent.is<sf::Event::KeyReleased>())
    {
      const auto& key = sfmlEvent.getIf<sf::Event::KeyReleased>();
      return Event(Event::KeyReleased{
        static_cast<keyboardKey::Type>(key->code),
        key->alt,
        key->control,
        key->shift,
        key->system
      });
    }
    else if (sfmlEvent.is<sf::Event::MouseButtonPressed>())
    {
      const auto& btn = sfmlEvent.getIf<sf::Event::MouseButtonPressed>();
      return Event(Event::MouseButtonPressed{
        static_cast<mouseButtonKey::Type>(btn->button),
        Vector2i(btn->position.x, btn->position.y)
      });
    }
    else if (sfmlEvent.is<sf::Event::MouseButtonReleased>())
    {
      const auto& btn = sfmlEvent.getIf<sf::Event::MouseButtonReleased>();
      return Event(Event::MouseButtonReleased{
        static_cast<mouseButtonKey::Type>(btn->button),
        Vector2i(btn->position.x, btn->position.y)
      });
    }
    else if (sfmlEvent.is<sf::Event::MouseWheelScrolled>())
    {
      const auto& wheel = sfmlEvent.getIf<sf::Event::MouseWheelScrolled>();
      return Event(Event::MouseWheelScrolled{
        static_cast<mouseWheelType::Type>(wheel->wheel),
        wheel->delta,
        Vector2i(wheel->position.x, wheel->position.y)
      });
    }
    else if (sfmlEvent.is<sf::Event::MouseMoved>())
    {
      const auto& move = sfmlEvent.getIf<sf::Event::MouseMoved>();
      return Event(Event::MouseMoved{
        Vector2i(move->position.x, move->position.y)
      });
    }

    return Optional<Event>();
  }
}
