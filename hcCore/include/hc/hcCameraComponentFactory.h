#pragma once

#include "hc/hcATypedComponentFactory.h"
#include "hc/hcCameraComponent.h"

namespace hc
{
  class SceneManager;

  /**
   * @brief Factory for creating CameraComponent instances.
   *
   * Implements the ATypedComponentFactory interface for CameraComponent,
   * providing a method to instantiate new camera components.
   */
  class CameraComponentFactory : public ATypedComponentFactory<CameraComponent>
  {
  public:
    CameraComponentFactory(SceneManager& sceneManager);
    ~CameraComponentFactory() override = default;

    /**
     * @brief Creates a new CameraComponent instance.
     * 
     * @return UniquePtr<CameraComponent> The created camera component.
     */
    UniquePtr<CameraComponent> createComponent() const override;

  private:
    SceneManager& m_sceneManager;
  };
}
