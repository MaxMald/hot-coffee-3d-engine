#include "hc/editor/hcGameObjectEditorWindow.h"

#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "hc/editor/hcComponentDrawersManager.h"
#include "hc/editor/hcComponentDrawersRegistry.h"
#include "hc/editor/hcProjectFileSelector.h"
#include "hc/editor/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  GameObjectEditorWindow::GameObjectEditorWindow(
    HotCoffeeEngine& hotCoffeeEngine,
    ProjectFileSelector& projectFileSelector,
    GameObjectSelectionService& gameObjectSelectionService
  ) :
    AWindowView("Game Object Editor", true),
    m_componentDrawersManager(),
    m_gameObjectSelectionService(gameObjectSelectionService)
  {
    componentDrawersRegistry::registryDefaultComponentDrawers(
      hotCoffeeEngine,
      m_componentDrawersManager,
      projectFileSelector
    );
  }

  GameObjectEditorWindow::~GameObjectEditorWindow() = default;

  void GameObjectEditorWindow::onDraw()
  {
    const Vector<GameObject*> selectedGameObjects =
      m_gameObjectSelectionService.getSelectedGameObjects();

    if (selectedGameObjects.empty())
    {
      ImGui::Text("No Game Object Selected");
      return;
    }

    GameObject* gameObject = selectedGameObjects.front();
    if (!gameObject)
      return;

    String name = gameObject->getName();
    if (imguiUtilities::DrawInputText("Name", name))
      gameObject->setName(name);

    ImGui::Separator();
    drawTransform(gameObject);
    ImGui::Separator();
    drawCreateComponentSection(gameObject);
    ImGui::Separator();
    drawComponents(gameObject);
  }

  void GameObjectEditorWindow::onDestroy()
  {
    m_componentDrawersManager.clear();
  }

  void GameObjectEditorWindow::drawTransform(GameObject* gameObject)
  {
    if (!gameObject)
      return;

    if (ImGui::CollapsingHeader("Transform"))
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

      if (ImGui::TreeNode("World Matrix"))
      {
        imguiUtilities::DrawMatrix("World Matrix", gameObject->getWorldMatrix());
        ImGui::TreePop();
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

    m_createComponentSection.createComponentFromSelection(gameObject);
  }

  void GameObjectEditorWindow::drawComponents(GameObject* gameObject)
  {
    if (!gameObject)
      return;

    Vector<IComponent*> components =  gameObject->getComponents();
    for (IComponent* component : components)
    {
      ImGui::PushID(component);
      m_componentDrawersManager.drawComponent(component);
      ImGui::PopID();
    }
  }
}
