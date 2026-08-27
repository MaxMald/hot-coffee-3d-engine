#version 420 core

#include "commons/materialUnlit.glsl"

in vec2 vTexCoord;
in vec4 vColor;

out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
  vec4 baseColor = uColor * vColor;
  vec4 textureColor = texture(uTexture, vTexCoord);

  if (uAlphaCutoff > 0.0 && textureColor.a < uAlphaCutoff)
    discard;

  FragColor = baseColor * textureColor;
}