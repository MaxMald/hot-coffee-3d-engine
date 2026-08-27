#version 420 core

layout(location = 0) in vec3 aPosition;

layout (std140, binding = 0) uniform MatrixBlock
{
  mat4 uLightViewProjection;
  mat4 uModel;
};

void main()
{
  gl_Position = uLightViewProjection * uModel * vec4(aPosition, 1.0);
}