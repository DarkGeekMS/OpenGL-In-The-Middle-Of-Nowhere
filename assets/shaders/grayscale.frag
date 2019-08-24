#version 330 core

in vec2 vUV;

uniform sampler2D tex;

out vec4 color;

void main()
{
    vec4 frag_color = texture(tex, vUV);
    float gray_color = (frag_color.r + frag_color.g + frag_color.b) / 3.0; // Take the average of the colors, there are better techniques but this is sufficient
    color = vec4(gray_color, gray_color, gray_color, 1.0);
}