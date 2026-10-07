layout(std140, binding = 8) uniform SceneBlock
{
  vec4 uSceneAmbientColor;
  vec4 usInvertSkybox;               // Invert components of the skybox's cubemap sample direction (x, y, z, w) as needed (1.0 for normal, -1.0 for inverted)

  float uSceneAmbientIntensity;
  float uSPadding0;
  float uSPadding1;
  float uSPadding2;
};