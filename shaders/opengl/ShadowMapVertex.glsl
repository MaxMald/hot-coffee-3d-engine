#version 420 core

#include "commons/object.glsl"
#include "commons/lightViewProjection.glsl"

layout(location = 0) in vec3 aPosition;

void main()
{
  gl_Position = uLightViewProjection * uModel * vec4(aPosition, 1.0);
}