#include "hc/editor/views/windows/sceneSettings/hcSceneSettingsWindow.h"

#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"
#include "hc/editor/imgui/hcImgui.h"

namespace hc::editor
{
  SceneSettingsWindow::SceneSettingsWindow(
    EditorSceneManager& editorSceneManager,
    ProjectFileDialogView& projectFileDialogView,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    AWindowView("Scene Settings", false, Vector2f(400.0f, 300.0f)),
    m_editorSceneManager(editorSceneManager),
    m_projectFileDialogView(projectFileDialogView),
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_cubeMapDescriptorExtensions({ hc::serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION })
  {
  }

  SceneSettingsWindow::~SceneSettingsWindow()
  {
  }

  void  SceneSettingsWindow::destroy()
  {
    // No specific resources to release in this implementation.
  }

  void SceneSettingsWindow::onDraw()
  {
    if (!m_editorSceneManager.isSceneOpen())
    {
      ImGui::Text("No scene is currently open. Please open a scene to edit its properties.");
      return;
    }

    Scene& scene = m_editorSceneManager.getEditorScene();
    SceneSettings& settings = scene.getSettings();

    hcImGui::DrawColorEdit3("Ambient Color", settings.ambientColor);
    ImGui::SliderFloat(
      "Ambient Intensity",
      &settings.ambientIntensity,
      0.0f,
      1.0f
    );

    ImGui::Text("Skybox");
    ImGui::Separator();

    Skybox& skybox = m_editorSceneManager.getEditorScene().getSceneSkybox();
    String cubeMapDescriptorSourcePath;

    if (skybox.hasCubeMap())
    {
      const ICubeMap& cubeMap = skybox.getCubeMap();

      if (!cubeMap.isValid())
        ImGui::Text("NOTE: Current skybox cube map is invalid. Please update the skybox with valid images.");

      Path cubeMapDescriptorPath = cubeMap.getSourcePath();
      cubeMapDescriptorSourcePath = cubeMapDescriptorPath.toGenericString();
    }

    ImGui::Text(
      "Skybox's cubemap descriptor: %s",
      cubeMapDescriptorSourcePath.empty() ? "None" : cubeMapDescriptorSourcePath.c_str()
    );

    if (ImGui::Button("Select"))
    {
      m_projectFileDialogView.openFileSelector(
        "Select Cubemap Descriptor",
        m_cubeMapDescriptorExtensions,
        [this](const Path& selectedPath)
        {
          updateSkyboxCubeMap(selectedPath);
        }
      );
    }
  }

  void SceneSettingsWindow::updateSkyboxCubeMap(const Path& cubeMapDescriptorPath)
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
