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
layout(location = 4) out vec4 OutSpecularColorAndShininess;

uniform vec4 uColor;
uniform float uAlphaCutoff;
uniform float uShininess;

uniform sampler2D uAlbedo;
uniform sampler2D uNormalMap;

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
  OutMaterialParameters = vec4(0.0, 0.0, 0.0, 1.0);
  
  // Specularity is not used for hair during deferred geometry pass, so we set
  // it to 0.0.
  OutSpecularColorAndShininess = vec4(0.0, 0.0, 0.0, 0.0);
}