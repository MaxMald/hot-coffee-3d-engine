#include "hc/editor/views/mainMenuBar/hcOpenRecentSceneMenuItem.h"

#include <imgui.h>

#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"

namespace hc::editor
{
  OpenRecentSceneMenuItem::OpenRecentSceneMenuItem(
    ProjectManager& projectManager,
    EditorSceneManager& editorSceneManager,
    EditorMetadataManager& editorMetadataManager
  ) :
    m_projectManager(projectManager),
    m_editorSceneManager(editorSceneManager),
    m_editorMetadataManager(editorMetadataManager)
  {
  }

  OpenRecentSceneMenuItem::~OpenRecentSceneMenuItem()
  {
  }

  void OpenRecentSceneMenuItem::draw()
  {
    Path currentProjectDirectory = m_projectManager.getCurrentProjectDirectory();

    if (ImGui::BeginMenu("Open Recent"))
    {
      const auto& recentScenes = m_editorMetadataManager
        .getProjectMetadataManager()
        .getLastOpenedScenePaths();

      for (const auto& relativeScenePath : recentScenes)
      {
        ImGui::PushID(relativeScenePath.toString().c_str());

        try
        {
          Path absoluteScenePath = relativeScenePath.toAbsolute(currentProjectDirectory);

          String sceneName = absoluteScenePath.filename().toString();
          if (ImGui::MenuItem(sceneName.c_str()))
            m_editorSceneManager.openScene(absoluteScenePath);

          if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", absoluteScenePath.toString().c_str());
        }
        catch (const Exception&)
        {
          // Handle any exceptions that may occur while converting the path
          // For now, we can simply ignore them or log them if needed
        }

        ImGui::PopID();
      }
      ImGui::EndMenu();
    }
  }
}
