#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcKeyboardKey.h"
#include "hc/hcMouseButtonKey.h"
#include "hc/hcVector2.h"

namespace hc
{
  /**
   * @brief Represents a single input or window event.
   */
  class Event
  {
  public:
    struct KeyPressed
    {
      keyboardKey::Type keyCode{};
      bool alt{};
      bool control{};
      bool shift{};
      bool system{};
    };

    struct KeyReleased
    {
      keyboardKey::Type keyCode{};
      bool alt{};
      bool control{};
      bool shift{};
      bool system{};
    };

    struct MouseButtonPressed
    {
      mouseButtonKey::Type button{};
      Vector2i position{};
    };

    struct MouseButtonReleased
    {
      mouseButtonKey::Type button{};
      Vector2i position{};
    };

    struct MouseMoved
    {
      Vector2i position{};
    };

    struct Closed
    {
    };

    struct Resized
    {
      Vector2u size;
    };

    /**
     * @brief Variant type that holds any supported event data structure.
     */
    using EventVariant = Variant<
      KeyPressed,
      KeyReleased,
      MouseButtonPressed,
      MouseButtonReleased,
      MouseMoved,
      Closed,
      Resized
    >;

    /**
     * @brief Constructs an Event from a specific event data structure.
     * 
     * @tparam T The event data type.
     * 
     * @param event The event data instance.
     */
    template<typename T>
    Event(const T& event) : m_data(event) {}

    /**
     * @brief Checks if the event is of the specified type.
     * 
     * @tparam T The event data type to check.
     * 
     * @return True if the event holds the specified type, false otherwise.
     */
    template<typename T>
    bool is() const
    {
      return std::holds_alternative<T>(m_data);
    }

    /**
    * @brief Gets a pointer to the event data if it matches the specified type.
    * 
    * @tparam T The event data type to retrieve.
    * 
    * @return Pointer to the event data if the type matches, nullptr otherwise.
    */
    template<typename T>
    const T* getIf() const
    {
      return std::get_if<T>(&m_data);
    }

    /**
     * @brief Applies a visitor to the stored event data.
     * 
     * @tparam Visitor The visitor callable type.
     * 
     * @param visitor The visitor instance.
     * 
     * @return The result of the visitor invocation.
     */
    template<typename Visitor>
    decltype(auto) visit(Visitor&& visitor) const
    {
      return std::visit(std::forward<Visitor>(visitor), m_data);
    }

  private:
    EventVariant m_data;
  };
}
