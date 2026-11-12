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
    SharedPtr<IMesh> m_line;
    SharedPtr<IMesh> m_circle;
    SharedPtr<IMesh> m_square;
    SharedPtr<IMesh> m_cube;

    void draw(
      const GameObject& gameObject,
      const Camera& camera,
      const GameObject* activeGameObject
    );

    void drawSpotLightGizmo(
      const GameObject& gameObject,
      const scene::lights::Light& spotLight,
      const Camera& camera,
      bool isSelected
    );

    void drawOmniLightGizmo(
      const GameObject& gameObject,
      const scene::lights::Light& omniLight,
      const Camera& camera,
      bool isSelected
    );

    void drawDirectionalLightGizmo(
      const GameObject& gameObject,
      const scene::lights::Light& directionalLight,
      const Camera& camera,
      bool isSelected
    );

    void drawLightIcon(
      const GameObject& gameObject,
      const scene::lights::Light& light,
      const Camera& camera,
      bool isSelected
    );
  };
}
