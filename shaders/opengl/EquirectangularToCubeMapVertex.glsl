#version 420 core

#include "commons/camera.glsl"

layout(location = 0) in vec3 aPosition;

layout(location = 0) out vec3 vLocalPosition;

void main()
{
    vLocalPosition = aPosition;
    gl_Position = projection * view * vec4(aPosition, 1.0);
}