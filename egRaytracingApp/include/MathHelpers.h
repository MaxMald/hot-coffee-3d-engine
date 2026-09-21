#pragma once

#include <stdint.h>
#include <cmath>
#include <vector>
#include <hc/utilities/hcUtilities.h>

using hc::UInt32;
using hc::Int32;
using hc::UInt8;
using hc::Vector3f;
using hc::Vector4f;
using hc::Color;
using hc::Vector;

struct Material
{
  Color albedo;
  float metallic = 0.0f;
  float roughness = 0.0f; // 0.0 roughness does not exist, always minimum to 0.04;

  // Index of Refraction for most dielectrics Depending on the type of material, the IOR
  // can vary. For example, water has an IOR of 1.33, glass has an IOR of 1.5, and diamond
  // has an IOR of 2.42.
  float ior = 1.5f; // Index of Refraction
};

class AABB
{
public:
  static AABB Union(const AABB& a, const AABB& b)
  {
    Vector3f newMin(
      std::min(a.minimum.x, b.minimum.x),
      std::min(a.minimum.y, b.minimum.y),
      std::min(a.minimum.z, b.minimum.z)
    );
    Vector3f newMax(
      std::max(a.maximum.x, b.maximum.x),
      std::max(a.maximum.y, b.maximum.y),
      std::max(a.maximum.z, b.maximum.z)
    );
    return AABB(newMin, newMax);
  }

  AABB() {};
  AABB(
    const Vector3f& _minimum,
    const Vector3f& _maximum
  ) : minimum(_minimum), maximum(_maximum)
  {
  }

  bool intersects(const AABB& other)
  {
    return (minimum.x <= other.maximum.x && maximum.x >= other.minimum.x) &&
           (minimum.y <= other.maximum.y && maximum.y >= other.minimum.y) &&
           (minimum.z <= other.maximum.z && maximum.z >= other.minimum.z);
  }

  Vector3f getCenter() const
  {
    return (minimum + maximum) * 0.5f;
  }

public:
  Vector3f minimum;
  Vector3f maximum;
};

class Sphere
{
public:
  Sphere() {};
  Sphere(
    const Vector3f& center,
    float radius,
    const Material& material
  )
    : center(center),
    radius(radius),
    material(material)
  {
  }

public:
  Vector3f center;
  Material material;
  float radius;
};

class Plane
{
public:
  Plane() {};
  Plane(
    const Vector3f& point,
    const Vector3f& normal,
    const Material& material
  )
    : point(point),
    normal(normal.normalized()),
    material(material)
  {
  }

public:
  Vector3f point;
  Vector3f normal;
  Material material;
};

class Triangle
{
public:
  Triangle() {};
  Triangle(
    const Vector3f& v0,
    const Vector3f& v1,
    const Vector3f& v2,
    const Material& material,
    const AABB& aabb
  )
    : v0(v0),
    v1(v1),
    v2(v2),
    material(material),
    aabb(aabb)
  {
  }

public:
  Vector3f v0;
  Vector3f v1;
  Vector3f v2;
  Material material;
  AABB aabb;
};

class Ray
{
public:
  Ray() = default;
  Ray(
    const Vector3f& origin,
    const Vector3f& direction
  )
    : origin(origin), direction(direction.normalized())
  {
  }

  Vector3f where(float t) const
  {
    return origin + direction * t;
  }

  /**
   * @brief Checks if the ray intersects with an Axis-Aligned Bounding Box (AABB).
   *
   * source: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes//ray-box-intersection.html
   */
  bool intersects(const AABB& aabb) const
  {
    Vector3f min = aabb.minimum;
    Vector3f max = aabb.maximum;

    float tmin = (min.x - origin.x) / direction.x;
    float tmax = (max.x - origin.x) / direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - origin.y) / direction.y;
    float tymax = (max.y - origin.y) / direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (min.z - origin.z) / direction.z;
    float tzmax = (max.z - origin.z) / direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
      return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return true;
  }

public:
  Vector3f origin;
  Vector3f direction;
};

template<typename T>
class OctreeItem
{
public:
  OctreeItem() {};
  OctreeItem(T* object, const AABB& bounds)
    : object(object), bounds(bounds)
  {
  }

public:
  T* object;
  AABB bounds;
};

template<typename T>
class Octree
{
public:
  static constexpr UInt32 MAX_ITEMS = 4;

  Octree() {};
  Octree(const AABB& bounds)
    : bounds(bounds)
  {
  }

  void insert(T* object, const AABB& objectBounds)
  {
    OctreeItem<T> item = OctreeItem<T>(object, objectBounds);

    if (items.size() < MAX_ITEMS)
    {
      items.emplace_back(item);
    }
    else
    {
      if (children.empty())
        subdivide();

      for (auto& child : children)
      {
        if (child.bounds.intersects(objectBounds))
          child.insert(object, objectBounds);
      }
    }
  }

  void query(const Ray& ray, Vector<T*>& results)
  {
    if (!ray.intersects(bounds))
      return;

    for (const auto& item : items)
    {
      if (ray.intersects(item.bounds))
        results.push_back(item.object);
    }

    for (auto& child : children)
      child.query(ray, results);
  }

  const AABB& getBounds()
  {
    return bounds;
  }

public:
  AABB bounds;
  Vector<OctreeItem<T>> items;
  Vector<Octree<T>> children;

private:
  void subdivide()
  {
    Vector3f center = bounds.getCenter();
    for (int i = 0; i < 8; ++i)
    {
      Vector3f newMin = bounds.minimum;
      Vector3f newMax = center;
      if (i & 1) { newMin.x = center.x; newMax.x = bounds.maximum.x; }
      if (i & 2) { newMin.y = center.y; newMax.y = bounds.maximum.y; }
      if (i & 4) { newMin.z = center.z; newMax.z = bounds.maximum.z; }
      children.emplace_back(Octree<T>(AABB(newMin, newMax)));
    }
  }
};

#define REAL_TYPE float

struct HitInfo
{
  REAL_TYPE distance;
  Vector3f position;
  Vector3f normal;
  Sphere* pSphere = nullptr;
  Plane* pPlane = nullptr;
  Triangle* pTriangle = nullptr;
};
