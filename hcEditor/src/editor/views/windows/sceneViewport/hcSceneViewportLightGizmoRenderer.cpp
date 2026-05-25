#include "hc/editor/views/windows/sceneViewport/hcSceneViewportLightGizmoRenderer.h"
#include "hc/editor/graphics/hcMeshShapeFactory.h"

namespace hc::editor
{
  SceneViewportLightGizmoRenderer::SceneViewportLightGizmoRenderer(
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    m_assetManager(assetManager),
    m_graphicsManager(graphicsManager),
    m_enabled(true),
    m_line(nullptr),
    m_circle(nullptr),
    m_square(nullptr),
    m_cube(nullptr)
  {}

  SceneViewportLightGizmoRenderer::~SceneViewportLightGizmoRenderer()
  {}

  void SceneViewportLightGizmoRenderer::prepare()
  {
    IModelAssetManager& modelAssetManager = m_assetManager.getModelAssetManager();
    IMeshManager& meshManager = m_graphicsManager.getMeshManager();

    m_line = MeshShapeFactory::CreateLine(
      meshManager,
      Vector3f(0.0f, 0.0f, 0.0f),
      Vector3f(0.0f, 0.0f, -1.0f),
      Color::White()
    );

    m_circle = MeshShapeFactory::CreateCircle(
      meshManager,
      0.5f,
      32,
      Color::White()
    );

    m_square = MeshShapeFactory::CreateRectangle(
      meshManager,
      1.0f,
      1.0f,
      Color::White()
    );

    m_cube = MeshShapeFactory::CreateBox(
      meshManager,
      1.0f,
      1.0f,
      1.0f,
      Color::White()
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
    if (&gameObject != activeGameObject)
    {
      // TODO:
      // Currently we skip non-selected games objects, however we may want to draw icons
      // for non-selected light game objects in the future.

      return;
    }

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
      else if (componentType == componentType::OmniLight)
      {
        OmniLightComponent* omniLightComponent =
          reinterpret_cast<OmniLightComponent*>(component);
        omniLightComponent->updateLight();

        drawOmniLightGizmo(
          gameObject,
          *omniLightComponent,
          camera,
          (&gameObject == activeGameObject)
        );
      }
      else if (componentType == componentType::DirectionalLight)
      {
        DirectionalLightComponent* directionalLightComponent =
          reinterpret_cast<DirectionalLightComponent*>(component);
        directionalLightComponent->updateLight();

        drawDirectionalLightGizmo(
          gameObject,
          *directionalLightComponent,
          camera,
          (&gameObject == activeGameObject)
        );
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
    if (!isSelected)
      return;

    const SpotLight& spotLight = spotLightComponent.getSpotLight();

    RenderContext renderContext;
    renderContext.cameraRenderData = CameraRenderData::Create(camera);
    renderContext.modelPosition = gameObject.getWorldPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Outer Circle
    float coneHeight = Math::Cos(spotLight.getOuterConeAngle().toRadians());
    float coneRadius = Math::Sin(spotLight.getOuterConeAngle().toRadians());

    coneHeight *= spotLight.getRange();
    coneRadius *= spotLight.getRange();

    Transform outerCircleTransform;
    outerCircleTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);
    outerCircleTransform.setPosition(0.0f, 0.0f, -coneHeight);
    outerCircleTransform.setScale(Vector3f(coneRadius, 1.0f, coneRadius));

    renderContext.transform = cameraWorldMatrix * outerCircleTransform.getMatrix();
    m_circle->draw(renderContext);

    // Inner Circle
    coneHeight = Math::Cos(spotLight.getInnerConeAngle().toRadians());
    coneRadius = Math::Sin(spotLight.getInnerConeAngle().toRadians());

    coneHeight *= spotLight.getRange();
    coneRadius *= spotLight.getRange();

    Transform innerCircleTransform = outerCircleTransform;
    innerCircleTransform.setScale(Vector3f(coneRadius, 1.0f, coneRadius));

    renderContext.transform = cameraWorldMatrix * innerCircleTransform.getMatrix();
    m_circle->draw(renderContext);

    // Range Line
    Transform rangeLineTransform;
    rangeLineTransform.setScale(Vector3f(1.0f, 1.0f, spotLight.getRange()));

    renderContext.transform = cameraWorldMatrix * rangeLineTransform.getMatrix();
    m_line->draw(renderContext);
  }

  void SceneViewportLightGizmoRenderer::drawOmniLightGizmo(
    const GameObject& gameObject,
    const OmniLightComponent& omniLightComponent,
    const Camera& camera,
    bool isSelected
  )
  {
    if (!isSelected)
      return;

    const OmniLight& omniLight = omniLightComponent.getOmniLight();

    RenderContext renderContext;
    renderContext.cameraRenderData = CameraRenderData::Create(camera);
    renderContext.modelPosition = gameObject.getWorldPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Ring 1
    Transform sphereTransform;
    sphereTransform.setScale(Vector3f(omniLight.getRange(), omniLight.getRange(), omniLight.getRange()));

    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext);

    // Ring 2
    sphereTransform.setRotation(0.0f, 0.0f, Math::HalfPi);
    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext);

    // Ring 3
    sphereTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);
    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext);
  }

  void SceneViewportLightGizmoRenderer::drawDirectionalLightGizmo(
    const GameObject& gameObject,
    const DirectionalLightComponent& directionalLightComponent,
    const Camera& camera,
    bool isSelected
  )
  {
    if (!isSelected)
      return;

    const DirectionalLight& directionalLight = directionalLightComponent.getDirectionalLight();

    RenderContext renderContext;
    renderContext.cameraRenderData = CameraRenderData::Create(camera);
    renderContext.modelPosition = gameObject.getWorldPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Plane
    Transform planeTransform;
    planeTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);

    renderContext.transform = cameraWorldMatrix * planeTransform.getMatrix();
    m_square->draw(renderContext);

    // Range Line
    Transform rangeLineTransform;
    rangeLineTransform.setScale(Vector3f(1.0f, 1.0f, directionalLight.getRange()));
    renderContext.transform = cameraWorldMatrix * rangeLineTransform.getMatrix();
    m_line->draw(renderContext);
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
}
