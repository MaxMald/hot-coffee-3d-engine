#pragma once

#include "hc/hcMath.h"

namespace hc
{
  class Angle
  {
  public:
    static Angle FromDegrees(float degrees);
    static Angle FromRadians(float radians);

    Angle();
    Angle(float radians);
    ~Angle();

    float toDegrees() const;
    float toRadians() const;

    Angle operator+(const Angle& rhs) const;
    Angle operator-(const Angle& rhs) const;
    Angle operator*(float scalar) const;
    Angle operator/(float scalar) const;

    Angle& operator+=(const Angle& rhs);
    Angle& operator-=(const Angle& rhs);
    Angle& operator*=(float scalar);
    Angle& operator/=(float scalar);

    bool operator==(const Angle& rhs) const;
    bool operator!=(const Angle& rhs) const;
    bool operator<(const Angle& rhs) const;
    bool operator<=(const Angle& rhs) const;
    bool operator>(const Angle& rhs) const;
    bool operator>=(const Angle& rhs) const;

  private:
    float m_radians;
  };

  inline Angle Angle::FromDegrees(float degrees)
  {
    return Angle(degrees * Math::DegToRad);
  }

  inline Angle Angle::FromRadians(float radians)
  {
    return Angle(radians);
  }

  inline Angle::Angle()
    : m_radians(0.0f)
  {
  }

  inline Angle::~Angle()
  {
  }

  inline Angle::Angle(float radians)
    : m_radians(radians)
  {
  }

  inline float Angle::toDegrees() const
  {
    return m_radians * Math::RadToDeg;
  }

  inline float Angle::toRadians() const
  {
    return m_radians;
  }

  inline Angle Angle::operator+(const Angle& rhs) const
  {
    return Angle(m_radians + rhs.m_radians);
  }

  inline Angle Angle::operator-(const Angle& rhs) const
  {
    return Angle(m_radians - rhs.m_radians);
  }

  inline Angle Angle::operator*(float scalar) const
  {
    return Angle(m_radians * scalar);
  }

  inline Angle Angle::operator/(float scalar) const
  {
    return Angle(m_radians / scalar);
  }

  inline Angle& Angle::operator+=(const Angle& rhs)
  {
    m_radians += rhs.m_radians;
    return *this;
  }

  inline Angle& Angle::operator-=(const Angle& rhs)
  {
    m_radians -= rhs.m_radians;
    return *this;
  }

  inline Angle& Angle::operator*=(float scalar)
  {
    m_radians *= scalar;
    return *this;
  }

  inline Angle& Angle::operator/=(float scalar)
  {
    m_radians /= scalar;
    return *this;
  }

  inline bool Angle::operator==(const Angle& rhs) const
  {
    return m_radians == rhs.m_radians;
  }

  inline bool Angle::operator!=(const Angle& rhs) const
  {
    return m_radians != rhs.m_radians;
  }

  inline bool Angle::operator<(const Angle& rhs) const
  {
    return m_radians < rhs.m_radians;
  }

  inline bool Angle::operator<=(const Angle& rhs) const
  {
    return m_radians <= rhs.m_radians;
  }

  inline bool Angle::operator>(const Angle& rhs) const
  {
    return m_radians > rhs.m_radians;
  }

  inline bool Angle::operator>=(const Angle& rhs) const
  {
    return m_radians >= rhs.m_radians;
  }
}
