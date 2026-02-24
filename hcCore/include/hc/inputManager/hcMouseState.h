#include "hc/hcCorePrerequisites.h"
#include "hc/inputManager/hcMouseScrollState.h"

namespace hc
{
  class IInputManager;

  class HC_CORE_EXPORT MouseState : public NonCopyable
  {
  public:
    MouseState();
    ~MouseState() = default;

    void reset();
    Vector2i getPosition() const;
    void setPosition(const Vector2i& position);
    MouseScrollState& getScrollState();

  private:
    MouseScrollState m_scrollState;
    Vector2i m_position;

    friend class IInputManager;
  };
}
