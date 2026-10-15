#version 420 core

layout(location = 0) in vec3 vDirection;

layout(location = 0) out vec4 FragColor;

layout(binding = 8) uniform samplerCube uEnviromentCubeMap;

void main()
{
  FragColor = texture(uEnviromentCubeMap, vDirection);
}