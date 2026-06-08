#include "hc/editor/views/windows/sceneSkybox/hcSceneSkyboxWindow.h"

#include <imgui.h>
#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/views/windows/sceneSkybox/hcSceneSkyboxWindow.h"

namespace hc::editor
{
  SceneSkyboxWindow::SceneSkyboxWindow(
    EditorSceneManager& editorSceneManager,
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    AWindowView("Scene Skybox", false, Vector2f(400.0f, 300.0f)),
    m_editorSceneManager(editorSceneManager),
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_faceWidth(0), m_faceHeight(0), m_faceChannels(4),
    m_rightImagePath(), m_leftImagePath(), m_topImagePath(),
    m_bottomImagePath(), m_backImagePath(), m_frontImagePath()
  {}

  SceneSkyboxWindow::~SceneSkyboxWindow()
  {}

  void SceneSkyboxWindow::onDraw()
  {
    if (!m_editorSceneManager.isSceneOpen())
    {
      clearValues();
      setOpen(false);
      return;
    }

    // Should have listeners to know if a scene is opened or closed.

    if (ImGui::Button("Cancel"))
    {
      clearValues();
      setOpen(false);
    }

    if (ImGui::Button("Update Skybox"))
    {
      if (updateSkybox())
      {
        ImGui::Text("Skybox updated successfully.");
        clearValues();
        setOpen(false);
      }
      else
      {
        ImGui::Text("Failed to update skybox. Check logs for details.");
      }
    }

    ImGui::SameLine();
  }

  bool hc::editor::SceneSkyboxWindow::updateSkybox()
  {
    Scene& currentScene = m_editorSceneManager.getEditorScene();
    if (!currentScene.hasSkybox())
    {
      try
      {
        currentScene.createSceneSkybox(m_graphicsManager);
      }
      catch (const RuntimeErrorException& e)
      {
        LogService::Error("Failed to create skybox: " + String(e.what()));
        return false;
      }
    }

    if (!currentScene.hasSkybox())
    {
      LogService::Error("Scene does not have a skybox.");
      return false;
    }

    if (m_faceWidth == 0 || m_faceHeight == 0)
    {
      LogService::Error("Invalid skybox face dimensions.");
      return false;
    }

    if (m_faceChannels != 3 && m_faceChannels != 4)
    {
      LogService::Error("Invalid skybox face channels. Must be 3 (RGB) or 4 (RGBA).");
      return false;
    }

    if (m_rightImagePath.empty() || m_leftImagePath.empty() || m_topImagePath.empty() ||
      m_bottomImagePath.empty() || m_backImagePath.empty() || m_frontImagePath.empty())
    {
      LogService::Error("All skybox face image paths must be specified.");
      return false;
    }

    IImageAssetManager& imageAssetManager = m_assetManager.getImageAssetManager();
    SharedPtr<Image> rightImage = imageAssetManager.load(m_rightImagePath);
    SharedPtr<Image> leftImage = imageAssetManager.load(m_leftImagePath);
    SharedPtr<Image> topImage = imageAssetManager.load(m_topImagePath);
    SharedPtr<Image> bottomImage = imageAssetManager.load(m_bottomImagePath);
    SharedPtr<Image> backImage = imageAssetManager.load(m_backImagePath);
    SharedPtr<Image> frontImage = imageAssetManager.load(m_frontImagePath);

    if (!rightImage)
    {
      LogService::Error("Failed to load right face image: " + m_rightImagePath.string());
      return false;
    }

    if (!leftImage)
    {
      LogService::Error("Failed to load left face image: " + m_leftImagePath.string());
      return false;
    }

    if (!topImage)
    {
      LogService::Error("Failed to load top face image: " + m_topImagePath.string());
      return false;
    }

    if (!bottomImage)
    {
      LogService::Error("Failed to load bottom face image: " + m_bottomImagePath.string());
      return false;
    }

    if (!backImage)
    {
      LogService::Error("Failed to load back face image: " + m_backImagePath.string());
      return false;
    }

    if (!frontImage)
    {
      LogService::Error("Failed to load front face image: " + m_frontImagePath.string());
      return false;
    }

    try
    {
      Skybox& skybox = currentScene.getSceneSkybox();
      skybox.initialize(
        m_faceWidth,
        m_faceHeight,
        m_faceChannels,
        *rightImage,
        *leftImage,
        *topImage,
        *bottomImage,
        *backImage,
        *frontImage
      );
    }
    catch (const Exception& e)
    {
      LogService::Error("Failed to initialize skybox: " + String(e.what()));
      return false;
    }

    return true;
  }

  void SceneSkyboxWindow::getValuesFromScene(
    const Scene& scene
  )
  {
    if (!scene.hasSkybox())
      return;

    const Skybox& skybox = scene.getSceneSkybox();

    m_faceWidth = skybox.getCubeMap().getFaceWidth();
    m_faceHeight = skybox.getCubeMap().getFaceHeight();
    m_faceChannels = skybox.getCubeMap().getChannels();

    m_rightImagePath = skybox.getRightImagePath();
    m_leftImagePath = skybox.getLeftImagePath();
    m_topImagePath = skybox.getTopImagePath();
    m_bottomImagePath = skybox.getBottomImagePath();
    m_backImagePath = skybox.getBackImagePath();
    m_frontImagePath = skybox.getFrontImagePath();
  }

  void SceneSkyboxWindow::clearValues()
  {
    m_faceWidth = 0;
    m_faceHeight = 0;
    m_faceChannels = 4;
    m_rightImagePath.clear();
    m_leftImagePath.clear();
    m_topImagePath.clear();
    m_bottomImagePath.clear();
    m_backImagePath.clear();
    m_frontImagePath.clear();
  }
}
