#pragma once

#include "hc/hcCameraComponent.h"
#include "hc/hcATypedComponentFactory.h"

namespace hc
{
  /**
   * @brief Factory class for creating CameraComponent instances.
   */
  class CameraComponentFactory : public ATypedComponentFactory<CameraComponent>
  {
  public:
    /**
     * @brief Constructs a CameraComponentFactory with a reference to the
     * SceneManager.
     * 
     * @param sceneManager Reference to the SceneManager used for component
     * initialization.
     */
    CameraComponentFactory(SceneManager& sceneManager);
    ~CameraComponentFactory() override;

    /**
     * @brief Creates a new CameraComponent instance.
     *
     * @return UniquePtr to the newly created CameraComponent.
     */
    UniquePtr<CameraComponent> create() const override;

  private:
    SceneManager& m_sceneManager;
  };
}
