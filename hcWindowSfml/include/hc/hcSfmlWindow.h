#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/hcIWindow.h"

namespace sf
{
  class Window;
  class Event;
}

namespace hc
{
  /**
   * @brief Implementation of IWindow using SFML's sf::Window.
   */
  class SfmlWindow : public IWindow
  {
  public:
    SfmlWindow();
    virtual ~SfmlWindow();

    /**
     * @copydoc IWindow::create
     */
    void create(const WindowSettings& settings) override;

    /**
     * @copydoc IWindow::destroy
     */
    void destroy() override;

    /**
     * @copydoc IWindow::getSize
     */
    Vector2u getSize() const override;

    /**
     * @copydoc IWindow::setSize
     */
    void setSize(const Vector2u& size) override;

    /**
     * @copydoc IWindow::getPosition
     */
    Vector2i getPosition() const override;

    /**
     * @copydoc IWindow::setPosition
     */
    void setPosition(const Vector2i& position) override;

    /**
     * @copydoc IWindow::isOpen
     */
    bool isOpen() const override;

    /**
     * @copydoc IWindow::pollEvent
     */
    Optional<Event> pollEvent() override;

  private:
    SharedPtr<sf::Window> m_sfmlWindow;

    /**
     * @brief Converts an sf::Event to a hc::Event.
     */
    Optional<Event> convertSfmlEvent(const sf::Event& sfmlEvent) const;
  };
}
