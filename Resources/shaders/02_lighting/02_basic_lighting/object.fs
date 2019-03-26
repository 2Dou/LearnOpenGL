#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    // 环境光
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(light_pos - FragPos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    // 镜面反射
    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_pos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    // 汇总
    vec3 result = (ambient + diffuse + specular) * object_color;

    FragColor = vec4(result, 1.0);
}
