#include "hc/editor/views/mainMenuBar/hcMenuBuilder.h"
#include "hc/editor/views/mainMenuBar/hcMenu.h"
#include "hc/editor/views/mainMenuBar/hcIMenuItem.h"

namespace hc::editor
{
  MenuBuilder::MenuBuilder()
  {
  }

  MenuBuilder::~MenuBuilder()
  {
  }

  MenuBuilder& MenuBuilder::beginMenu(const String& name)
  {
    m_menuStack.push_back(MakeUnique<Menu>(name));
    return *this;
  }

  MenuBuilder& MenuBuilder::endMenu()
  {
    assertMenuStackNotEmpty();

    if (m_menuStack.size() > 1)
    {
      auto completedMenu = std::move(m_menuStack.back());
      m_menuStack.pop_back();
      m_menuStack.back()->addItem(std::move(completedMenu));
    }

    return *this;
  }
  MenuBuilder& MenuBuilder::addMenuItem(UniquePtr<IMenuItem> menuItem)
  {
    assertMenuStackNotEmpty();

    m_menuStack.back()->addItem(std::move(menuItem));
    return *this;
  }

  UniquePtr<Menu> MenuBuilder::build()
  {
    assertMenuStackNotEmpty();

    if (m_menuStack.size() > 1)
    {
      throw RuntimeErrorException(
        "Cannot build menu. There are unclosed menus in the stack."
      );
    }

    UniquePtr<Menu> rootMenu = std::move(m_menuStack.front());
    m_menuStack.clear();
    return std::move(rootMenu);
  }

  void MenuBuilder::assertMenuStackNotEmpty() const
  {
    if (m_menuStack.empty())
    {
      throw RuntimeErrorException(
        "Menu stack is empty. Cannot perform this operation."
      );
    }
  }
}
