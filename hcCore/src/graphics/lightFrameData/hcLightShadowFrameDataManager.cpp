#include "hc/graphics/lightShadowFrameData/hcLightShadowFrameDataManager.h"
#include "hc/scene/light/hcDirectionalLight.h"

namespace hc
{
  LightShadowFrameDataManager::LightShadowFrameDataManager() :
    m_lightShadowFrameData(),
    m_countDirectionalLightShadows(0)
  {}

  LightShadowFrameDataManager::~LightShadowFrameDataManager()
  {}

  void LightShadowFrameDataManager::initialize()
  {}

  void LightShadowFrameDataManager::clearLightShadowFrameData()
  {
    m_countDirectionalLightShadows = 0;
  }

  Int32 LightShadowFrameDataManager::generateDirectionalLightShadowData(
    const DirectionalLight& directionalLight
  )
  {
    if (hasReachedMaxDirectionalLightShadows())
      return -1;

    DirectionalLightShadowFrameData& shadowData = m_lightShadowFrameData
      .directionalLightShadowData[m_countDirectionalLightShadows];

    // TODO We should have a better way to calculate the size of the orthographic
    // projection for directional lights, instead of hardcoding the values.
    //
    // For now, we will use a fixed size of 20x20 units for the orthographic projection.

    Matrix4 projectionMatrix = Matrix4::Orthographic(
      -10.0f, 10.0f,
      -10.0f, 10.0f,
      0.1f, 100.0f
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

    Vector3f lightPosition = directionalLight.getPosition();
    Matrix4 viewMatrix = Matrix4::LookAt(
      lightPosition,
      lightPosition + lightDirection,
      up
    );

    shadowData.shadowBias = directionalLight.getShadowBias();
    shadowData.shadowStrength = directionalLight.getShadowStrength();
    shadowData.LightViewProjectionMatrix = projectionMatrix * viewMatrix;
    // TODO: Assign the appropriate shadow map index here.
    //shadowData.shadowMapIndex = 0;

    Int32 newIndex = static_cast<Int32>(m_countDirectionalLightShadows);
    ++m_countDirectionalLightShadows;
    return newIndex;
  }

  void LightShadowFrameDataManager::destroy()
  {}
}
