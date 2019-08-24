#version 330 core
out vec4 Fragcolor;

in Interpolators {
    vec2 uv;      //TexCoords
    vec3 view;
    vec3 normal;
    vec3 world;   //FragPos
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

uniform samplerCube shadowMap;

uniform float far_plane;
uniform bool shadows;

uniform vec3 ambient;

float diffuse(vec3 n, vec3 l)
{
    return max(0, dot(n,l));
}

float phong(vec3 n, vec3 l, vec3 v, float shininess)
{
    vec3 h = normalize(l + v);
    return pow(max(0, dot (n, h)), shininess);
}


float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.position;
    // use the light to fragment vector to sample from the depth map
    float closestDepth = texture(shadowMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
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
  vec3 color = albedo*ao*ambient +
                (
                    albedo*light.diffuse*diffuse(n, l) +
                    specular*light.specular*phong(n, l, v, material.shininess)
                )/attenuation;

  float shadow = shadows ? ShadowCalculation(fs_in.world) : 0.0;
  Fragcolor = vec4(color*(1-0.5*shadow), 1.0); //if it's in shadow it will be black, this will be changed when we have lighting
}
