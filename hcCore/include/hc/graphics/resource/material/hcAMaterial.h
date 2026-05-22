#pragma once

#include "hc/graphics/resource/material/hcIMaterial.h"

namespace hc
{
  /**
   * @brief Abstract base class for materials.
   */
  class HC_CORE_EXPORT AMaterial : public IMaterial
  {
  public:
    ~AMaterial() override;

    /**
     * @copydoc IMaterial::getId
     */
    const Id& getId() const override final;

    /**
     * @copydoc IMaterial::getMaterialId
     */
    UInt16 getMaterialId() const override final;

    /**
     * @copydoc IMaterial::getRenderMode
     */
    materialRenderMode::Type getRenderMode() const override final;

    /**
     * @copydoc IMaterial::setRenderMode
     */
    void setRenderMode(materialRenderMode::Type renderMode) override final;

    /**
     * @copydoc IMaterial::getAlphaCutoutThreshold
     */
    float getAlphaCutoutThreshold() const override final;

    /**
     * @copydoc IMaterial::setAlphaCutoutThreshold
     */
    void setAlphaCutoutThreshold(float threshold) override final; 

    /**
     * @copydoc IMaterial::setAlphaCutoutThreshold
     */
    void setAlphaCutoutThreshold(float threshold) override final;

    /**
     * @copydoc IMaterial::isDoubleSided
     */
    bool isDoubleSided() const override final;

    /**
     * @copydoc IMaterial::setDoubleSided
     */
    void setDoubleSided(bool doubleSided) override final;

  protected:
    Id m_id;
    UInt16 m_materialId;
    materialRenderMode::Type m_renderMode;
    float m_alphaCutoutThreshold;
    bool m_doubleSided;

    AMaterial(
      UInt16 materialId,
      materialRenderMode::Type renderMode,
      float alphaCutoutThreshold,
      bool doubleSided
    );
  };
}
