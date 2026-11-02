#include "hc/editor/views/mainMenuBar/hcOpenRecentProjectMenuItem.h"

#include <imgui.h>

#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"
#include "hc/editor/services/projectManager/hcProjectManager.h"

namespace hc::editor
{
  OpenRecentProjectMenuItem::OpenRecentProjectMenuItem(
    ProjectManager& projectManager,
    EditorMetadataManager& editorMetadataManager
  ) :
    m_projectManager(projectManager),
    m_editorMetadataManager(editorMetadataManager)
  {
  }

  OpenRecentProjectMenuItem::~OpenRecentProjectMenuItem()
  {
  }

  void OpenRecentProjectMenuItem::draw()
  {
    if (ImGui::BeginMenu("Open Recent"))
    {
      const auto& recentProjects = m_editorMetadataManager.getLastOpenedProjectPaths();
      for (const auto& projectPath : recentProjects)
      {
        ImGui::PushID(projectPath.toString().c_str());

        String projectName = projectPath.filename().toString();
        if (ImGui::MenuItem(projectName.c_str()))
          m_projectManager.openProject(Path(projectPath));

        if (ImGui::IsItemHovered())
          ImGui::SetTooltip("%s", projectPath.toString().c_str());

        ImGui::PopID();
      }
      ImGui::EndMenu();
    }
  }
}
