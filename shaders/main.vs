#version 330 core

layout (location = 0) in vec3 pos;

uniform vec4 trans_0;
uniform vec4 trans_1;
uniform vec4 trans_2;
uniform vec4 trans_3;

void main()
{
    mat4 transform = mat4(trans_0, trans_1, trans_2, trans_3);
    gl_Position = transform * vec4(pos, 1.0f);
}
