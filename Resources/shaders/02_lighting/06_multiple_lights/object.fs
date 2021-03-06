#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;
uniform vec3 view_pos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dir_light;

struct PointLight {
    vec3 direction;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cut_off;   // 用大于0的数表示聚光，其他情况表示点光
    float outer_cut_off;
};
#define NR_POINT_LIGHTS 4
uniform PointLight point_lights[NR_POINT_LIGHTS];

uniform PointLight spot_light;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_direction) {
    vec3 light_direction = normalize(-light.direction);

    // 漫反射着色
    float diff = max(dot(normal, light_direction), 0.0);

    // 镜面光着色
    vec3 reflect_direction = reflect(-light_direction, normal);

    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction) {
    vec3 light_direction = normalize(light.position - frag_position);

    vec3 r = CalcDirLight(DirLight(-light_direction, light.ambient, light.diffuse, light.specular), normal, view_direction);

    // 衰减
    float distance    = length(light.position - frag_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));

    return (r * attenuation);
}

vec3 CalcSpotLight(PointLight light, vec3 normal, vec3 frag_position, vec3 vew_direction) {
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 light_direction = normalize(light.position - FragPos);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // 镜面反射
    vec3 view_direction = normalize(view_pos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta     = dot(light_direction, normalize(-light.direction));
    float epsilon   = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    ambient  *= attenuation;
    diffuse  *= (attenuation * intensity);
    specular *= (attenuation * intensity);

    // 汇总
    vec3 result = ambient + diffuse + specular;

    return result;
}

void main()
{
     // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(view_pos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dir_light, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(point_lights[i], norm, FragPos, viewDir);
    }
    // 第三阶段：聚光
    result += CalcSpotLight(spot_light, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}
