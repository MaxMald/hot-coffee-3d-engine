#include "hc/editor/hcAWindowView.h"
#include "imgui.h"

namespace hc::editor
{
  AWindowView::AWindowView(const String& name, bool isOpen) :
    m_windowName(name),
    m_isOpen(isOpen)
  {
  }

  AWindowView::~AWindowView()
  {
  }

  void AWindowView::draw()
  {
    if (!m_isOpen)
      return;
    
    ImGui::Begin(m_windowName.c_str(), &m_isOpen);
    onDraw();
    ImGui::End();
  }

  bool AWindowView::isOpen() const
  {
    return m_isOpen;
  }

  void AWindowView::setOpen(bool open)
  {
    m_isOpen = open;
  }

  bool& AWindowView::getOpenFlagReference()
  {
    return m_isOpen;
  }
}
