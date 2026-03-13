#include "hc/editor/views/windows/hcTextureManagerWindow.h"
#include "hc/editor/imgui/hcImguiUtilities.h"
#include "imgui.h"

namespace hc::editor
{
  TextureManagerWindow::TextureManagerWindow(ITextureManager& textureManager)
    : AWindowView("Texture Manager"),
    m_textureManager(textureManager)
  {
  }

  TextureManagerWindow::~TextureManagerWindow()
  {
  }

  void TextureManagerWindow::onDraw()
  {
    const auto& textures = m_textureManager.getTextures();
    if (textures.empty())
    {
      ImGui::Text("No textures loaded.");
      return;
    }

    for (const auto& texture : textures)
    {
      if (!texture)
        continue;

      String label = String::Format("Texture ID: %s", texture->getId().toString().c_str());
      if (ImGui::TreeNode(label.c_str()))
      {
        ImGui::Text("Image ID: %s", texture->getImage()->getId().toString().c_str());
        
        if (ImGui::TreeNode("Preview"))
        {
          imguiUtilities::DrawTexture(texture.get(), 500, 500);
          ImGui::TreePop();
        }

        ImGui::TreePop();
      }
    }
  }
}
