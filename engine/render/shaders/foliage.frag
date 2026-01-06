#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec3 uSunDirection;
uniform vec3 uSunColor;
uniform float uAmbientLight;
uniform vec3 uWindForce;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uSunDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uSunColor;
    
    vec3 ambient = uAmbientLight * vec3(0.2, 0.6, 0.2);
    
    vec4 texColor = texture(uTexture, vTexCoord);
    vec3 result = (ambient + diffuse) * texColor.rgb;
    
    FragColor = vec4(result, texColor.a);
}
