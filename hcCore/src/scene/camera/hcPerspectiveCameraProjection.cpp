#include "hc/scene/camera/hcPerspectiveCameraProjection.h"
#include "hc/scene/camera/hcProjectionType.h"

namespace hc
{
  PerspectiveCameraProjection::PerspectiveCameraProjection()
    : m_fovY(Angle::FromDegrees(60.0f)),
    m_aspectRatio(1.77778f),
    m_near(0.1f),
    m_far(1000.0f),
    m_isDirty(true),
    m_cachedProjectionMatrix(Matrix4::Identity())
  {
  }

  PerspectiveCameraProjection::~PerspectiveCameraProjection()
  {
  }

  projectionType::Type PerspectiveCameraProjection::getType() const
  {
    return projectionType::Perspective;
  }

  Matrix4 PerspectiveCameraProjection::getProjectionMatrix()
  {
    if (m_isDirty)
      update();
    return m_cachedProjectionMatrix;
  }

  void PerspectiveCameraProjection::update()
  {
    m_cachedProjectionMatrix = Matrix4::Perspective(
      m_fovY.toRadians(),
      m_aspectRatio,
      m_near, m_far
    );

    m_isDirty = false;
  }

  void PerspectiveCameraProjection::serialize(BinaryWriter& writer) const
  {
    writer.writeAngle(m_fovY);
    writer.writeFloat(m_aspectRatio);
    writer.writeFloat(m_near);
    writer.writeFloat(m_far);
  }

  void PerspectiveCameraProjection::deserialize(BinaryReader& reader)
  {
    m_fovY = reader.readAngle();
    m_aspectRatio = reader.readFloat();
    m_near = reader.readFloat();
    m_far = reader.readFloat();
    m_isDirty = true;
  }

  void PerspectiveCameraProjection::setFovY(Angle fovY)
  {
    if (m_fovY != fovY)
    {
      m_fovY = fovY;
      m_isDirty = true;
    }
  }

  const Angle& PerspectiveCameraProjection::getFovY() const
  {
    return m_fovY;
  }

  void PerspectiveCameraProjection::setAspectRatio(float aspectRatio)
  {
    if (m_aspectRatio != aspectRatio)
    {
      m_aspectRatio = aspectRatio;
      m_isDirty = true;
    }
  }

  float PerspectiveCameraProjection::getAspectRatio() const
  {
    return m_aspectRatio;
  }

  void PerspectiveCameraProjection::setNear(float near)
  {
    if (m_near != near)
    {
      m_near = near;
      m_isDirty = true;
    }
  }

  float PerspectiveCameraProjection::getNear() const
  {
    return m_near;
  }

  void PerspectiveCameraProjection::setFar(float far)
  {
    if (m_far != far)
    {
      m_far = far;
      m_isDirty = true;
    }
  }

  float PerspectiveCameraProjection::getFar() const
  {
    return m_far;
  }
}
