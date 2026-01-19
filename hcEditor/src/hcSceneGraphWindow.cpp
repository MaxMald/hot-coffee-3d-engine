#include "hc/editor/hcSceneGraphWindow.h"

#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcSceneManager.h>
#include <hc/hcSceneGraph.h>
#include <hc/hcScene.h>
#include <hc/hcGameObject.h>
#include <hc/hcDependencyContainer.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcGameObjectSelectionService.h"
#include "imgui.h"

namespace hc::editor
{
  SceneGraphWindow::SceneGraphWindow() :
    AWindowView("Scene Graph", true),
    m_sceneManager(nullptr),
    m_selectionService(nullptr)
  {
  }

  SceneGraphWindow::~SceneGraphWindow()
  {
  }

  void SceneGraphWindow::resolveDependencies(DependencyContainer& container)
  {
    SharedPtr<EditorViewsManager> viewsManager = container.resolve<EditorViewsManager>();
    viewsManager->registerView(this);

    m_selectionService = container.resolve<GameObjectSelectionService>();
    m_sceneManager = &(HotCoffeeEngine::Instance().getSceneManager());
  }

  void SceneGraphWindow::onDraw()
  {
    if (m_sceneManager == nullptr)
    {
      ImGui::Text("No SceneManager assigned.");
      return;
    }

    hc::Scene* scene = m_sceneManager->getActiveScene();
    if (!scene)
    {
      ImGui::Text("No active scene.");
      return;
    }

    hc::SceneGraph& sceneGraph = scene->getSceneGraph();
    const auto& roots = sceneGraph.getRoots();

    if (roots.empty())
    {
      ImGui::Text("Scene graph is empty.");
      return;
    }

    for (const auto& pair : roots)
    {
      hc::GameObject* root = pair.second.get();
      if (root)
        drawGameObjectNode(root);
    }
  }

  void SceneGraphWindow::drawGameObjectNode(GameObject* gameObject)
  {
    String gameObjectName = gameObject->getName();
    if (gameObjectName.empty())
      gameObjectName = "<unnamed>";

    ImGui::PushID(gameObject);
    if (ImGui::TreeNode(gameObjectName.c_str()))
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
