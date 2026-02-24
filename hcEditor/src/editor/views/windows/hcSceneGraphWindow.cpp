#include "hc/editor/views/windows/hcSceneGraphWindow.h"

#include "hc/editor/views/hcEditorViewsManager.h"
#include "hc/editor/services/gameObjectSelection/hcGameObjectSelectionService.h"
#include "imgui.h"

namespace hc::editor
{
  SceneGraphWindow::SceneGraphWindow(
    SceneManager& sceneManager,
    GameObjectSelectionService& gameObjectSelectionService
  ) :
    AWindowView("Scene Graph", true),
    m_sceneManager(sceneManager),
    m_gameObjectSelectionService(gameObjectSelectionService)
  {
  }

  SceneGraphWindow::~SceneGraphWindow()
  {
  }

  void SceneGraphWindow::onDraw()
  {
    hc::Scene* scene = m_sceneManager.getActiveScene();
    if (!scene)
    {
      ImGui::Text("No active scene.");
      return;
    }

    drawCreateLayerSection(*scene);
    drawSceneGraph(scene->getSceneGraph());
  }

  void SceneGraphWindow::drawCreateLayerSection(Scene& scene)
  {
    if (ImGui::CollapsingHeader("Create Root"))
    {
      static char layerNameBuffer[128] = "";
      ImGui::InputText("Root Name", layerNameBuffer, sizeof(layerNameBuffer));

      if (ImGui::Button("Create"))
      {
        String layerName(layerNameBuffer);
        if (layerName.empty())
          layerName = "New Root";

        scene.createRootGameObject(layerName);
        layerNameBuffer[0] = '\0';
      }
    }
  }

  void SceneGraphWindow::drawSceneGraph(const SceneGraph& sceneGraph)
  {
    const auto& roots = sceneGraph.getRoots();

    if (roots.empty())
    {
      ImGui::Text("Scene graph is empty.");
      return;
    }

    for (const UniquePtr<GameObject>& root : roots)
    {
      if (root)
        drawGameObjectNode(root.get());
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

    // Popup menu for creating a child
    if (ImGui::BeginPopup("GameObjectMenu"))
    {
      if (ImGui::MenuItem("Create Child"))
        gameObject->createChild("New Child");

      ImGui::EndPopup();
    }

    if (open)
    {
      const auto& children = gameObject->getChildren();
      for (const auto& child : children)
      {
        if (child)
          drawGameObjectNode(child.get());
      }
      ImGui::TreePop();
    }
    ImGui::PopID();
  }
}
