#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 camera_position;
uniform samplerCube skybox;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

// 反射贴图实际保存在环境光贴图中
uniform sampler2D texture_ambient1;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 view_dir = normalize(camera_position-FragPos);

    float diff = max(normalize(dot(norm, view_dir)), 0.0f);
    vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 I = normalize(FragPos - camera_position);
    vec3 R = reflect(I, norm);

    vec3 reflection = vec3(texture(texture_ambient1, TexCoords));
    reflection *= vec3(texture(skybox, R).rgb);

    FragColor = vec4(diffuse + reflection, 1.0);
}
