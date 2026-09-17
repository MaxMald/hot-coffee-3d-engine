#version 420 core

#include "commons/camera.glsl"
#include "commons/object.glsl"

layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec4 vColor;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec4 aColor;

void main()
{
  vTexCoord = aTexCoord;
  vColor = aColor;

  gl_Position = projection * view * uModel * vec4(aPosition, 1.0);
}