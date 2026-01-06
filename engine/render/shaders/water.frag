#version 330 core

uniform vec3 uWaterColor;
uniform vec3 uSunColor;

out vec4 FragColor;

void main()
{
    vec3 waterColor = mix(uWaterColor, uSunColor, 0.3);
    FragColor = vec4(waterColor, 0.6);
}
