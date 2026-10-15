#version 420 core

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uScene;

void main()
{
  vec4 color = texture(uScene, vTexCoord);

  // Apply tone mapping (Reinhard)
  //vec3 toneMappedColor = color.rgb / (color.rgb + vec3(1.0));
  // Apply gamma correction (gamma = 2.2)
  vec3 gammaCorrectedColor = pow(color.rgb, vec3(0.454545)); 

  FragColor = vec4(gammaCorrectedColor, color.a);
}