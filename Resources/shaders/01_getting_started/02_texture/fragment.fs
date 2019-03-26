#version 330 core

in vec2 tex_coord;
out vec4 FragColor;

uniform sampler2D texture_id1;
uniform sampler2D texture_id2;

void main()
{
   FragColor = mix(texture(texture_id1, tex_coord), texture(texture_id2, tex_coord), 0.2);
}
