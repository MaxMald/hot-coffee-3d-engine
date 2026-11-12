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

    GameObject* root = scene
      .getSceneGraph()
      .getRoot();

    if (root != nullptr)
      draw(*root, camera, activeGameObject);
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

      Vector<GameObject*> children;
      gameObject.getChildren(children);
      for (GameObject* child : children)
        draw(*child, camera, activeGameObject);

      return;
    }

    for (IComponent* component : gameObject.getComponents())
    {
      if (component->getType() != componentType::Light)
        return;

      LightComponent* lightComponent =
        reinterpret_cast<LightComponent*>(component);

      scene::lights::Light& light = lightComponent->getLight();
      scene::lights::lightType::Type lightType = light.getType();

      switch (lightType)
      {
      case scene::lights::lightType::Spot:
        drawSpotLightGizmo(
          gameObject,
          light,
          camera,
          (&gameObject == activeGameObject)
        );
        break;

      case scene::lights::lightType::Directional:
        drawDirectionalLightGizmo(
          gameObject,
          light,
          camera,
          (&gameObject == activeGameObject)
        );
        break;

      case scene::lights::lightType::Omni:
        drawOmniLightGizmo(
          gameObject,
          light,
          camera,
          (&gameObject == activeGameObject)
        );
        break;
      }
    }

    Vector<GameObject*> children;
    gameObject.getChildren(children);
    for (GameObject* child : children)
      draw(*child, camera, activeGameObject);
  }

  void SceneViewportLightGizmoRenderer::drawSpotLightGizmo(
    const GameObject& gameObject,
    const scene::lights::Light& light,
    const Camera& camera,
    bool isSelected
  )
  {
    if (!isSelected)
      return;

    const scene::lights::SpotLight* spotLight = light.getIfSpotLight();
    if (spotLight == nullptr)
    {
      LogService::Error(
        "SceneViewportLightGizmoRenderer: drawSpotLightGizmo called on non-spot light."
      );
      return;
    }

    RenderContext renderContext;
    renderContext.modelPosition = gameObject.getWorldPosition();
    renderContext.cameraWorldPosition = camera.getPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Outer Circle
    float coneHeight = Math::Cos(spotLight->outerConeAngle.toRadians());
    float coneRadius = Math::Sin(spotLight->outerConeAngle.toRadians());

    coneHeight *= spotLight->range;
    coneRadius *= spotLight->range;

    Transform outerCircleTransform;
    outerCircleTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);
    outerCircleTransform.setPosition(0.0f, 0.0f, -coneHeight);
    outerCircleTransform.setScale(Vector3f(coneRadius, 1.0f, coneRadius));

    renderContext.transform = cameraWorldMatrix * outerCircleTransform.getMatrix();
    m_circle->draw(renderContext, m_graphicsManager.getDrawCommandQueue());

    // Inner Circle
    coneHeight = Math::Cos(spotLight->innerConeAngle.toRadians());
    coneRadius = Math::Sin(spotLight->innerConeAngle.toRadians());

    coneHeight *= spotLight->range;
    coneRadius *= spotLight->range;

    Transform innerCircleTransform = outerCircleTransform;
    innerCircleTransform.setScale(Vector3f(coneRadius, 1.0f, coneRadius));

    renderContext.transform = cameraWorldMatrix * innerCircleTransform.getMatrix();
    m_circle->draw(renderContext, m_graphicsManager.getDrawCommandQueue());

    // Range Line
    Transform rangeLineTransform;
    rangeLineTransform.setScale(Vector3f(1.0f, 1.0f, spotLight->range));

    renderContext.transform = cameraWorldMatrix * rangeLineTransform.getMatrix();
    m_line->draw(renderContext, m_graphicsManager.getDrawCommandQueue());
  }

  void SceneViewportLightGizmoRenderer::drawOmniLightGizmo(
    const GameObject& gameObject,
    const scene::lights::Light& light,
    const Camera& camera,
    bool isSelected
  )
  {
    if (!isSelected)
      return;

    const scene::lights::OmniLight* omniLight = light.getIfOmniLight();
    if (omniLight == nullptr)
    {
      LogService::Error(
        "SceneViewportLightGizmoRenderer: drawOmniLightGizmo called on non-omni light."
      );
      return;
    }

    RenderContext renderContext;
    renderContext.modelPosition = gameObject.getWorldPosition();
    renderContext.cameraWorldPosition = camera.getPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Ring 1
    Transform sphereTransform;
    sphereTransform.setScale(Vector3f(omniLight->range, omniLight->range, omniLight->range));

    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext, m_graphicsManager.getDrawCommandQueue());

    // Ring 2
    sphereTransform.setRotation(0.0f, 0.0f, Math::HalfPi);
    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext, m_graphicsManager.getDrawCommandQueue());

    // Ring 3
    sphereTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);
    renderContext.transform = cameraWorldMatrix * sphereTransform.getMatrix();
    m_circle->draw(renderContext, m_graphicsManager.getDrawCommandQueue());
  }

  void SceneViewportLightGizmoRenderer::drawDirectionalLightGizmo(
    const GameObject& gameObject,
    const scene::lights::Light& directionalLightComponent,
    const Camera& camera,
    bool isSelected
  )
  {
    if (!isSelected)
      return;

    const scene::lights::DirectionalLight* directionalLight = directionalLightComponent.getIfDirectionalLight();
    if (directionalLight == nullptr)
    {
      LogService::Error(
        "SceneViewportLightGizmoRenderer: drawDirectionalLightGizmo called on non-directional light."
      );
      return;
    }

    RenderContext renderContext;
    renderContext.modelPosition = gameObject.getWorldPosition();
    renderContext.cameraWorldPosition = camera.getPosition();

    Matrix4 cameraWorldTranslation = Matrix4::Translate(gameObject.getWorldPosition());
    Matrix4 cameraWorldRotation = gameObject.getWorldRotationMatrix();
    Matrix4 cameraWorldMatrix = cameraWorldTranslation * cameraWorldRotation;

    // Plane
    Transform planeTransform;
    planeTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);

    renderContext.transform = cameraWorldMatrix * planeTransform.getMatrix();
    m_square->draw(renderContext, m_graphicsManager.getDrawCommandQueue());

    // Range Line
    Transform rangeLineTransform;
    rangeLineTransform.setScale(Vector3f(1.0f, 1.0f, 10.0f));
    renderContext.transform = cameraWorldMatrix * rangeLineTransform.getMatrix();
    m_line->draw(renderContext, m_graphicsManager.getDrawCommandQueue());
  }

  void SceneViewportLightGizmoRenderer::drawLightIcon(
    const GameObject& gameObject,
    const scene::lights::Light& light,
    const Camera& camera,
    bool isSelected
  )
  {
    (void)gameObject;
    (void)light;
    (void)camera;
    (void)isSelected;

    // TODO:
    // - Draw billboard/icon mesh with constant-ish screen size.
  }
}
