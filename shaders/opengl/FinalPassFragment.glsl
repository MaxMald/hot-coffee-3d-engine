#version 420 core

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uScene;

void main()
{
  vec4 color = texture(uScene, vTexCoord);
  color.rgb = pow(color.rgb, vec3(0.454545)); // Apply gamma correction (gamma = 2.2)
  FragColor = color;
}