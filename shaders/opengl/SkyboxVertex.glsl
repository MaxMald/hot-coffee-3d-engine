#version 420 core

layout(std140, binding = 1) uniform CameraFrameBlock
{
  mat4 projection;
  mat4 view;
  vec3 cameraPosition;
  float cPadding0;
};

layout(location = 0) in vec3 aPosition;

out vec3 vDirection;

void main()
{
  vDirection = vec3(aPosition.xy, -aPosition.z); // Flip Z to match OpenGL's coordinate system
  gl_Position = projection * mat4(mat3(view)) * vec4(aPosition, 1.0);
  gl_Position = gl_Position.xyww; // Force depth to 1.0 to ensure skybox is rendered behind all geometry
}