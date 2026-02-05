#include "hc/editor/hcABaseView.h"
#include "hc/editor/hcEditorViewsManager.h"

namespace hc::editor
{
  ABaseView::ABaseView()
  {
  }
  
  void ABaseView::destroy()
  {
    onDestroy();
  }

  void ABaseView::onDestroy()
  {
    // Base implementation does nothing
  }
}
