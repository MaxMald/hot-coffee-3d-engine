#version 420 core

layout(std140, binding = 5) uniform MaterialUnlitBLock
{
  vec4 uColor;
  float uAlphaCutoff;
  float muPadding0;
  float muPadding1;
  float muPadding2;
};