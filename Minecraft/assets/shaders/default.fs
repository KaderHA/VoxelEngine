#version 330 core

in vec3 fNormal;
in vec2 fTexCoord;

struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
};
uniform Light uLight;

uniform sampler2D uTexture;

out vec4 Color;

void main() {
   // ambient
    vec3 ambient = uLight.ambient * texture(uTexture, fTexCoord).rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(-uLight.direction);  
    float diff = max(dot(fNormal, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * texture(uTexture, fTexCoord).rgb;  
        
    vec3 result = ambient + diffuse;
    Color = vec4(result, 1.0);
}