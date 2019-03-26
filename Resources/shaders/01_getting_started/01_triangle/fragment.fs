#version 330 core

in vec3 tex_color;

out vec4 FragColor;

void main()
{
   FragColor = vec4(tex_color, 1.0f);
}
