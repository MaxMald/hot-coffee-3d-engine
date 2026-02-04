#include "hc/editor/hcCameraManagerWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  CameraManagerWindow::CameraManagerWindow() :
    AWindowView("Camera Manager", false),
    m_sceneManager(nullptr)
  {
    m_sceneManager = &(HotCoffeeEngine::Instance().getSceneManager());
  }

  CameraManagerWindow::~CameraManagerWindow() = default;

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

    // Draw Default Camera
    Camera& defaultCamera = cameraManager.getDefaultCamera();
    ImGui::PushID(&defaultCamera);
    if (ImGui::CollapsingHeader("Default Camera"))
      drawCameraController(&defaultCamera);
    ImGui::Separator();
    ImGui::PopID();

    
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

    // Position
    Vector3f position = camera->getPosition();
    if (ImGui::InputFloat3("position", &position.x, "%.2f"))
      camera->setPosition(position);

    // Direction
    Vector3f direction = camera->getDirection();
    ImGui::Text("Direction: %.2f, %.2f, %.2f", direction.x, direction.y, direction.z);

    // Up
    Vector3f up = camera->getUp();
    ImGui::Text("Up: %.2f, %.2f, %.2f", up.x, up.y, up.z);

    ImGui::Separator();
    drawCameraProjectionController(camera->getActiveProjection());
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Projection Matrix"))
    {
      imguiUtilities::DrawMatrix(
        "Projection",
        camera->getProjectionMatrix()
      );
    }

    if (ImGui::CollapsingHeader("View Matrix"))
    {
      imguiUtilities::DrawMatrix(
        "View",
        camera->getViewMatrix()
      );
    }
  }

  void CameraManagerWindow::drawCameraProjectionController(
    ICameraProjection* projection
  )
  {
    if (!projection)
      return;

    projectionType::Type type = projection->getType();
    if (type == projectionType::Perspective)
    {
      PerspectiveCameraProjection* perspectiveProj =
        static_cast<PerspectiveCameraProjection*>(projection);

      if (perspectiveProj)
        drawPerspectiveProjectionController(perspectiveProj);
    }
    else // Orthographic
    {
      OrthographicCameraProjection* orthographicProj =
        static_cast<OrthographicCameraProjection*>(projection);

      if (orthographicProj)
        drawOrthographicProjectionController(orthographicProj);
    }
  }

  void CameraManagerWindow::drawPerspectiveProjectionController(
    PerspectiveCameraProjection* projection
  )
  {
    if (!projection)
      return;
        
    ImGui::Text("Perspective Projection");

    float fovYDegrees = projection->getFovY().toDegrees();
    if (ImGui::SliderFloat("FOV (degrees)", &fovYDegrees, 1.0f, 179.0f))
      projection->setFovY(Angle::FromDegrees(fovYDegrees));

    float aspectRatio = projection->getAspectRatio();
    if (ImGui::InputFloat("Aspect Ratio", &aspectRatio, 0.1f, 1.0f, "%.2f"))
      projection->setAspectRatio(aspectRatio);

    float nearPlane = projection->getNear();
    if (ImGui::InputFloat("Near Plane", &nearPlane, 0.1f, 1.0f, "%.2f"))
      projection->setNear(nearPlane);

    float farPlane = projection->getFar();
    if (ImGui::InputFloat("Far Plane", &farPlane, 1.0f, 10000.0f, "%.2f"))
      projection->setFar(farPlane);
  }

  void CameraManagerWindow::drawOrthographicProjectionController(
    OrthographicCameraProjection* projection
  )
  {
    if (!projection)
      return;

    ImGui::Text("Orthographic Projection");

    float left = projection->getLeft();
    if (ImGui::InputFloat("Left", &left, 0.1f, 1.0f, "%.2f"))
      projection->setLeft(left);

    float right = projection->getRight();
    if (ImGui::InputFloat("Right", &right, 0.1f, 1.0f, "%.2f"))
      projection->setRight(right);

    float top = projection->getTop();
    if (ImGui::InputFloat("Top", &top, 0.1f, 1.0f, "%.2f"))
      projection->setTop(top);

    float bottom = projection->getBottom();
    if (ImGui::InputFloat("Bottom", &bottom, 0.1f, 1.0f, "%.2f"))
      projection->setBottom(bottom);

    float nearPlane = projection->getNear();
    if (ImGui::InputFloat("Near Plane", &nearPlane, 0.1f, 1.0f, "%.2f"))
      projection->setNear(nearPlane);

    float farPlane = projection->getFar();
    if (ImGui::InputFloat("Far Plane", &farPlane, 1.0f, 10000.0f, "%.2f"))
      projection->setFar(farPlane);
  }
}
