#pragma once

#include "hc/scene/lights/hcLight.h"
#include "hc/scene/gameObject/components/hcABaseComponent.h"
#include "hc/scene/gameObject/components/hcIUpdatableComponent.h"

namespace hc
{
  /**
   * @brief A component that represents a light source in the scene.
   */
  class HC_CORE_EXPORT LightComponent :
    public ABaseComponent,
    public IUpdatableComponent
  {
  public:
    LightComponent();
    ~LightComponent() override;

    /**
     * @copydoc IUpdatableComponent::preUpdate
     */
    void preUpdate(float deltaTime) override;

    /**
     * @copydoc IUpdatableComponent::update
     */
    void update(float deltaTime) override;

    /**
     * @copydoc IUpdatableComponent::postUpdate
     */
    void postUpdate(float deltaTime) override;

    /**
     * @brief Updates the light's properties based on the current state of the component.
     * This method should be called whenever the component's state changes and the light
     * needs to be updated accordingly.
     */
    void updateLight();

    /**
     * @brief Sets the type of light for this component. This will reset the light's
     * properties to their default values for the new light type.
     * @param type The new light type to set (e.g., Omni, Directional, Spot)
     */
    void setLightType(scene::lights::lightType::Type type);

    /**
     * @brief Gets the current type of light for this component.
     * @return The current light type (e.g., Omni, Directional, Spot)
     */
    inline scene::lights::lightType::Type getLightType() const
    {
      return m_light.getType();
    }

    /**
     * @brief Retrieves a reference to the light associated with this component.
     * @return A reference to the Light object.
     */
    inline scene::lights::Light& getLight()
    {
      return m_light;
    }

    /**
     * @brief Retrieves a constant reference to the light associated with this component.
     * @return A constant reference to the Light object.
     */
    inline const scene::lights::Light& getLight() const
    {
      return m_light;
    }

  protected:
    scene::lights::Light m_light; ///< The light associated with this component.

    /**
     * @copydoc ABaseComponent::onSerialize
     */
    void onSerialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc ABaseComponent::onDeserialize
     */
    void onDeserialize(io::BinaryReader& reader) override;

    /**
     * @copydoc ABaseComponent::getDerivedVersion
     */
    UInt16 getDerivedVersion() const override;
  };
}
