#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 camera_position;
uniform sampler2D texture1;
uniform samplerCube skybox;

void main()
{
    if (gl_FrontFacing) {
        // 反射
        vec3 I = normalize(Position - camera_position);
        vec3 R = reflect(I, normalize(Normal));
        FragColor = vec4(texture(skybox, R).rgb, 1.0);

        // // 折射
        // float ratio = 1.00 / 1.52;
        // vec3 I = normalize(Position - camera_position);
        // vec3 R = refract(I, normalize(Normal), ratio);
        // FragColor = vec4(texture(skybox, R).rgb, 1.0);
    } else {
        FragColor = vec4(texture(texture1, TexCoords).rgb, 1.0);
    }
}
