#version 420 core

in vec2 vTexCoord;
in vec3 vWorldPos;
in vec3 vNormal;
in vec3 vTangent;
in vec4 vColor;

layout(location = 0) out vec4 OutPositionAndDepth;
layout(location = 1) out vec4 OutNormalRoughness;
layout(location = 2) out vec4 OutAlbedoAlpha;
layout(location = 3) out vec4 OutMaterialParameters;

uniform vec4 uColor;
uniform sampler2D uAlbedo;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;
uniform float uAlphaCutoff;
uniform float uShininess;

void main()
{
  vec4 albedoTex = texture(uAlbedo, vTexCoord);
  if (uAlphaCutoff > 0.0 && albedoTex.a < uAlphaCutoff)
    discard;

  vec4 baseColor = uColor * vColor * albedoTex;

  vec3 N = normalize(vNormal);
  vec3 T = normalize(vTangent - dot(vTangent, N) * N);
  vec3 B = normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);
  vec3 normalTS = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
  vec3 normalWS = normalize(TBN * normalTS);

  OutPositionAndDepth = vec4(vWorldPos, gl_FragCoord.z);
  OutNormalRoughness = vec4(normalWS, 1.0 - (uShininess / 256.0));
  OutAlbedoAlpha = baseColor;
  OutMaterialParameters = vec4(texture(uSpecularMap, vTexCoord).r, 0.0, 0.0, 1.0);
}