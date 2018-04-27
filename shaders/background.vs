#version 330 core

out vec2 bg_vertex;

void main()
{
    uint idx = uint( gl_VertexID );
    gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;
    bg_vertex = vec2( gl_Position.xy * 0.5 + 0.5 );
}
