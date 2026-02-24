#include "hc/scene/camera/hcOrthographicCameraProjection.h"

namespace hc
{
  OrthographicCameraProjection::OrthographicCameraProjection() :
    m_left(-1.0f),
    m_right(1.0f),
    m_top(1.0f),
    m_bottom(-1.0f),
    m_near(0.1f),
    m_far(1000.0f),
    m_isDirty(true)
  {
  }

  OrthographicCameraProjection::~OrthographicCameraProjection()
  {
  }

  projectionType::Type OrthographicCameraProjection::getType() const
  {
    return projectionType::Orthographic;
  }

  float OrthographicCameraProjection::getLeft() const
  {
    return m_left;
  }

  void OrthographicCameraProjection::setLeft(float left)
  {
    m_left = left;
    m_isDirty = true;
  }

  float OrthographicCameraProjection::getRight() const
  {
    return m_right;
  }

  void OrthographicCameraProjection::setRight(float right)
  {
    m_right = right;
    m_isDirty = true;
  }

  float OrthographicCameraProjection::getTop() const
  {
    return m_top;
  }

  void OrthographicCameraProjection::setTop(float top)
  {
    m_top = top;
    m_isDirty = true;
  }

  float OrthographicCameraProjection::getBottom() const
  {
    return m_bottom;
  }

  void OrthographicCameraProjection::setBottom(float bottom)
  {
    m_bottom = bottom;
    m_isDirty = true;
  }

  void OrthographicCameraProjection::setNear(float near)
  {
    m_near = near;
    m_isDirty = true;
  }

  float OrthographicCameraProjection::getNear() const
  {
    return m_near;
  }

  void OrthographicCameraProjection::setFar(float far)
  {
    m_far = far;
    m_isDirty = true;
  }

  float OrthographicCameraProjection::getFar() const
  {
    return m_far;
  }

  Matrix4 OrthographicCameraProjection::getProjectionMatrix()
  {
    if (m_isDirty)
      update();

    return m_cachedProjectionMatrix;
  }
  void OrthographicCameraProjection::update()
  {
    m_cachedProjectionMatrix = Matrix4::Orthographic(
      m_left, m_right,
      m_bottom, m_top,
      m_near, m_far
    );

    m_isDirty = false;
  }
}
