
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 imodel;

layout (std140) uniform Camera {
    mat4 camProj;
    mat4 camRot;
    mat4 camView;
	vec3 camPos;
};

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

out vec3 CamPos;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main() {
	FragPos = vec3(imodel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(imodel))) * aNormal;

	gl_Position = camProj * camView * vec4(FragPos, 1.0);
    
	TexCoord = aTexCoord;
	CamPos = camPos;
}