#include "hc/graphics/lightShadowManager/hcALightShadowManager.h"

#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/light/hcDirectionalLight.h"

namespace hc
{
  ALightShadowManager::ALightShadowManager() :
    m_lightShadowFrameData(),
    m_countDirectionalLightShadows(0)
  {}

  ALightShadowManager::~ALightShadowManager()
  {}

  void ALightShadowManager::clear()
  {
    m_countDirectionalLightShadows = 0;
    onClear();
  }

  Int32 ALightShadowManager::generateDirectionalLightShadowData(
    const DirectionalLight& directionalLight,
    const SceneGraph& sceneGraph
  )
  {
    if (hasReachedMaxDirectionalLightShadows())
      return -1;

    DirectionalLightShadowFrameData& shadowData = m_lightShadowFrameData
      .directionalLightShadowData[m_countDirectionalLightShadows];

    float projectionSize = directionalLight.getShadowProjectionSize() * 0.5f;
    float nearPlane = directionalLight.getShadowProjectionNearPlane();
    float farPlane = directionalLight.getShadowProjectionFarPlane();

    Matrix4 projectionMatrix = Matrix4::Orthographic(
      -projectionSize, projectionSize,
      -projectionSize, projectionSize,
      nearPlane, farPlane
    );

    Vector3f lightDirection = directionalLight.getDirection();

    Vector3f up;
    if (Math::IsNearlyEqual(Math::Abs(lightDirection.y), 1.0f))
    {
      Vector3f right = Vector3f(1.0f, 0.0f, 0.0f);
      up = right.cross(lightDirection).normalized();
    }
    else
    {
      Vector3f worldUp = Vector3f(0.0f, 1.0f, 0.0f);
      Vector3f right = worldUp.cross(lightDirection).normalized();
      up = lightDirection.cross(right).normalized();
    }

    // TODO
    //
    // Shadow center and distance of a directional light should be calculated based on the scene's
    // bounding box or the area of interest (like the center's of the camera's frustum),
    // not just the origin.

    Vector3f shadowCenter = Vector3f(0.0f, 0.0f, 0.0f); // Currently, the center of the world
    float distance = 50.0f;
    Vector3f lightPosition = shadowCenter - lightDirection * distance;

    Matrix4 viewMatrix = Matrix4::LookAt(
      lightPosition,
      shadowCenter,
      up
    );

    shadowData.shadowBias = directionalLight.getShadowBias();
    shadowData.shadowStrength = directionalLight.getShadowStrength();
    shadowData.LightViewProjectionMatrix = projectionMatrix * viewMatrix;

    try
    {
      shadowData.shadowMapIndex = generateDirectionalLightShadowTexture(
        lightPosition,
        shadowData.LightViewProjectionMatrix,
        sceneGraph
      );
    }
    catch (const Exception& e)
    {
      destroy();
      throw;
    }

    if (shadowData.shadowMapIndex < 0)
      return -1;

    // TODO
    //
    // Transpose should be defined by the type of graphics API we are using, not here.

    shadowData.LightViewProjectionMatrix.transpose();

    Int32 newIndex = static_cast<Int32>(m_countDirectionalLightShadows);
    ++m_countDirectionalLightShadows;
    return newIndex;
  }
}
