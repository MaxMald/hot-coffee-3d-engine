#version 420 core

#include "commons/camera.glsl"
#include "commons/object.glsl"

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec4 aColor;

layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec3 vWorldPos;
layout(location = 2) out vec3 vNormal;
layout(location = 3) out vec3 vTangent;
layout(location = 4) out vec4 vColor;

void main()
{
  vec4 worldPos = uModel * vec4(aPosition, 1.0);
  vWorldPos = worldPos.xyz;

  vTexCoord = aTexCoord;
  vNormal = normalize(mat3(uModel) * aNormal);
  vTangent = normalize(mat3(uModel) * aTangent);
  vColor = aColor;

  gl_Position = projection * view * worldPos;
}