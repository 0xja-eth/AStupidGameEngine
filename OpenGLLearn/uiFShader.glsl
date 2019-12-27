#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D img;

void main() {
	//FragColor = vec4(1);
	
	vec4 texColor = texture(img, TexCoord);
	if(texColor.a < 0.1) discard;
	FragColor = texColor;
	
}