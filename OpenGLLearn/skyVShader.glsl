#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Camera {
    mat4 camProj;
    mat4 camRot;
    mat4 camView;
	vec3 camPos;
};

out vec3 TexCoord;

void main() {
    TexCoord = aPos;
    vec4 pos = camProj * camRot * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
