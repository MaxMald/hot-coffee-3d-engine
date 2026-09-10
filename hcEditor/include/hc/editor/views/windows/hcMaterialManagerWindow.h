#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class MaterialDrawersManager;

  /**
   * @brief Editor window to display the engine's material manager.
   */
  class MaterialManagerWindow : public AWindowView
  {
  public:
    /**
     * @brief Constructs a MaterialManagerWindow.
     *
     * @param materialManager Reference to the material manager providing material data.
     * @param materialDrawersManager Reference to the manager handling material drawers.
     */
    MaterialManagerWindow(
      IMaterialManager& materialManager,
      MaterialDrawersManager& materialDrawersManager
    );

    virtual ~MaterialManagerWindow();

    void destroy() override;

  private:
    IMaterialManager& m_materialManager;
    MaterialDrawersManager& m_materialDrawersManager;

    /**
     * @brief Draws the material manager window contents.
     */
    void onDraw() override;
  };
}
