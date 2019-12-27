
#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Camera {
    mat4 camProj;
    mat4 camRot;
    mat4 camView;
	vec3 camPos;
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main() {
	gl_Position = camProj * camView * model * vec4(aPos, 1.0);
}