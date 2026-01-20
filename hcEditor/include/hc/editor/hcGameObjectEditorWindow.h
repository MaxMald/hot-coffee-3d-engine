#pragma once

#include <hc/hcIDependencyResolvable.h>
#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIGameObjectSelectionServiceListener.h"

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

    void onDraw() override;
    void drawTransform(GameObject* gameObject);
  };
}
