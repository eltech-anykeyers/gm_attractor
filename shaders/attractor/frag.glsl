#version 330 core

uniform vec4 color;
uniform vec4 light_color;

out vec4 FragColor;

void main()
{
    float ambient_strength = 0.1;
    vec4 ambient = ambient_strength * light_color;

    FragColor = vec4(ambient.rgb * color.rgb, color.a);
}
