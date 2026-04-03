#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  AWindowView::AWindowView(
    const String& name,
    bool isOpen,
    const Vector2f& defaultWindowSize
  ) :
    ABaseView(),
    m_windowName(name),
    m_windowSize(0.0f, 0.0f),
    m_contentSize(0.0f, 0.0f),
    m_windowPosition(0.0f, 0.0f),
    m_defaultWindowSize(defaultWindowSize.x, defaultWindowSize.y),
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
    
    ImGui::SetNextWindowSize(
      ImVec2(m_defaultWindowSize.x, m_defaultWindowSize.y),
      ImGuiCond_FirstUseEver
    );

    ImGui::Begin(m_windowName.c_str(), &m_isOpen);
    updateWindowState();
    onDraw();
    ImGui::End();
  }

  const String& AWindowView::getWindowName() const
  {
    return m_windowName;
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

  const Vector2f& AWindowView::getWindowSize() const
  {
    return m_windowSize;
  }

  const Vector2f& AWindowView::getContentSize() const
  {
    return m_contentSize;
  }

  const Vector2f& AWindowView::getWindowPosition() const
  {
    return m_windowPosition;
  }

  void AWindowView::onWindowSizeChanged(const Vector2f& newSize)
  {
    // Derived classes can override this to respond to size changes.
  }

  void AWindowView::updateWindowState()
  {
    ImVec2 currentWindowSize = ImGui::GetWindowSize();
    if (currentWindowSize.x != m_windowSize.x || currentWindowSize.y != m_windowSize.y)
    {
      m_windowSize.x = currentWindowSize.x;
      m_windowSize.y = currentWindowSize.y;
      onWindowSizeChanged(m_windowSize);
    }

    m_contentSize.x = ImGui::GetContentRegionAvail().x;
    m_contentSize.y = ImGui::GetContentRegionAvail().y;
    m_windowPosition.x = ImGui::GetWindowPos().x;
    m_windowPosition.y = ImGui::GetWindowPos().y;
  }
}
