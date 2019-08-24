#version 330 core
// This shader is based on the tutorial in this link https://gamedevelopment.tutsplus.com/tutorials/how-to-write-a-smoke-shader--cms-25587
// changed some stuff because they are either deprecated or to have more control or clarification

uniform vec2 screenResolution;
uniform sampler2D inputTexture; 

uniform vec2 smokeSource; 
uniform float smokeDensity;
uniform float smokeRadius;

uniform float time;

const float timeStep = 1 / 60.0; // 60 Frames per second

out vec4 fragColor;

void main() 
{
    vec2 pixel = gl_FragCoord.xy / screenResolution.xy;
    
    float dist = distance(smokeSource, gl_FragCoord.xy);
    fragColor = texture(inputTexture, pixel);

    // Generate smoke when mouse is pressed
    fragColor.rgb += smokeDensity * max(smokeRadius - dist, 0.0);

    vec2 smokePoint = vec2(screenResolution.x / 2.0 + 100.0 * sin(time * 1.5 ), screenResolution.y / 2.0 + cos(time*3.5) * 20.0);
    dist = distance(smokePoint, gl_FragCoord.xy);
    fragColor.rgb += smokeDensity * max(smokeRadius - dist, 0.0);

    // A very Lame Diffusion Model for smoke but sufficient for our purposes
    float xOffset = 1.0 / screenResolution.x; // The size of a single pixel
    float yOffset = 1.0 / screenResolution.y;

    vec4 rightColor = texture(inputTexture,vec2(pixel.x + xOffset, pixel.y));
    vec4 leftColor = texture(inputTexture,vec2(pixel.x - xOffset, pixel.y));
    vec4 upColor = texture(inputTexture,vec2(pixel.x, pixel.y + yOffset));
    vec4 downColor = texture(inputTexture,vec2(pixel.x, pixel.y - yOffset));

    // Handle the bottom boundary problem
    if(pixel.y <= yOffset){
        downColor.rgb = vec3(0.0);
    }

    // Diffuse equation
    float diffuseFactor = 9.8 * timeStep * (leftColor.r + rightColor.r + downColor.r * 3.0 + upColor.r - 6 * fragColor.r); // 9.8 is an empirical value

    fragColor.rgb += diffuseFactor;
}