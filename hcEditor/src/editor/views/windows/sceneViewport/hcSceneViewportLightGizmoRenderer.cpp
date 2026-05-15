#include "hc/editor/views/windows/sceneViewport/hcSceneViewportLightGizmoRenderer.h"

namespace hc::editor
{
  SceneViewportLightGizmoRenderer::SceneViewportLightGizmoRenderer(
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_enabled(true),
    m_coneMesh(nullptr)
  {}

  SceneViewportLightGizmoRenderer::~SceneViewportLightGizmoRenderer()
  {}

  void SceneViewportLightGizmoRenderer::prepare()
  {
    IModelAssetManager& modelAssetManager = m_assetManager.getModelAssetManager();
    IMeshManager& meshManager = m_graphicsManager.getMeshManager();

    m_coneMesh = meshManager.createMeshFromModel(
      modelAssetManager.getPrimitive(primitiveModelType::Cone)
    );
  }

  void SceneViewportLightGizmoRenderer::setEnabled(bool enabled)
  {
    m_enabled = enabled;
  }

  bool SceneViewportLightGizmoRenderer::isEnabled() const
  {
    return m_enabled;
  }

  void SceneViewportLightGizmoRenderer::draw(
    const Scene& scene,
    const Camera& camera,
    const GameObject* activeGameObject
  )
  {
    if (!m_enabled)
      return;

    //(void)scene;
    //(void)camera;
    //(void)activeGameObject;

    const Vector<UniquePtr<GameObject>>& rootGameObjects = scene
      .getSceneGraph()
      .getRoots();

    for (const UniquePtr<GameObject>& rootGameObject : rootGameObjects)
    {
      if (rootGameObject)
        draw(*rootGameObject, camera, activeGameObject);
    }
  }

  void SceneViewportLightGizmoRenderer::draw(
    const GameObject& gameObject,
    const Camera& camera,
    const GameObject* activeGameObject
  )
  {
    for (IComponent* component : gameObject.getComponents())
    {
      componentType::Type componentType = component->getType();

      if (componentType == componentType::SpotLight)
      {
        SpotLightComponent* spotLightComponent =
          reinterpret_cast<SpotLightComponent*>(component);

        spotLightComponent->updateLight();

        drawSpotLightGizmo(
          gameObject,
          *spotLightComponent,
          camera,
          (&gameObject == activeGameObject)
        );
      }
      else
      {
        // TODO other light types.
      }
    }
  }

  void SceneViewportLightGizmoRenderer::drawSpotLightGizmo(
    const GameObject& gameObject,
    const SpotLightComponent& spotLightComponent,
    const Camera& camera,
    bool isSelected
  )
  {
    //(void)gameObject;
    //(void)spotLightComponent;
    //(void)camera;
    //(void)isSelected;

    // TODO:
    // - Build cone transform from world position + direction.
    // - Scale cone from outer angle and desired visualization range.
    // - Submit translucent cone mesh draw command.
    // - Optionally add direction line/arrow.

    if (!isSelected)
      return;

    const SpotLight& spotLight = spotLightComponent.getSpotLight();
    spotLight.getPosition();

    RenderContext renderContext;
    renderContext.cameraMatrices = CameraMatrices::Create(camera);
    renderContext.cameraPosition = camera.getPosition();
    renderContext.transform = gameObject.getWorldMatrix();
    renderContext.modelPosition = Matrix4::ExtractTranslation(renderContext.transform);

    renderContext.transform *= computeSpotlightConeTransform(spotLight).getMatrix();

    m_coneMesh->draw(renderContext);
  }

  void SceneViewportLightGizmoRenderer::drawLightIcon(
    const GameObject& gameObject,
    const ALightComponent& lightComponent,
    const Camera& camera,
    bool isSelected
  )
  {
    (void)gameObject;
    (void)lightComponent;
    (void)camera;
    (void)isSelected;

    // TODO:
    // - Draw billboard/icon mesh with constant-ish screen size.
  }

  Transform SceneViewportLightGizmoRenderer::computeSpotlightConeTransform(
    const SpotLight& spotLight
  ) const
  {
    Transform coneTransform;

    float coneHeight = Math::cos(spotLight.getOuterConeAngle() * Math::DegToRad);
    float coneRadius = Math::sin(spotLight.getOuterConeAngle() * Math::DegToRad);

    coneTransform.setPosition(Vector3f(0.0f, 0.0f, -coneHeight * spotLight.getRange() * 0.5f));
    coneTransform.setRotation(Vector3f(Math::HalfPi, 0.0f, 0.0f));
    coneTransform.setScale(Vector3f(coneRadius, coneHeight * spotLight.getRange(), coneRadius));

    return coneTransform;
  }
  
}
