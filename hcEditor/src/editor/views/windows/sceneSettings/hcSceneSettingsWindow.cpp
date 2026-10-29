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
    m_skyboxSourceExtensions()
  {
    m_skyboxSourceExtensions.push_back(
      hc::serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION
    );

    for (const Char* imageExtension : assetFileExtensions::SUPPORTED_IMAGES_EXTENSIONS)
      m_skyboxSourceExtensions.push_back(imageExtension);
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

    ImGui::Text("Scene's Skybox");
    ImGui::Separator();

    Skybox& skybox = scene.getSceneSkybox();
    if (skybox.hasCubeMap())
    {
      const ICubeMap& cubeMap = skybox.getCubeMap();

      if (!cubeMap.isValid())
        ImGui::Text("NOTE: Current skybox cube map is invalid. Please update the skybox with valid images.");
    }

    if (ImGui::Button("Select"))
    {
      m_projectFileDialogView.openFileSelector(
        "Select Skybox Source",
        m_skyboxSourceExtensions,
        [this](const Path& selectedPath)
        {
          onSourcePathSelected(selectedPath);
        }
      );
    }

    ImGui::SameLine();

    Path skyboxSourcePath = settings.skyboxSettings.sourcePath;
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
    {
      skybox.clear();
      settings.skyboxSettings.sourcePath.clear();
    }

    if (ImGui::CollapsingHeader("Environment CubeMap Generator Settings"))
    {
      hc::graphics::generators::EquirectangularToCubeMapGeneratorSettings& equiToCubeMapSettings
        = settings.skyboxSettings.cubeMapGeneratorSettings;

      ImGui::Text("Cube Axis Inversion: ");

      invertX = equiToCubeMapSettings.invert.x != 1.0f;
      ImGui::PushID("CubeMapGenInvertX");
      if (ImGui::Checkbox("X", &invertX))
        equiToCubeMapSettings.invert.x = invertX ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the X-Axis of the cube vertices.");
      ImGui::PopID();

      ImGui::SameLine();

      invertY = equiToCubeMapSettings.invert.y != 1.0f;
      ImGui::PushID("CubeMapGenInvertY");
      if (ImGui::Checkbox("Y", &invertY))
        equiToCubeMapSettings.invert.y = invertY ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the Y-Axis of the cube vertices.");
      ImGui::PopID();

      ImGui::SameLine();
      invertZ = equiToCubeMapSettings.invert.z != 1.0f;
      ImGui::PushID("CubeMapGenInvertZ");
      if (ImGui::Checkbox("Z", &invertZ))
        equiToCubeMapSettings.invert.z = invertZ ? -1.0f : 1.0f;
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Invert the Z-Axis of the cube vertices.");
      ImGui::PopID();

      bool useCustomFaceSize = equiToCubeMapSettings.useCustomFaceSize;
      if (ImGui::Checkbox("Use Custom Face Size", &useCustomFaceSize))
        equiToCubeMapSettings.useCustomFaceSize = useCustomFaceSize;

      if (useCustomFaceSize)
      {
        Int32 customFaceSize = static_cast<Int32>(equiToCubeMapSettings.customFaceSize);

        ImGui::PushID("CubeMapGenCustomFaceSize");
        if (ImGui::InputInt("Custom Face Size", &customFaceSize))
        {
          customFaceSize = Math::Max(1, customFaceSize);
          equiToCubeMapSettings.customFaceSize = static_cast<UInt32>(customFaceSize);
        }
        ImGui::PopID();

        if (!Math::IsPowerOfTwo(equiToCubeMapSettings.customFaceSize))
        {
          ImGui::TextColored(
            ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
            "Warning: Custom face size must be a power of two."
          );
        }
      }
    }

    if (ImGui::CollapsingHeader("Diffuse Irradiance Cube Map Generator"))
    {
      hc::graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings& diffuseIrradianceSettings
        = settings.skyboxSettings.diffuseIrradianceCubeMapGeneratorSettings;

      float sampleDelta = diffuseIrradianceSettings.sampleDelta;
      if (ImGui::InputFloat(
        "Sample Delta",
        &sampleDelta,
        0.005f
      ))
      {
        diffuseIrradianceSettings.sampleDelta = Math::Min(
          graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings::MAXIMUM_SAMPLE_DELTA,
          Math::Max(
            graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings::MINIMUM_SAMPLE_DELTA,
            sampleDelta
          )
        );
      }

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          String::Format(
            "The sample delta. Lower values result in higher quality but take longer to compute. Must be between %f and %f.",
            graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings::MINIMUM_SAMPLE_DELTA,
            graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings::MAXIMUM_SAMPLE_DELTA
          ).c_str()
        );

      float sourceMipLevel = diffuseIrradianceSettings.sourceMipLevel;
      if (ImGui::InputFloat(
        "Source Mip Level",
        &sourceMipLevel,
        0.1f
      ))
      {
        diffuseIrradianceSettings.sourceMipLevel = Math::Max(0.0f, sourceMipLevel);
      }

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          "The mip level of the source cube map to sample from. Default is 0 (highest resolution). Increase to reduce \"firefly\" artifacts."
        );

      Int32 customFaceSize = static_cast<Int32>(diffuseIrradianceSettings.faceSize);

      ImGui::PushID("DiffuseIrradianceCustomFaceSize");
      if (ImGui::InputInt("Custom Face Size", &customFaceSize))
      {
        customFaceSize = Math::Max(1, customFaceSize);
        diffuseIrradianceSettings.faceSize = static_cast<UInt32>(customFaceSize);
      }
      ImGui::PopID();

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          "The size of the face for the Diffuse Irradiance Cube Map. Usually a low resolution is sufficient. Must be power of two."
        );

      if (!Math::IsPowerOfTwo(diffuseIrradianceSettings.faceSize))
      {
        ImGui::TextColored(
          ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
          "Warning: Custom face size must be a power of two."
        );
      }
    }

    if (ImGui::CollapsingHeader("Prefiltered Environment Cube Map Generator"))
    {
      graphics::generators::PrefilteredEnvironmentCubeMapGeneratorSettings& prefilteredEnvSettings
        = settings.skyboxSettings.prefilteredEnvironmentCubeMapGeneratorSettings;

      // - Face Size

      Int32 faceSize = static_cast<Int32>(prefilteredEnvSettings.faceSize);
      ImGui::PushID("PrefilteredEnvMapGenFaceSize");
      if (ImGui::InputInt("Face Size", &faceSize))
        prefilteredEnvSettings.faceSize = static_cast<UInt32>(Math::Max(1, faceSize));
      ImGui::PopID();

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          "The size of each face of the generated cube map. Must be a power of two."
        );

      if (!Math::IsPowerOfTwo(prefilteredEnvSettings.faceSize))
      {
        ImGui::TextColored(
          ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
          "Warning: Custom face size must be a power of two."
        );
      }

      // - Number of Samples

      Int32 numSamples = static_cast<Int32>(prefilteredEnvSettings.numSamples);
      ImGui::PushID("PrefilteredEnvMapGenNumSamples");
      if (ImGui::InputInt("Num Samples", &numSamples))
        prefilteredEnvSettings.numSamples = static_cast<UInt32>(Math::Max(1, numSamples));
      ImGui::PopID();

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          "The number of samples to use for prefiltering. Higher values yield better quality but are more computationally expensive."
        );

      // - Maximum Mip Levels

      Int32 maxMipLevels = static_cast<Int32>(prefilteredEnvSettings.maxMipLevels);
      ImGui::PushID("PrefilteredEnvMapGenMaxMipLevels");
      if (ImGui::InputInt("Max Mip Levels", &maxMipLevels))
        prefilteredEnvSettings.maxMipLevels = static_cast<UInt32>(Math::Max(1, maxMipLevels));
      ImGui::PopID();

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip(
          "The maximum number of mip levels to generate for the prefiltered environment cube map."
        );
    }

    if (ImGui::Button("Regenerate Skybox"))
      regenerateSkybox(Path(skyboxSourcePath));
  }

  void SceneSettingsWindow::onSourcePathSelected(
    const Path& sourcePath
  )
  {
    try
    {
      regenerateSkybox(sourcePath);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String("Error updating skybox from equirectangular image: ") + e.what()
      );
    }
  }

  void SceneSettingsWindow::regenerateSkybox(const Path& sourcePath)
  {
    if (sourcePath.empty())
    {
      LogService::Error("Cannot regenerate skybox: source path is empty.");
      return;
    }

    if (!m_editorSceneManager.isSceneOpen())
    {
      LogService::Error(
        "No scene is currently open. Cannot update skybox from equirectangular image."
      );
      return;
    }

    Scene& scene = m_editorSceneManager.getEditorScene();
    SceneSettings& sceneSettings = scene.getSettings();
    Path previousSkyboxSourcePath = sceneSettings.skyboxSettings.sourcePath;
    Skybox& skybox = scene.getSceneSkybox();

    try
    {
      Path resolvedPath = sourcePath;
      if (resolvedPath.isAbsolute())
        resolvedPath = sourcePath.toRelative(m_assetManager.getRootPath());
      
      sceneSettings.skyboxSettings.sourcePath = resolvedPath;
      
      skybox.destroy();
      skybox.initialize(
        m_assetManager,
        m_graphicsManager,
        sceneSettings.skyboxSettings
      );
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String("Error while updating skybox: ") + e.what()
      );
      sceneSettings.skyboxSettings.sourcePath = previousSkyboxSourcePath;
      skybox.destroy();
    }
  }
}
