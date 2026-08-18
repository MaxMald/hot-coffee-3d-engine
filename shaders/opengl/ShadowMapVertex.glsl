#version 420 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uLightViewProjection;
uniform mat4 uModel;

void main()
{
  gl_Position = uLightViewProjection * uModel * vec4(aPosition, 1.0);
}