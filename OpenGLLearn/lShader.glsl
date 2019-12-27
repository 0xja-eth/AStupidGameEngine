#version 330 core
out vec4 FragColor;

struct Light {
    vec3 ambient;
};

#define NR_POINT_LIGHTS 1
uniform Light lights[NR_POINT_LIGHTS];

void main()
{
    FragColor = vec4(lights[0].ambient*50, 1.0);
}