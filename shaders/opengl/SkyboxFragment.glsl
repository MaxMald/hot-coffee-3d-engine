#version 420 core

in vec3 vDirection;

out vec4 FragColor;

layout(binding = 0) uniform samplerCube uSkybox;

void main()
{
  FragColor = texture(uSkybox, vDirection);
}