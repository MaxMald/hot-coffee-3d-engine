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

    ImGui::Text("Scene's Skybox");
    ImGui::Separator();

    Skybox& skybox = m_editorSceneManager.getEditorScene().getSceneSkybox();
    String skyboxSourcePath = skybox.sourcePath.toGenericString();

    if (skybox.hasCubeMap())
    {
      const ICubeMap& cubeMap = skybox.getCubeMap();

      if (!cubeMap.isValid())
        ImGui::Text("NOTE: Current skybox cube map is invalid. Please update the skybox with valid images.");
    }

    hcImGui::Filename("Skybox's source", skyboxSourcePath);

    ImGui::Text("Skybox Axis Inversion: ");

    bool invertX = settings.skyboxInvert.x != 1.0f;
    if (ImGui::Checkbox("X", &invertX))
      settings.skyboxInvert.x = invertX ? -1.0f : 1.0f;
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Invert the X-Axis of the skybox cubemap direction sample.");

    ImGui::SameLine();

    bool invertY = settings.skyboxInvert.y != 1.0f;
    if (ImGui::Checkbox("Y", &invertY))
      settings.skyboxInvert.y = invertY ? -1.0f : 1.0f;
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Invert the Y-Axis of the skybox cubemap direction sample.");

    ImGui::SameLine();
    bool invertZ = settings.skyboxInvert.z != 1.0f;
    if (ImGui::Checkbox("Z", &invertZ))
      settings.skyboxInvert.z = invertZ ? -1.0f : 1.0f;
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("Invert the Z-Axis of the skybox cubemap direction sample.");

    if (ImGui::Button("Clear Skybox"))
      skybox.clear();

    ImGui::Text("Skybox CubeMap Descriptor");
    ImGui::Separator();
    if (ImGui::Button("Select CubeMap Descriptor"))
    {
      m_projectFileDialogView.openFileSelector(
        "Select Cubemap Descriptor",
        m_cubeMapDescriptorExtensions,
        [this](const Path& selectedPath)
        {
          onCubeMapDescriptorSelected(selectedPath);
        }
      );
    }

    ImGui::Text("Skybox Equirectangular Image");
    ImGui::Separator();
    if (ImGui::Button("Select Equirectangular Image"))
    {
      m_projectFileDialogView.openImageFile(
        [this](const Path& selectedPath)
        {
          onEquirectangularImageSelected(selectedPath);
        }
      );
    }

    if (ImGui::CollapsingHeader("CubeMap Generator Settings"))
    {
      graphics::generators::CubeMapGeneratorSettings& cubeMapGenSettings
        = scene.getCubeMapGeneratorSettings();

      ImGui::Text("Cube Axis Inversion: ");

      invertX = cubeMapGenSettings.invert.x != 1.0f;
      ImGui::PushID("CubeMapGenInvertX");
      if (ImGui::Checkbox("X", &invertX))
        cubeMapGenSettings.invert.x = invertX ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the X-Axis of the cube vertices.");
      ImGui::PopID();

      ImGui::SameLine();

      invertY = cubeMapGenSettings.invert.y != 1.0f;
      ImGui::PushID("CubeMapGenInvertY");
      if (ImGui::Checkbox("Y", &invertY))
        cubeMapGenSettings.invert.y = invertY ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the Y-Axis of the cube vertices.");
      ImGui::PopID();

      ImGui::SameLine();
      invertZ = cubeMapGenSettings.invert.z != 1.0f;
      ImGui::PushID("CubeMapGenInvertZ");
      if (ImGui::Checkbox("Z", &invertZ))
        cubeMapGenSettings.invert.z = invertZ ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the Z-Axis of the cube vertices.");
      ImGui::PopID();

      bool useCustomFaceSize = cubeMapGenSettings.useCustomFaceSize;
      if (ImGui::Checkbox("Use Custom Face Size", &useCustomFaceSize))
        cubeMapGenSettings.useCustomFaceSize = useCustomFaceSize;

      if (useCustomFaceSize)
      {
        Int32 customFaceSize = static_cast<Int32>(cubeMapGenSettings.customFaceSize);
        if (ImGui::InputInt("Custom Face Size", &customFaceSize))
        {
          customFaceSize = Math::Max(1, customFaceSize);
          cubeMapGenSettings.customFaceSize = static_cast<UInt32>(customFaceSize);
        }

        if (!Math::IsPowerOfTwo(cubeMapGenSettings.customFaceSize))
        {
          ImGui::TextColored(
            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            "Warning: Custom face size must be a power of two."
          );
        }
      }

      if (ImGui::Button("Regenerate Skybox From Image"))
        regenerateSkyboxFromEquirectangularImage(Path(skybox.sourcePath));
    }
  }

  void SceneSettingsWindow::onCubeMapDescriptorSelected(const Path& cubeMapDescriptorPath)
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
      skybox.initialize(newCubeMap, cubeMapDescriptorPath);
    }
    catch (const Exception& e)
    {
      LogService::Error(String("Error updating skybox cube map: ") + e.what());
    }
  }

  void SceneSettingsWindow::onEquirectangularImageSelected(
    const Path& equirectangularImagePath
  )
  {
    try
    {
      if (!m_editorSceneManager.isSceneOpen())
        throw RuntimeErrorException("No scene is currently open. Cannot update skybox from equirectangular image.");

      regenerateSkyboxFromEquirectangularImage(equirectangularImagePath);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String("Error updating skybox from equirectangular image: ") + e.what()
      );
    }
  }

  void SceneSettingsWindow::regenerateSkyboxFromEquirectangularImage(
    const Path& equirectangularImagePath
  )
  {
    try
    {
      if (!m_editorSceneManager.isSceneOpen())
        throw RuntimeErrorException(
          "No scene is currently open. Cannot update skybox from equirectangular image."
        );

      if (!m_assetManager.getImageAssetManager().isSupportedImage(equirectangularImagePath))
        throw RuntimeErrorException(
          String::Format(
            "The source path of the current skybox is not a supported image format. Cannot regenerate skybox. Source Path: %s",
            equirectangularImagePath.c_str()
          )
        );

      Scene& scene = m_editorSceneManager.getEditorScene();

      SharedPtr<ICubeMap> newCubeMap = CubeMapFactory::CreateFromEquirectangularImage(
        equirectangularImagePath,
        scene.getCubeMapGeneratorSettings(),
        m_assetManager,
        m_graphicsManager
      );

      if (newCubeMap == nullptr)
        throw RuntimeErrorException(
          "Failed to create a cube map from the selected equirectangular image. Please check the image file."
        );

      if (!newCubeMap->isValid())
        throw RuntimeErrorException(
          "Failed to create a valid cube map from the selected equirectangular image. Please check the image file."
        );

      Skybox& skybox = scene.getSceneSkybox();

      try
      {
        skybox.destroy();
        skybox.initialize(newCubeMap, equirectangularImagePath);
      }
      catch (const Exception&)
      {
        skybox.destroy();
        throw;
      }
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String("Error updating skybox from equirectangular image: ") + e.what()
      );
    }
  }
}
