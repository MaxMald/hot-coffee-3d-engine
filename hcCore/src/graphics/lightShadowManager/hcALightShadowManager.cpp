#include "hc/graphics/lightShadowManager/hcALightShadowManager.h"

#include "hc/scene/hcSceneGraph.h"
#include "hc/scene/light/hcDirectionalLight.h"
#include "hc/scene/light/hcSpotLight.h"

namespace hc
{
  ALightShadowManager::ALightShadowManager() :
    m_lightShadowFrameData(),
    m_countDirectionalLightShadows(0),
    m_countSpotLightShadows(0)
  {}

  ALightShadowManager::~ALightShadowManager()
  {}

  void ALightShadowManager::clear()
  {
    m_countDirectionalLightShadows = 0;
    m_countSpotLightShadows = 0;
    onClear();
  }

  Int32 ALightShadowManager::generateDirectionalLightShadowData(
    const DirectionalLight& directionalLight,
    const SceneGraph& sceneGraph
  )
  {
    if (m_countDirectionalLightShadows >= LightShadowFrameData::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA)
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
    Vector3f up = LinearAlgebra::CalculateUpFromDirection(lightDirection);

    // TODO
    //
    // Shadow target position and distance of a directional light should be calculated
    // based on the scene's bounding box or the area of interest (like the center's of the
    // camera's frustum), not just the origin.
    //
    // For now, we let the user specify the shadow target and distance in the
    // DirectionalLight class.

    Vector3f shadowTarget = directionalLight.getShadowViewTarget();
    float distance = directionalLight.getShadowViewDistance();
    Vector3f lightPosition = shadowTarget - lightDirection * distance;

    Matrix4 viewMatrix = Matrix4::LookAt(
      lightPosition,
      shadowTarget,
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

  Int32 ALightShadowManager::generateSpotLightShadowData(
    const SpotLight& spotLight,
    const SceneGraph& sceneGraph
  )
  {
    if (m_countSpotLightShadows >= LightShadowFrameData::MAX_SPOT_LIGHTS_SHADOW_DATA)
      return -1;

    SpotLightShadowFrameData& shadowData = m_lightShadowFrameData
      .spotLightShadowData[m_countSpotLightShadows];

    float nearPlane = spotLight.getShadowProjectionNearPlane();
    float farPlane = spotLight.getShadowProjectionFarPlane();
    float outerConeAngle = spotLight.getOuterConeAngle().toRadians();

    Matrix4 projectionMatrix = Matrix4::Perspective(
      outerConeAngle,
      1.0f,
      nearPlane,
      farPlane
    );

    Vector3f lightDirection = spotLight.getDirection();
    Vector3f up = LinearAlgebra::CalculateUpFromDirection(lightDirection);

    Vector3f lightPosition = spotLight.getPosition();
    Matrix4 viewMatrix = Matrix4::LookAt(
      lightPosition,
      lightPosition + lightDirection,
      up
    );

    shadowData.shadowBias = spotLight.getShadowBias();
    shadowData.shadowStrength = spotLight.getShadowStrength();
    shadowData.lightViewProjectionMatrix = projectionMatrix * viewMatrix;
    shadowData.projectionFarPlane = farPlane;
    shadowData.projectionNearPlane = nearPlane;

    try
    {
      shadowData.shadowMapIndex = generateSpotLightShadowTexture(
        lightPosition,
        shadowData.lightViewProjectionMatrix,
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

    shadowData.lightViewProjectionMatrix.transpose();

    Int32 newIndex = static_cast<Int32>(m_countSpotLightShadows);
    ++m_countSpotLightShadows;
    return newIndex;
  };
}
