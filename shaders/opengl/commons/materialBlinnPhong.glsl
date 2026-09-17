layout(std140, binding = 6) uniform MaterialBlinnPhongBlock
{
  vec4 uColor;
  float uAlphaCutoff;
  float uShininess;
  float mbpPadding0;
  float mbpPadding1;
};