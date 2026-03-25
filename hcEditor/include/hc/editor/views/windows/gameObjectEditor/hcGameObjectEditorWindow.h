#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"
#include "hc/editor/services/gameObjectSelection/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/views/windows/gameObjectEditor/hcCreateComponentSection.h"
#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcComponentDrawersManager.h"

namespace hc::editor
{
  class ProjectFileDialogView;
  class GameObjectSelectionService;

  class GameObjectEditorWindow : public AWindowView
  {
  public:
    GameObjectEditorWindow(
      HotCoffeeEngine& hotCoffeeEngine,
      ProjectFileDialogView& projectFileSelector,
      GameObjectSelectionService& gameObjectSelectionService
    );
    virtual ~GameObjectEditorWindow();

  protected:
    GameObjectSelectionService& m_gameObjectSelectionService;
    CreateComponentSection m_createComponentSection;
    ComponentDrawersManager m_componentDrawersManager;
    Vector<IComponent*> m_gameObjectComponents;

    void onDraw() override;
    void onDestroy() override;
    void drawTransform(GameObject* gameObject);
    void drawCreateComponentSection(GameObject* gameObject);
    void drawComponents(GameObject* gameObject);
  };
}
