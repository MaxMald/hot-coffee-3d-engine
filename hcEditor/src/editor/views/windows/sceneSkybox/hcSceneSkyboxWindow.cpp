#include "hc/editor/views/windows/sceneSkybox/hcSceneSkyboxWindow.h"

#include <imgui.h>
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  SceneSkyboxWindow::SceneSkyboxWindow(
    EditorSceneManager& editorSceneManager,
    ProjectFileDialogView& projectFileDialogView,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    AWindowView("Scene Skybox", false, Vector2f(400.0f, 300.0f)),
    m_editorSceneManager(editorSceneManager),
    m_projectFileDialogView(projectFileDialogView),
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_cubeMapDescriptorExtensions({ hc::serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION })
  {}

  SceneSkyboxWindow::~SceneSkyboxWindow()
  {
    destroy();
  }

  void SceneSkyboxWindow::destroy()
  {}

  void SceneSkyboxWindow::onDraw()
  {
    if (!m_editorSceneManager.isSceneOpen())
    {
      ImGui::Text("No scene is currently open. Please open a scene to edit its skybox.");
      return;
    }

    Skybox& skybox = m_editorSceneManager.getEditorScene().getSceneSkybox();
    String cubeMapDescriptorSourcePath;

    if (skybox.hasCubeMap())
    {
      const ICubeMap& cubeMap = skybox.getCubeMap();
      cubeMapDescriptorSourcePath = cubeMap.getCubeMapDescriptorSourcePath().generic_string();

      if (!cubeMap.isValid())
        ImGui::Text("NOTE: Current skybox cube map is invalid. Please update the skybox with valid images.");
    }

    ImGui::Text(
      "Current Cube Map Descriptor Source Path: %s",
      cubeMapDescriptorSourcePath.empty() ? "None" : cubeMapDescriptorSourcePath.c_str()
    );

    if (ImGui::Button("Select CubeMap"))
    {
      m_projectFileDialogView.openFileSelector(
        "Select Cube Map Descriptor",
        m_cubeMapDescriptorExtensions,
        [this](const Path& selectedPath)
        {
          updateSkyboxCubeMap(selectedPath);
        },
        false
      );
    }

    ImGui::SameLine();

    if (ImGui::Button("Ok"))
    {
      setOpen(false);
    }
  }

  void SceneSkyboxWindow::updateSkyboxCubeMap(const Path& cubeMapDescriptorPath)
  {
    try
    {
      if (!m_editorSceneManager.isSceneOpen())
        throw RuntimeErrorException("No scene is currently open. Cannot update skybox cube map.");

      SharedPtr<ICubeMap> newCubeMap = CubeMapFactory::CreateFromDescriptor(
        cubeMapDescriptorPath,
        m_assetManager,
        m_graphicsManager
      );

      if (!newCubeMap)
        throw RuntimeErrorException(
          "Failed to create a cube map from the selected descriptor. Please check the descriptor and its referenced images."
        );

      if (!newCubeMap->isValid())
        throw RuntimeErrorException(
          "Failed to create a valid cube map from the selected descriptor. Please check the descriptor and its referenced images."
        );

      Skybox& skybox = m_editorSceneManager.getEditorScene().getSceneSkybox();
      skybox.destroy();
      skybox.initialize(newCubeMap);
    }
    catch (const Exception& e)
    {
      LogService::Error(String("Error updating skybox cube map: ") + e.what());
    }
  }
}
