#include "hc/editor/hcGameObjectEditorWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcComponentViewManager.h"
#include "imgui.h"

namespace hc::editor
{
  GameObjectEditorWindow::GameObjectEditorWindow() :
    AWindowView("Game Object Editor", true)
  {
  }

  GameObjectEditorWindow::~GameObjectEditorWindow() = default;

  void GameObjectEditorWindow::onDraw()
  {
    const Vector<GameObject*> selectedGameObjects =
      GameObjectSelectionService::Instance().getSelectedGameObjects();

    if (selectedGameObjects.empty())
    {
      ImGui::Text("No Game Object Selected");
      return;
    }

    GameObject* gameObject = selectedGameObjects.front();
    if (!gameObject)
      return;

    String label = String::Format("Game Object: %s", gameObject->getName().c_str());
    ImGui::Text(label.c_str());
    ImGui::Separator();

    drawTransform(gameObject);
    ImGui::Separator();
    drawCreateComponentSection(gameObject);
    ImGui::Separator();
    drawComponents(gameObject);
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

  void GameObjectEditorWindow::drawCreateComponentSection(GameObject* gameObject)
  {
    if (!gameObject)
      return;

    m_createComponentSection.draw();
    if (!m_createComponentSection.wasCreationRequested())
      return;

    UniquePtr<IComponent> newComponent = m_createComponentSection
      .createComponentFromSelection();

    if (!newComponent)
      return;

    gameObject->addComponent(std::move(newComponent));
  }

  void GameObjectEditorWindow::drawComponents(GameObject* gameObject)
  {
    const Vector<UniquePtr<IComponent>>& components = gameObject->getComponents();
    for (const UniquePtr<IComponent>& component : components)
    {
      ImGui::PushID(component.get());
      ComponentViewManager::Instance().drawComponent(component.get());
      ImGui::PopID();
    }
  }
}
