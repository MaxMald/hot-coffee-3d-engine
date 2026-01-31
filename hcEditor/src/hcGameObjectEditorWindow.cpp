#include "hc/editor/hcGameObjectEditorWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "imgui.h"

namespace hc::editor
{
  GameObjectEditorWindow::GameObjectEditorWindow() :
    AWindowView("Game Object Editor", true),
    m_gameObjectSelectionService(nullptr)
  {
  }

  GameObjectEditorWindow::~GameObjectEditorWindow() = default;

  void GameObjectEditorWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_gameObjectSelectionService = container.resolve<GameObjectSelectionService>().get();
  }

  void GameObjectEditorWindow::onDraw()
  {
    const Vector<GameObject*> selectedGameObjects =
      m_gameObjectSelectionService->getSelectedGameObjects();

    if (selectedGameObjects.empty())
    {
      ImGui::Text("No Game Object Selected");
      return;
    }

    GameObject* gameObject = selectedGameObjects.front();

    String label = String::Format("Game Object: %s", gameObject->getName().c_str());
    ImGui::Text(label.c_str());
    ImGui::Separator();

    drawTransform(gameObject);
  }

  void GameObjectEditorWindow::drawTransform(GameObject* gameObject)
  {
    Vector3f position = gameObject->getPosition();
    Vector3f rotation = gameObject->getRotation();
    Vector3f scale = gameObject->getScale();

    if (ImGui::DragFloat3("Position", &position.x, 0.1f))
      gameObject->setPosition(position);
      
    if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f))
      gameObject->setRotation(rotation);

    if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
      gameObject->setScale(scale);

    if (ImGui::CollapsingHeader("World Matrix"))
    {
      const Matrix4 worldMatrix = gameObject->getWorldMatrix();
      for (int row = 0; row < 4; ++row)
      {
        ImGui::Text("%.3f %.3f %.3f %.3f",
          worldMatrix.m[row][0],
          worldMatrix.m[row][1],
          worldMatrix.m[row][2],
          worldMatrix.m[row][3]);
      }
    }
  }
}
