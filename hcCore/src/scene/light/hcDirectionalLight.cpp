#include "hc/scene/light/hcDirectionalLight.h"

namespace hc
{
  DirectionalLight::DirectionalLight() :
    ALight(lightType::Type::Directional),
    m_direction(0.0f, -1.0f, 0.0f),
    m_shadowViewTarget(0.0f, 0.0f, 0.0f),
    m_shadowViewDistance(50.0f),
    m_shadowProjectionSize(10.0f),
    m_shadowProjectionNearPlane(0.1f),
    m_shadowProjectionFarPlane(100.0f)
  {}

  DirectionalLight::~DirectionalLight() = default;

  void DirectionalLight::serialize(io::BinaryWriter& writer) const
  {
    ALight::serialize(writer);
    writer.writeVector3f(m_direction);

    // TODO
    //
    // We should serialize the shadow projection parameters as well.
  }

  void DirectionalLight::deserialize(io::BinaryReader& reader)
  {
    ALight::deserialize(reader);
    m_direction = reader.readVector3f();

    // TODO
    //
    // We should deserialize the shadow projection parameters as well.
  }

  void DirectionalLight::setDirection(const Vector3f& direction)
  {
    m_direction = direction;
  }

  const Vector3f& DirectionalLight::getDirection() const
  {
    return m_direction;
  }

  void DirectionalLight::setShadowViewTarget(const Vector3f& target)
  {
    m_shadowViewTarget = target;
  }

  const Vector3f& DirectionalLight::getShadowViewTarget() const
  {
    return m_shadowViewTarget;
  }

  void DirectionalLight::setShadowViewDistance(float distance)
  {
    m_shadowViewDistance = Math::Max(distance, 0.1f);
  }

  float DirectionalLight::getShadowViewDistance() const
  {
      return m_shadowViewDistance;
  }

  void DirectionalLight::setShadowProjectionSize(float size)
  {
    m_shadowProjectionSize = Math::Max(size, 0.1f);
  }

  float DirectionalLight::getShadowProjectionSize() const
  {
    return m_shadowProjectionSize;
  }

  void DirectionalLight::setShadowProjectionNearPlane(float nearPlane)
  {
    m_shadowProjectionNearPlane = Math::Max(nearPlane, 0.1f);
  }

  float DirectionalLight::getShadowProjectionNearPlane() const
  {
    return m_shadowProjectionNearPlane;
  }

  void DirectionalLight::setShadowProjectionFarPlane(float farPlane)
  {
    m_shadowProjectionFarPlane = Math::Max(farPlane, 0.1f);
  }

  float DirectionalLight::getShadowProjectionFarPlane() const
  {
    return m_shadowProjectionFarPlane;
  }

  dataBlockStructure::DirectionalLight DirectionalLight::getDataBlockStructure() const
  {
    dataBlockStructure::DirectionalLight lightData;
    lightData.directionAndIntensity = Vector4f(m_direction.normalized(), m_intensity);
    lightData.color = m_color;
    return lightData;
  }

  dataBlockStructure::DirectionalLightShadow DirectionalLight::getShadowDataBlockStructure(
    bool transposeMatrices
  ) const
  {
    dataBlockStructure::DirectionalLightShadow shadowData;
    shadowData.shadowBias = m_shadowBias;
    shadowData.shadowStrength = m_shadowStrength;

    float projectionSize = m_shadowProjectionSize * 0.5f;
    Matrix4 projectionMatrix = Matrix4::Orthographic(
      -projectionSize, projectionSize,
      -projectionSize, projectionSize,
      m_shadowProjectionNearPlane, m_shadowProjectionFarPlane
    );

    // TODO
    //
    // Shadow target position and distance of a directional light should be calculated
    // based on the scene's bounding box or the area of interest (like the center's of the
    // camera's frustum), not just the origin.
    //
    // For now, we let the user specify the shadow target and distance in the
    // DirectionalLight class.

    Vector3f lightPosition = m_shadowViewTarget - m_direction * m_shadowViewDistance;
    Vector3f up = LinearAlgebra::CalculateUpFromDirection(m_direction);

    Matrix4 viewMatrix = Matrix4::LookAt(
      lightPosition,
      m_shadowViewTarget,
      up
    );

    shadowData.LightViewProjectionMatrix = projectionMatrix * viewMatrix;

    if (transposeMatrices)
    {
      shadowData.LightViewProjectionMatrix.transpose();
    }

    return shadowData;
  }
}
