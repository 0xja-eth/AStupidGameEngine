#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 imodel;

out vec2 TexCoord;
out vec3 FragPos;

void main() {
	FragPos = vec3(imodel * vec4(aPos, 1.0));
    gl_Position = vec4(FragPos, 1.0);
    TexCoord = aTexCoord;
}
