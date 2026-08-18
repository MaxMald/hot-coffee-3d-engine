#version 420 core

layout(std140, binding = 1) uniform CameraFrameBlock
{
  mat4 projection;
  mat4 view;
  vec3 cameraPosition;
  float cPadding0;
};

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in vec4 aColor;

uniform mat4 uModel;

out vec2 vTexCoord;
out vec3 vWorldPos;
out vec3 vNormal;
out vec3 vTangent;
out vec4 vColor;

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