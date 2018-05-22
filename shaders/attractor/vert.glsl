#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 _polygon_normal;

uniform vec4 model_0;
uniform vec4 model_1;
uniform vec4 model_2;
uniform vec4 model_3;

uniform vec4 trans_0;
uniform vec4 trans_1;
uniform vec4 trans_2;
uniform vec4 trans_3;

out vec4 frag_position;
out vec3 polygon_normal;

void main()
{
    polygon_normal = _polygon_normal;

    mat4 model = mat4(model_0, model_1, model_2, model_3);
    frag_position = vec4(model * vec4(pos, 1.0));

    mat4 transform = mat4(trans_0, trans_1, trans_2, trans_3);
    gl_Position = transform * vec4(pos, 1.0f);
}
