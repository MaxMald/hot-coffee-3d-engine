#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class LightComponentDrawer : public ABaseComponentDrawer<LightComponent>
  {
  public:
    LightComponentDrawer();
    virtual ~LightComponentDrawer();

  protected:
    void onDrawComponent(LightComponent* component) override;

    /**
     * Gets the index (of the combobox) corresponding to the light's type.
     *
     * @param light The light whose type index is to be retrieved.
     *
     * @returns The index corresponding to the light's type, or -1 if the type is invalid.
     */
    Int32 getLightTypeIndex(Light* light) const;
  };
}
