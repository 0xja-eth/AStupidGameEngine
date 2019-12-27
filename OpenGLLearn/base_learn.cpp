//#define CUBE_STYLE
#ifdef CUBE_STYLE

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void updateInput(GLFWwindow *window);

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define WIN_TITLE "LearnOpenGL"

#define BORDER_COLOR { 1.0f, 1.0f, 0.0f, 1.0f }

#define CAMER_POS glm::vec3(0.0f, 0.0f, 3.0f)

GLFWwindow* window;
Shader* shader, *lightShader;
Camera* camera;

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
float lastX = SCR_WIDTH / 2; // ��һ֡���Xλ��
float lastY = SCR_HEIGHT / 2; //  ��һ֡���Yλ��

#pragma region �������ݶ���

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
/*
float vertices[] = {
	// λ��              // ��ɫ
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};
*/

float vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // ����
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // ����
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // ����
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // ����
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f,  0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // ����
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // ����
		-0.5f, -0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // ����
		-0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // ����
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // ����
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // ����
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // ǰ��
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // ǰ��
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // ����
		0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // ����
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // ǰ��
		0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // ǰ��
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // �Һ�
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // ��ǰ
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // ��ǰ
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // ���
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -    --- ������ ---
		 0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // �Һ�
		 0.5f, 0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // ��ǰ
		-0.5f, 0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // ��ǰ
		-0.5f, 0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // ���
};

/*
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 1.5f,  // ���½�
	-0.5f, -0.5f, 1.0f, // ���½�
	-0.5f, 0.5f, -1.0f,  // ���Ͻ�
	-0.5f, 0.0f, -0.0f,  // ���н�
	0.0f, 0.0f, 3.0f,   // ���н�
};
*/

// ��������
float texCoords[] = {
	0.0f, 0.0f, // ���½�
	1.0f, 0.0f, // ���½�
	0.5f, 1.0f // ����
};

unsigned int indices[] = { // ע��������0��ʼ! 
	0, 1, 3,  // ��1��������
	1, 2, 3,  // ��2��������

	4, 6, 7,  // ��3��������
	4, 6, 5,  // ��4��������

	8, 10,11, // ��5��������
	8, 10,9, // ��6��������

	12,13,15, // ��7��������
	13,14,15, // ��8��������

	16,18,19, // ��9��������
	16,18,17, // ��10��������

	20,21,23, // ��11��������
	21,22,23, // ��12��������
};

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

#pragma endregion

// ��ʼ�� GLFW
void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

// ����GLAD
bool setupGLAD() {
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

// �ص���ʼ��
void setupCallback() {
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // �������
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

// ����GL����
bool createGLWindow() {
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); return false;
	}
	setupCallback();
	return true;
}

// GL ����
unsigned int VAO, VBO, EBO;
unsigned int lightVAO;
unsigned int texture;

// ���ö�������
void setupVertexAttrs() {
	// �趨����λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ���ö�����ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ���ö���������������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ���÷�������������
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
}

// ���õƶ������ԣ�ֻ��λ�����ݣ�
void setupLightAttrs() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11  * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

// ��ʼ������ͻ������
void initVertexBuffers() {

	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// �󶨶����������
	glBindVertexArray(VAO);

	// �󶨶�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ����������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	setupVertexAttrs();

	// �󶨵ƶ����������
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	setupLightAttrs();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// ��������
void setupTexture() {
	// ���ز���������
	int width, height, nrChannels;
	unsigned char *data = stbi_load("box.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
}

// ��ʼ������
void initTexture() {
	// ��������
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// ��Ե�ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// ����ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ���ñ߿���ɫ
	float borderColor[] = BORDER_COLOR;
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	setupTexture();
}

// ��ʼ��GL����
void initGLObjects() {
	// ��Ȳ���
	glEnable(GL_DEPTH_TEST);
	initVertexBuffers();
	initTexture();
}

// ��ʼ��
bool initialize() {
	initGLFW();
	if (!createGLWindow()) return false;
	if (!setupGLAD()) return false;
	initGLObjects();
	return true;
}

// ���»�ȡDeltaʱ��
void updateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

// ��������ʱ��
void updateInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->updateCameraMove(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->updateCameraMove(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->updateCameraMove(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->updateCameraMove(RIGHT, deltaTime);
}

// �������
void clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ���ù۲�/ͶӰ����
void setupMatrices(glm::mat4& view, glm::mat4& projection) {

	view = camera->GetViewMatrix(); // �۲����
	projection = camera->GetProjectionMatrix(SCR_WIDTH, SCR_HEIGHT); // ͶӰ����
}

// ���ƺ���
void drawBoxes(glm::mat4& view, glm::mat4& projection, glm::vec3 lightPos) {
	shader->activate();

	shader->setMat4("view", glm::value_ptr(view));
	shader->setMat4("projection", glm::value_ptr(projection));

	shader->setVec3("lightPos", lightPos);
	shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	shader->setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
	shader->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
	shader->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("material.shininess", 32.0f);

	shader->setVec3("viewPos", camera->Position);

	glBindVertexArray(VAO);

	for (unsigned int i = 0; i < 10; i++) {
		glm::mat4 model; float angle = 20.0f * (i + 1);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians(angle)*
			(float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));

		shader->setMat4("model", glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

// ����
void light(glm::mat4& view, glm::mat4& projection, glm::vec3 lightPos) {
	lightShader->activate();

	lightShader->setMat4("view", glm::value_ptr(view));
	lightShader->setMat4("projection", glm::value_ptr(projection));

	lightShader->setVec3("color", 1.0f, 1.0f, 1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader->setMat4("model", glm::value_ptr(model));

	// ���Ƶ����������
	glBindVertexArray(lightVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

// ��Ⱦ
void render() {
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	float radius = 5.0f;
	float lx = sin(glfwGetTime()*0.5) * radius;
	float lz = cos(glfwGetTime()*0.5) * radius;
	glm::vec3 lightPos = glm::vec3(lx, 0.0, lz);

	glm::mat4 view, projection;
	setupMatrices(view, projection);
	drawBoxes(view, projection, lightPos);
	light(view, projection, lightPos);
}

// ˢ�½���
void refresh() {
	clear();
	render();
}

// ����
void update() {
	updateDeltaTime();
	updateInput(window);

	refresh();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {

	// ��ʼ��GL
	if (!initialize()) return -1;

	// ��ʼ�� Shader ����
	shader = new Shader("vShader.sd", "fShader.sd");
	lightShader = new Shader("vShader.sd", "lShader.sd");

	// ��ʼ�������
	camera = new Camera(CAMER_POS);

	while (!glfwWindowShouldClose(window)) update();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &texture);

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;
	if (firstMouse) {
		lastX = xpos; lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos; lastY = ypos;

	camera->updateCameraRotate(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->updateScrollEvents(yoffset);
}

#endif
