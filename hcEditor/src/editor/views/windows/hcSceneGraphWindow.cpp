#include "hc/editor/views/windows/hcSceneGraphWindow.h"
#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"
#include "imgui.h"

namespace hc::editor
{
  SceneGraphWindow::SceneGraphWindow(
    SceneManager& sceneManager,
    GameObjectSelectionService& gameObjectSelectionService
  ) :
    AWindowView("Scene Graph", true),
    m_sceneManager(sceneManager),
    m_gameObjectSelectionService(gameObjectSelectionService),
    m_rootChildren(),
    m_gameObjectPopupMenuState()
  {
  }

  SceneGraphWindow::~SceneGraphWindow()
  {
  }

  void SceneGraphWindow::destroy()
  {
    m_gameObjectSelectionService.clearSelection();
  }

  void SceneGraphWindow::onDraw()
  {
    hc::Scene* scene = m_sceneManager.getScene(EditorSceneNames::CONTENT_SCENE);
    if (!scene)
    {
      ImGui::Text("No content scene.");
      return;
    }

    drawCreateLayerSection(*scene);
    drawSceneGraph(scene->getSceneGraph());
  }

  void SceneGraphWindow::drawCreateLayerSection(Scene& scene)
  {
    static char layerNameBuffer[128] = "";
    ImGui::InputText("Name", layerNameBuffer, sizeof(layerNameBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Create"))
    {
      String layerName(layerNameBuffer);
      if (layerName.empty())
        layerName = "New Game Object";

      scene.createRootGameObject(layerName);
      layerNameBuffer[0] = '\0';
    }
    ImGui::Separator();
  }

  void SceneGraphWindow::drawSceneGraph(const SceneGraph& sceneGraph)
  {
    GameObject* root = sceneGraph.getRoot();
    if (root == nullptr)
      return;

    root->getChildren(m_rootChildren);
    for (GameObject* child : m_rootChildren)
    {
      if (child)
        drawGameObjectNode(child);
    }
  }

  void SceneGraphWindow::drawGameObjectNode(GameObject* gameObject)
  {
    if (!gameObject)
      return;

    String gameObjectName = gameObject->getName();
    if (gameObjectName.empty())
      gameObjectName = "<unnamed>";

    ImGui::PushID(gameObject);

    bool isSelected = m_gameObjectSelectionService.isGameObjectSelected(gameObject);
    ImGuiTreeNodeFlags flags = isSelected ? ImGuiTreeNodeFlags_Selected : 0;
    bool open = ImGui::TreeNodeEx(gameObjectName.c_str(), flags);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
      m_gameObjectSelectionService.clearSelection();
      m_gameObjectSelectionService.selectGameObject(gameObject);
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
      ImGui::OpenPopup("GameObjectMenu");
    }

    drawGameObjectPopupMenu(gameObject);

    if (m_gameObjectPopupMenuState.clickedDelete)
    {
      m_gameObjectSelectionService.deselectGameObject(gameObject);
      GameObject* parent = gameObject->getParent();
      if (parent != nullptr)
        parent->removeChild(gameObject->getUUID());

      if (open)
        ImGui::TreePop();
      ImGui::PopID();
      return;
    }

    if (open)
    {
      Vector<GameObject*> children;
      gameObject->getChildren(children);
      for (GameObject* child : children)
      {
        if (child)
          drawGameObjectNode(child);
      }
      ImGui::TreePop();
    }

    ImGui::PopID();
  }

  void SceneGraphWindow::drawGameObjectPopupMenu(GameObject* gameObject)
  {
    m_gameObjectPopupMenuState.clear();

    if (!gameObject)
      return;

    if (ImGui::BeginPopup("GameObjectMenu"))
    {
      if (ImGui::MenuItem("Create Child"))
        gameObject->createChild("New Child");
      if (ImGui::MenuItem("Delete"))
        m_gameObjectPopupMenuState.clickedDelete = true;
      ImGui::EndPopup();
    }
  }
}
