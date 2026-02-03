#include "hc/editor/hcABaseView.h"
#include "hc/editor/hcEditorViewsManager.h"

namespace hc::editor
{
  ABaseView::ABaseView()
  {
    EditorViewsManager::Instance().registerView(this);
  }
  
  void ABaseView::destroy()
  {
    onDestroy();
    EditorViewsManager::Instance().unregisterView(this);
  }

  void ABaseView::onDestroy()
  {
    // Base implementation does nothing
  }
}
