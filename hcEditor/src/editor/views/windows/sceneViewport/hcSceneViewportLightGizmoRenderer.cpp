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
    if (!isSelected)
      return;

    const SpotLight& spotLight = spotLightComponent.getSpotLight();

    Matrix4 cameraWorldMatrix = gameObject.getWorldMatrix();
    RenderContext renderContext;
    renderContext.cameraMatrices = CameraMatrices::Create(camera);
    renderContext.cameraPosition = camera.getPosition();
    renderContext.modelPosition = Matrix4::ExtractTranslation(renderContext.transform);

    // Outer Circle
    float coneHeight = Math::Cos(spotLight.getOuterConeAngle() * Math::DegToRad);
    float coneRadius = Math::Sin(spotLight.getOuterConeAngle() * Math::DegToRad);

    coneHeight *= spotLight.getRange();
    coneRadius *= spotLight.getRange();

    Transform outerCircleTransform;
    outerCircleTransform.setRotation(Math::HalfPi, 0.0f, 0.0f);
    outerCircleTransform.setPosition(0.0f, 0.0f, -coneHeight);
    outerCircleTransform.setScale(Vector3f(coneRadius, 1.0f, coneRadius));

    renderContext.transform = cameraWorldMatrix * outerCircleTransform.getMatrix();
    m_circle->draw(renderContext);

    // Inner Circle
    coneHeight = Math::Cos(spotLight.getInnerConeAngle() * Math::DegToRad);
    coneRadius = Math::Sin(spotLight.getInnerConeAngle() * Math::DegToRad);

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

    float coneHeight = Math::Cos(spotLight.getOuterConeAngle() * Math::DegToRad);
    float coneRadius = Math::Sin(spotLight.getOuterConeAngle() * Math::DegToRad);

    coneTransform.setPosition(Vector3f(0.0f, 0.0f, -coneHeight * spotLight.getRange() * 0.5f));
    coneTransform.setRotation(Vector3f(Math::HalfPi, 0.0f, 0.0f));
    coneTransform.setScale(Vector3f(coneRadius, coneHeight * spotLight.getRange(), coneRadius));

    return coneTransform;
  }
  
}
