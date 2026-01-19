#include "hc/editor/hcEditorLoggerWindow.h"

#include <hc/hcDependencyContainer.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcEditorLogger.h"
#include "imgui.h"

namespace hc::editor
{
  EditorLoggerWindow::EditorLoggerWindow() :
    AWindowView("Logger", true),
    m_autoScroll(true)
  {
  }

  EditorLoggerWindow::~EditorLoggerWindow()
  {
  }

  void EditorLoggerWindow::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<EditorViewsManager> editorViewsManager = container.resolve<EditorViewsManager>();
    editorViewsManager->registerView(this);

    m_editorLogger = container.resolve<EditorLogger>();
  }

  void EditorLoggerWindow::onDraw()
  {
    ImGui::Begin("Logger", &m_isOpen);

    if (ImGui::Button("Clear"))
      m_editorLogger->clear();

    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &m_autoScroll);

    ImGui::Separator();
    ImGui::BeginChild("LogRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    const auto& entries = m_editorLogger->getEntries();
    for (const auto& entry : entries)
    {
      ImVec4 color;
      const char* prefix = "";

      switch (entry.type)
      {
      case editorLoggerEntryType::Type::Message:
        color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        prefix = "[Message] ";
        break;
      case editorLoggerEntryType::Type::Warning:
        color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        prefix = "[Warning] ";
        break;
      case editorLoggerEntryType::Type::Error:
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
