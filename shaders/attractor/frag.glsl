#version 330 core

uniform vec4 color;

in vec3 polygon_normal;
in vec4 frag_position;

out vec4 FragColor;

void main()
{
    vec3 light_position = vec3(-1.0f, 2.7f, 3.14f);
    vec4 light_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    float ambient_strength = 0.314;
    vec4 ambient = ambient_strength * light_color;

    vec3 normal = normalize(polygon_normal);
    vec3 light_dir = normalize(light_position.xyz - frag_position.xyz);

    float diff = max(dot(normal, light_dir), 0.0f);
    vec4 diffuse = diff * light_color;

    vec3 result = (ambient.rgb + diffuse.rgb) * color.rgb;
    FragColor = vec4(result, color.a);
}
