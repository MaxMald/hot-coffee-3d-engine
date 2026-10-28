#define MAXIMUM_IOR 5
#define PI 3.14159265359
#define EPSILON 0.0001

const vec2 invAtan = vec2(0.1591, 0.3183);

/**
 * @brief Samples a spherical map using a 3D direction vector.
 *
 * @param v The 3D normalized direction vector.
 *
 * @return The corresponding 2D UV coordinates in the spherical map.
 */
vec2 sampleSphericalMap(vec3 v)
{
    vec3 direction = normalize(v);
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

/**
* @brief Clamps a value between 0.0 and 1.0.
*
* @param value The input value to be clamped.
*
* @return The clamped value, which will be in the range [0.0, 1.0].
*/
float saturate(float value)
{
  return clamp(value, 0.0, 1.0);
}

/**
 * @brief Performs alpha testing on a color against a specified alpha cutoff
 * value. If the alpha component of the color is less than this value, the
 * function returns true.
 *
 * If the alpha cutoff is less than or equal to zero, the function will always
 * return false.
 * 
 * @param color The color to test, which includes an alpha component.
 * @param alphaCutoff The alpha cutoff value to compare against.
 * 
 * @return true if the alpha component of the color is less than the alpha
 * cutoff value.
 */
bool isAlphaLessThanCutoff(vec4 color, float alphaCutoff)
{
  return alphaCutoff > 0.0 && color.a < alphaCutoff;
}

/**
 * @brief Computes the radical inverse of a given integer using the Van der Corput sequence.
 *
 * @param bits The input integer.
 *
 * @return The radical inverse of the input integer as a float in the range [0, 1).
 */
float radicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

/**
 * @brief Generates a 2D Hammersley point for a given index and total number of points.
 *
 * @param i The index of the point.
 * @param N The total number of points.
 *
 * @return A 2D point in the Hammersley sequence.
 */
vec2 hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), radicalInverse_VdC(i));
}  