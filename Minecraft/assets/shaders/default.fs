#version 330 core

flat in uint fFace;
in vec2 fTexCoord;

struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
};
uniform Light uLight;

uniform sampler2D uTexture;

out vec4 Color;

vec3 faceToNormal(uint face);

void main() {
   // ambient
    vec3 ambient = uLight.ambient * texture(uTexture, fTexCoord).rgb;
  	
    // diffuse 
    vec3 normal = faceToNormal(fFace);
    vec3 lightDir = normalize(-uLight.direction);  
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * texture(uTexture, fTexCoord).rgb;  
        
    vec3 result = ambient + diffuse;
    Color = vec4(result, 1.0);
}


vec3 faceToNormal(uint face) {
    vec3 normal;
    switch(face) {
    case uint(0):
        normal = vec3(0.0, 1.0, 0.0);
        break;
    case uint(1):
        normal = vec3(0.0, 0.0, -1.0);
        break;
    case uint(2):
        normal = vec3(-1.0, 0.0, 0.0);
        break;
    case uint(3):
        normal = vec3(1.0, 0.0, 0.0);
        break;
    case uint(4):
        normal = vec3(0.0, 0.0, 1.0);
        break;
    case uint(5):
        normal = vec3(0.0, -1.0, 0.0);
        break;
    }
    return normal;
}