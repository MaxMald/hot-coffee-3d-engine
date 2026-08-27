#version 420 core

layout(std140, binding = 4) uniform LightViewProjectionBlock
{
  mat4 uLightViewProjection;
};