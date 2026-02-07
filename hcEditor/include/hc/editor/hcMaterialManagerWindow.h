#pragma once

#include "hc/editor/hcAWindowView.h"
#include "hc/editor/hcMaterialDrawersManager.h"

namespace hc::editor
{
  /**
   * @brief Editor window to display the engine's material manager.
   */
  class MaterialManagerWindow : public AWindowView
  {
  public:
    /**
     * @brief Constructs a MaterialManagerWindow.
     *
     * @param materialManager Reference to the material manager providing
     * material data.
     * @param materialDrawersManager Unique pointer to the manager handling
     * material drawers.
     */
    MaterialManagerWindow(
      IMaterialManager& materialManager,
      UniquePtr<MaterialDrawersManager> materialDrawersManager
    );

    virtual ~MaterialManagerWindow();

  private:
    IMaterialManager& m_materialManager;
    UniquePtr<MaterialDrawersManager> m_materialDrawersManager;

    /**
     * @brief Draws the material manager window contents.
     */
    void onDraw() override;

    /**
     * @brief Performs cleanup when the window is destroyed.
     */
    void onDestroy() override;
  };
}
