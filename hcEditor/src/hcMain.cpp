#include <iostream>
#include "hc/editor/hcHotCoffeeEditor.h"

using hc::editor::HotCoffeeEditor;
using hc::ProcessResult;

int main()
{
  HotCoffeeEditor editor;

  try
  { 
    ProcessResult result = editor.initialize();
    if (!result.success)
    { 
      std::cerr << "Editor initialization failed: " << result.message << std::endl;
      return -1;
    }

    editor.run();
    editor.destroy();
  }
  catch (const std::exception& e)
  { 
    std::cerr << "Unhandled exception: " << e.what() << std::endl;
    editor.destroy();
    return -1;
  }

  return 0;
}
