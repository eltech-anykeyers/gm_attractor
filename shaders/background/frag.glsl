#version 330 core

in vec2 bg_vertex;

out vec4 frag_color;

uniform vec3 top_color;
uniform vec3 bot_color;

void main()
{
    vec3 grad_rgb = bot_color * (1.0f - bg_vertex.y) + top_color * bg_vertex.y;
    frag_color = vec4( grad_rgb, 1.0f );
}
