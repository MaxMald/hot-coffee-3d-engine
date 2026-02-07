#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  /**
   * @brief Editor window for the engine's texture manager.
   */
  class TextureManagerWindow : public AWindowView
  {
  public:
    /**
     * @brief Constructs a TextureManagerWindow.
     *
     * @param textureManager Reference to the texture manager providing texture
     * data.
     */
    TextureManagerWindow(ITextureManager& textureManager);
    virtual ~TextureManagerWindow();

  private:
    ITextureManager& m_textureManager;

    /**
     * @brief Draws the texture manager window contents.
     */
    void onDraw() override;
  };
}
