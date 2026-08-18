#version 420 core

layout(binding = 0) uniform sampler2D uScene;

in vec2 vTexCoord;
out vec4 FragColor;

void main()
{
  vec4 color = texture(uScene, vTexCoord);
  color.rgb = pow(color.rgb, vec3(0.454545)); // Apply gamma correction (gamma = 2.2)
  FragColor = color;
}