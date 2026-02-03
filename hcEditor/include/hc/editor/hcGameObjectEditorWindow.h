#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/hcCreateComponentSection.h"

namespace hc::editor
{
  class GameObjectEditorWindow :
    public AWindowView,
    public IDependencyResolvable
  {
  public:
    GameObjectEditorWindow();
    virtual ~GameObjectEditorWindow();

    void resolveDependencies(DependencyContainer& container) override;

  protected:
    GameObjectSelectionService* m_gameObjectSelectionService;
    CreateComponentSection m_createComponentSection;

    void onDraw() override;
    void drawTransform(GameObject* gameObject);
    void drawCreateComponentSection(GameObject* gameObject);
    void drawComponents(GameObject* gameObject);
  };
}
