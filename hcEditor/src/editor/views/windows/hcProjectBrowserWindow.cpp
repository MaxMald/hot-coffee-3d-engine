#include "hc/editor/views/windows/hcProjectBrowserWindow.h"

#include <filesystem>
#include "hc/editor/services/projectManager/hcProjectManager.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/views/directoryNavigator/hcDirectoryReference.h"
#include "hc/editor/views/directoryNavigator/hcFileReference.h"
#include "hc/editor/utilities/hcAssetCreator.h"
#include "hc/editor/views/windows/materialDescriptorEditorWindow/hcMaterialDescriptorEditorWindow.h"
#include "imgui.h"

namespace 
{
  constexpr const hc::Char* ASSET_TYPES[] = { "Material" };
}

namespace hc::editor
{
  ProjectBrowserWindow::ProjectBrowserWindow(
    ProjectManager& projectManager,
    MaterialDescriptorEditorWindow& matDescEditorWindow
  ) :
    AWindowView("Project Browser", true),
    m_projectManager(projectManager),
    m_matDescEditorWindow(matDescEditorWindow)
  {
    m_projectManager.subscribeListener(this);
  }

  ProjectBrowserWindow::~ProjectBrowserWindow()
  {
    m_projectManager.unsubscribeListener(this);
  }

  void ProjectBrowserWindow::destroy()
  {
    m_directoryNavigator.clear();
  }

  void ProjectBrowserWindow::onProjectOpened()
  {
    if (!m_projectManager.isProjectOpen())
      return;

    Path projectPath = m_projectManager.getCurrentProjectPath();
    Path projectDir = projectPath.parent_path();
    m_directoryNavigator.initialize(projectDir);
  }

  void ProjectBrowserWindow::onProjectClosed()
  {
    m_directoryNavigator.clear();
  }

  void ProjectBrowserWindow::onDraw()
  {
    drawAssetCreatorInterface();
    ImGui::Separator();
    drawDirectoryNavigator();
  }

  void ProjectBrowserWindow::drawDirectoryNavigator()
  {
    ImGui::Text("Project Navigator");
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (!currentDir)
    {
      ImGui::Text("No directory selected. Load a Project first.");
      return;
    }
    
    ImGui::Text("Current Directory: %s", currentDir->getFullPath().string().c_str());
    if (ImGui::Button("Back"))
    {
      m_directoryNavigator.navigateToParentDirectory();
      return;
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
    {
      refresh();
      return;
    }
    ImGui::Separator();

    // Folders
    for (const auto& subDir : currentDir->getSubDirectories())
    {
      std::string displayName = "<folder> " + subDir->getName();
      if (ImGui::Selectable(displayName.c_str()))
      {
        m_directoryNavigator.navigateToSubDirectory(subDir->getName());
        return;
      }
    }

    // Files
    for (const auto& file : currentDir->getFiles())
    {
      if (ImGui::Selectable(file->getNameWithExtension().c_str()))
      {
        tryOpenEditorForFile(*file);
      }
    }
  }

  void ProjectBrowserWindow::drawAssetCreatorInterface()
  {
    static Char fileName[256] = "";
    static Int32 selectedAssetType = 0;

    if (ImGui::CollapsingHeader("Asset Creator", ImGuiTreeNodeFlags_DefaultOpen))
    {
      ImGui::SetNextItemWidth(150.0f);
      ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));
      ImGui::SameLine();
      ImGui::SetNextItemWidth(120.0f);
      ImGui::Combo("Asset Type", &selectedAssetType, ASSET_TYPES, IM_ARRAYSIZE(ASSET_TYPES));
      ImGui::SameLine();

      if (ImGui::Button("Create Asset"))
      {
        DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
        if (!currentDir)
          return;

        if (strlen(fileName) <= 0)
          return;

        String assetType = ASSET_TYPES[selectedAssetType];
        if (assetType == "Material")
        {
          assetCreator::createMaterialDescriptor(
            combineDirectoryWithFileName(
              currentDir->getFullPath(),
              String(fileName),
              assetFileExtensions::MATERIAL_DESCRIPTOR
            )
          );
        }
        else
        {
          LogService::Error(
            String::Format(
              "Couldn't create asset. Unsupported asset type '%s' requested.",
              assetType.c_str()
            )
          );
        }
        refresh();
      }
    } // Collapsing Header
  }

  void ProjectBrowserWindow::tryOpenEditorForFile(
    const FileReference& fileReference
  )
  {
    String extension = fileReference.getExtension();
    if (extension == assetFileExtensions::MATERIAL_DESCRIPTOR)
    {
      m_matDescEditorWindow.open(fileReference.getFullPath());
    }
  }

  Path ProjectBrowserWindow::combineDirectoryWithFileName(
    const Path& directoryPath,
    const String& fileName,
    const String& extension
  )
  {
    Path fullPath = directoryPath / (fileName + extension);
    return fullPath;
  }

  void ProjectBrowserWindow::refresh()
  {
    DirectoryReference* currentDir = m_directoryNavigator.getCurrentDirectory();
    if (currentDir)
      currentDir->refresh();
  }
}
