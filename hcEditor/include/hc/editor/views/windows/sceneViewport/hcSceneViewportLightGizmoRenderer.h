#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class SceneViewportLightGizmoRenderer
  {
  public:
    explicit SceneViewportLightGizmoRenderer(
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    );
    ~SceneViewportLightGizmoRenderer();

    void prepare();
    void setEnabled(bool enabled);
    bool isEnabled() const;

    void draw(
      const Scene& scene,
      const Camera& camera,
      const GameObject* activeGameObject
    );

  private:
    IAssetManager& m_assetManager;
    IGraphicsManager& m_graphicsManager;
    bool m_enabled;

    SharedPtr<IMesh> m_coneMesh;

    void draw(
      const GameObject& gameObject,
      const Camera& camera,
      const GameObject* activeGameObject
    );

    void drawSpotLightGizmo(
      const GameObject& gameObject,
      const SpotLightComponent& spotLightComponent,
      const Camera& camera,
      bool isSelected
    );

    void drawLightIcon(
      const GameObject& gameObject,
      const ALightComponent& lightComponent,
      const Camera& camera,
      bool isSelected
    );

    Transform computeSpotlightConeTransform(
      const SpotLight& spotLight
    ) const;
  };
}
