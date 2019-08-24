#version 330 core

in Interpolators {
    vec2 uv;
    vec3 view;
    vec3 normal;
    vec3 world;
} fs_in;

struct Material {
    sampler2D ambient_occlusion;
	  sampler2D albedo;
  	sampler2D specular;
  	vec3 specular_tint;
  	vec3 albedo_tint;
  	float shininess;
};

uniform Material material;

struct PointLight {
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float attenuation_quadratic;
    float attenuation_linear;
    float attenuation_constant;
};

uniform PointLight light;

uniform vec3 ambient;

out vec4 color;

float diffuse(vec3 n, vec3 l){
    return max(0, dot(n,l));
}

float specular(vec3 n, vec3 l, vec3 v, float shininess){
    vec3 h = normalize(l + v);
    return pow(max(0, dot (n, h)), shininess);
}

void main()
{
  float ao = texture(material.ambient_occlusion, fs_in.uv).r;
  vec3 albedo = material.albedo_tint * texture(material.albedo, fs_in.uv).rgb;
  vec3 specular = material.specular_tint * texture(material.specular, fs_in.uv).rgb;
  vec3 n = normalize(fs_in.normal);
  vec3 v = normalize(fs_in.view);
  vec3 l = light.position - fs_in.world;
  float d = length(l);
  l /= d;
  float attenuation = light.attenuation_constant +
                      light.attenuation_linear * d +
                      light.attenuation_quadratic * d * d;
  color = vec4(
    albedo*ao*ambient +
    (
        albedo*light.diffuse*diffuse(n, l) +
        specular*light.specular*specular(n, l, v, material.shininess)
    )/attenuation,
    1.0f
    );
}
