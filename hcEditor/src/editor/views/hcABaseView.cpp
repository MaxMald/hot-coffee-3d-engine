#include "hc/editor/views/hcABaseView.h"
#include "hc/editor/views/hcEditorViewsManager.h"

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
