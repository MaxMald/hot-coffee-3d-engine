#version 420 core

layout(std140, binding = 3) uniform ObjectBlock
{
  mat4 uModel;
};