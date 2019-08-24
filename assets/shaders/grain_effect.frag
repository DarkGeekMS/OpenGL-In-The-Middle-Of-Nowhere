#version 330 core

in vec2 vUV;

uniform sampler2D tex;
uniform float time;
uniform float grainIntensity;

out vec4 color;

void main()
{
    vec4 frag_color = texture(tex, vUV);
    
    float x = (gl_FragCoord.x / 3.14159 + 4) * (gl_FragCoord.y / 5.49382 + 4) * ((fract(time) + 1));
    vec4 grain = grainIntensity * vec4(mod((mod(x, 13) + 1) * (mod(x, 123) + 1), 0.01) - 0.005);
    color = frag_color + grain;
}