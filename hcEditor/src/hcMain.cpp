#include <iostream>
#include "hc/editor/hcHotCoffeeEditor.h"

using hc::editor::HotCoffeeEditor;
using hc::ProcessResult;

int main()
{
  try
  {
    HotCoffeeEditor editor;
    ProcessResult result = editor.initialize();
    if (!result.success)
    {
      return -1;
      std::cerr << "Editor initialization failed: " << result.message << std::endl;
    }

    editor.run();
    editor.destroy();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Unhandled exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
