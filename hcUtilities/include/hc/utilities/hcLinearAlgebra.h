#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcMath.h"
#include "hc/utilities/hcVector3.h"

namespace hc
{
  struct LinearAlgebra
  {
    /**
     * @brief Calculates an up vector that is perpendicular to the given direction vector.
     *
     * @param direction The input direction vector for which to calculate an up vector.
     *
     * @return A normalized up vector that is perpendicular to the input direction.
     */
    static Vector3f CalculateUpFromDirection(const Vector3f& direction)
    {
      if (Math::IsNearlyEqual(Math::Abs(direction.y), 1.0f))
      {
        Vector3f right = Vector3f(1.0f, 0.0f, 0.0f);
        return right.cross(direction).normalized();
      }
      else
      {
        Vector3f worldUp = Vector3f(0.0f, 1.0f, 0.0f);
        Vector3f right = worldUp.cross(direction).normalized();
        return direction.cross(right).normalized();
      }
    }
  };
}
