#version 420 core
in vec2 vTexCoord;
in vec4 vColor;
out vec4 FragColor;

uniform vec4 uColor;
uniform sampler2D uTexture;
uniform float uAlphaCutoff;

void main()
{
  vec4 baseColor = uColor * vColor;
  vec4 textureColor = texture(uTexture, vTexCoord);

  if (uAlphaCutoff > 0.0 && textureColor.a < uAlphaCutoff)
    discard;

  FragColor = baseColor * textureColor;
}