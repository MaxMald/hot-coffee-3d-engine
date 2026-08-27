#version 420 core

layout(std140, binding = 7) uniform MaterialHairBlock
{
  vec4 uColor;
  vec4 uPrimaryColor;
  vec4 uSecondaryColor;
  
  float uAlphaCutoff;
  float uShininess;  
  float uPrimaryShift;
  float uSecondaryShift;

  float uSpecularWidth;
  float uSpecularStrength;
  float mhPadding0;
  float mhPadding1;
};