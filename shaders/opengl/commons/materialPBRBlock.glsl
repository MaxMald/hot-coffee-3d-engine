layout(std140, binding = 7) uniform MaterialPBRBlock
{
  vec4 uBaseColor;
  
  float uAlphaCutoff;
  float uMetallic;  
  float uRoughness;
  float uIOR;
};