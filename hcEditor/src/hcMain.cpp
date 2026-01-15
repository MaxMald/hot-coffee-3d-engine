#include "hc/editor/hcHotCoffeeEditor.h"

using hc::editor::HotCoffeeEditor;

int main()
{
  HotCoffeeEditor::Prepare();
  HotCoffeeEditor::Instance().start();
  HotCoffeeEditor::Shutdown();

  return 0;
}
