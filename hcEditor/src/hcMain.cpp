#include "hc/editor/hcHotCoffeeEditor.h"

using hc::editor::HotCoffeeEditor;

int main()
{
  HotCoffeeEditor::Prepare();
  HotCoffeeEditor::Initialize();
  HotCoffeeEditor::Run();
  HotCoffeeEditor::Shutdown();
  return 0;
}
