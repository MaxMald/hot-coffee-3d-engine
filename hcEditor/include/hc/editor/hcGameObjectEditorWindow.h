#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/hcCreateComponentSection.h"
#include "hc/editor/hcComponentDrawersManager.h"

namespace hc::editor
{
  class ProjectFileSelector;

  class GameObjectEditorWindow : public AWindowView
  {
  public:
    GameObjectEditorWindow(ProjectFileSelector& projectFileSelector);
    virtual ~GameObjectEditorWindow();

  protected:
    CreateComponentSection m_createComponentSection;
    ComponentDrawersManager m_componentDrawersManager;

    void onDraw() override;
    void onDestroy() override;
    void drawTransform(GameObject* gameObject);
    void drawCreateComponentSection(GameObject* gameObject);
    void drawComponents(GameObject* gameObject);
  };
}
