#include "hc/editor/hcImguiHandler.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
#include "imgui_impl_win32.h"
#endif

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

      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

      ImGui::StyleColorsDark();

    #if HC_PLATFORM == HC_PLATFORM_WIN32
      void* hwnd = window.getNativeHandle();
      ImGui_ImplWin32_Init(hwnd);
    #else
      (void)window;
    #endif
      ImGui_ImplOpenGL3_Init("#version 130");
    }

    bool processEvent(const Event& evt)
    {
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

      else if (evt.is<Event::MouseWheelScrolled>())
      {
        const auto* e = evt.getIf<Event::MouseWheelScrolled>();
        if (e)
        {
          if (e->wheel == mouseWheelType::Vertical)
          {
            ImGui::GetIO().AddMouseWheelEvent(0.0f, e->delta);
          }
          else if (e->wheel == mouseWheelType::Horizontal)
          {
            ImGui::GetIO().AddMouseWheelEvent(e->delta, 0.0f);
          }
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

      else if (evt.is<Event::TextEntered>())
      {
        const auto* e = evt.getIf<Event::TextEntered>();
        if (e)
        {
          ImGui::GetIO().AddInputCharacter(static_cast<ImWchar>(e->unicode));
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

    void beginFrame(IWindow& window, const Time& elapsedTime)
    {
      ImGuiIO& io = ImGui::GetIO();

      const Vector2u windowSize = window.getSize();
      io.DisplaySize = ImVec2(
        static_cast<float>(windowSize.x), 
        static_cast<float>(windowSize.y)
      );

      const float deltaTime = elapsedTime.toSeconds();
      io.DeltaTime = (deltaTime > 0.0f) ? deltaTime : (1.0f / 60.0f);

      ImGui_ImplOpenGL3_NewFrame();
#if HC_PLATFORM == HC_PLATFORM_WIN32
      ImGui_ImplWin32_NewFrame();
#endif
      ImGui::NewFrame();
      ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    }

    void endFrame()
    {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void destroy()
    {
      ImGui_ImplOpenGL3_Shutdown();
#if HC_PLATFORM == HC_PLATFORM_WIN32
      ImGui_ImplWin32_Shutdown();
#endif
      ImGui::DestroyContext();
    }
  }
}
