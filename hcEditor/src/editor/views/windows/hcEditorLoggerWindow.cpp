#include "hc/editor/views/windows/hcEditorLoggerWindow.h"

#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistory.h"
#include "imgui.h"

namespace hc::editor
{
  EditorLoggerWindow::EditorLoggerWindow(EditorLogHistory& logHistory) :
    AWindowView("Logger", true),
    m_logHistory(logHistory),
    m_autoScroll(true)
  {
  }

  EditorLoggerWindow::~EditorLoggerWindow()
  {
  }

  void EditorLoggerWindow::destroy()
  {
    m_logHistory.clear();
  }

  void EditorLoggerWindow::onDraw()
  {
    ImGui::Begin("Logger", &m_isOpen);

    if (ImGui::Button("Clear"))
      m_logHistory.clear();

    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &m_autoScroll);

    ImGui::Separator();
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    const auto& entries = m_logHistory.getEntries();
    for (const auto& entry : entries)
    {
      ImVec4 color;
      const char* prefix = "";

      switch (entry.type)
      {
      case editorLogHistoryEntryType::Type::Message:
        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        prefix = "[Message] ";
        break;
      case editorLogHistoryEntryType::Type::Warning:
        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        prefix = "[Warning] ";
        break;
      case editorLogHistoryEntryType::Type::Error:
        color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
        prefix = "[Error] ";
        break;
      default:
        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        break;
      }

      String formattedMessage = prefix + entry.message;
      ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::TextUnformatted(formattedMessage.c_str());
      ImGui::PopStyleColor();
    }

    if (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
  }
}
