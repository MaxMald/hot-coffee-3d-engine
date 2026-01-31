#include "hc/editor/hcCameraManagerWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "imgui.h"

namespace hc::editor
{
  CameraManagerWindow::CameraManagerWindow() :
    AWindowView("Camera Manager", false)
  {
  }

  CameraManagerWindow::~CameraManagerWindow() = default;

  void CameraManagerWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_sceneManager = &(HotCoffeeEngine::Instance().getSceneManager());
  }

  void CameraManagerWindow::onDraw()
  {
    if (!m_sceneManager)
      return;

    Scene* activeScene = m_sceneManager->getActiveScene();
    if (!activeScene)
    {
      ImGui::Text("No active scene.");
      return;
    }

    CameraManager& cameraManager = activeScene->getCameraManager();
    const Vector<UniquePtr<Camera>>& cameras = cameraManager.getCameras();

    for (size_t i = 0; i < cameras.size(); ++i)
    {
      Camera* camera = cameras[i].get();

      ImGui::PushID(static_cast<Int32>(i));
      drawCameraController(camera);
      ImGui::Separator();
      ImGui::PopID();
    }
  }

  void CameraManagerWindow::drawCameraController(Camera* camera)
  {
    if (!camera)
      return;

    ImGui::BeginGroup();

    // Position
    Vector3f position = camera->getPosition();
    ImGui::Text("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);
    ImGui::SameLine();

    // Direction
    Vector3f direction = camera->getDirection();
    ImGui::Text("Direction: %.2f, %.2f, %.2f", direction.x, direction.y, direction.z);
    ImGui::SameLine();

    // Up
    Vector3f up = camera->getUp();
    ImGui::Text("Up: %.2f, %.2f, %.2f", up.x, up.y, up.z);

    ImGui::EndGroup();
  }
}
