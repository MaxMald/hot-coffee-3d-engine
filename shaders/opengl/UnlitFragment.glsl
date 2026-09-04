#version 420 core

#include "commons/materialUnlit.glsl"

layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec4 vColor;

layout(location = 0) out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
  vec4 baseColor = uColor * vColor;
  vec4 textureColor = texture(uTexture, vTexCoord);

  if (uAlphaCutoff > 0.0 && textureColor.a < uAlphaCutoff)
    discard;

  FragColor = baseColor * textureColor;
}