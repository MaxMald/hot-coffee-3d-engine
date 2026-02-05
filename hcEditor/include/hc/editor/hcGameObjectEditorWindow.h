#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcIGameObjectSelectionServiceListener.h"
#include "hc/editor/hcCreateComponentSection.h"

namespace hc::editor
{
  class GameObjectEditorWindow : public AWindowView
  {
  public:
    GameObjectEditorWindow();
    virtual ~GameObjectEditorWindow();

  protected:
    CreateComponentSection m_createComponentSection;

    void onDraw() override;
    void drawTransform(GameObject* gameObject);
    void drawCreateComponentSection(GameObject* gameObject);
    void drawComponents(GameObject* gameObject);
  };
}
