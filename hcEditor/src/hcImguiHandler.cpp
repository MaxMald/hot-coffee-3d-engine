#include "hc/editor/hcImguiHandler.h"

#include <hc/hcIWindow.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "hc/editor/hcHotCoffeeToImguiMappings.h"

namespace hc::editor
{
  namespace hcImguiHandler
  {
    void init(IWindow& window)
    {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void)io;

      ImGui::StyleColorsDark();

      void* hwnd = window.getNativeHandle();
      ImGui_ImplWin32_Init(hwnd);
      ImGui_ImplOpenGL3_Init("#version 130");
    }

    bool processEvent(const Event& evt)
    {
      ImGuiIO& io = ImGui::GetIO();
      bool handled = false;

      if (evt.is<Event::MouseButtonPressed>())
      {
        const auto* e = evt.getIf<Event::MouseButtonPressed>();
        if (e)
        {
          ImGui::GetIO().AddMouseButtonEvent(hotCoffeeToImgui::mapMouseButton(e->button), true);
          ImGui::GetIO().AddMousePosEvent(static_cast<float>(e->position.x), static_cast<float>(e->position.y));
          handled = true;
        }
      }

      else if (evt.is<Event::MouseButtonReleased>())
      {
        const auto* e = evt.getIf<Event::MouseButtonReleased>();
        if (e)
        {
          ImGui::GetIO().AddMouseButtonEvent(hotCoffeeToImgui::mapMouseButton(e->button), false);
          ImGui::GetIO().AddMousePosEvent(static_cast<float>(e->position.x), static_cast<float>(e->position.y));
          handled = true;
        }
      }

      else if (evt.is<Event::MouseMoved>())
      {
        const auto* e = evt.getIf<Event::MouseMoved>();
        if (e)
        {
          ImGui::GetIO().AddMousePosEvent(static_cast<float>(e->position.x), static_cast<float>(e->position.y));
          handled = true;
        }
      }

      else if (evt.is<Event::KeyPressed>())
      {
        const auto* e = evt.getIf<Event::KeyPressed>();
        if (e)
        {
          ImGui::GetIO().AddKeyEvent(hotCoffeeToImgui::mapKeyboardCode(e->keyCode), true);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Ctrl, e->control);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Shift, e->shift);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Alt, e->alt);
          handled = true;
        }
      }

      else if (evt.is<Event::KeyReleased>())
      {
        const auto* e = evt.getIf<Event::KeyReleased>();
        if (e)
        {
          ImGui::GetIO().AddKeyEvent(hotCoffeeToImgui::mapKeyboardCode(e->keyCode), false);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Ctrl, e->control);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Shift, e->shift);
          ImGui::GetIO().AddKeyEvent(ImGuiMod_Alt, e->alt);
          handled = true;
        }
      }

      return handled;
    }

    void beginFrame()
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplWin32_NewFrame();
      ImGui::NewFrame();
    }

    void endFrame()
    {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void destroy()
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplWin32_Shutdown();
      ImGui::DestroyContext();
    }
  }
}
