/*

#include "Scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//#define SCR_WIDTH 1600
//#define SCR_HEIGHT 900

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#define WIN_TITLE "LearnOpenGL"

#define BORDER_COLOR { 1.0f, 1.0f, 0.0f, 1.0f }

#define CAMER_POS glm::vec3(0.0f, 0.0f, 3.0f)

#define TEST_TEXTURE

using namespace std;

GLFWwindow* window;
Scene* scene;

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastTime = 0.0f; // ��һ֡��ʱ��
float lastX = SCR_WIDTH / 2; // ��һ֡���Xλ��
float lastY = SCR_HEIGHT / 2; //  ��һ֡���Yλ��

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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

// ���ڴ�С�ı�ص�
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	scene->mouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	scene->scrollCallback(window, xoffset, yoffset);
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

// ��ʼ��GL����
void initGLObjects() {
	// ��Ȳ���
	glEnable(GL_DEPTH_TEST);
	scene = new Scene(SCR_WIDTH, SCR_HEIGHT);
}

// ��ʼ��
bool initialize() {
	initGLFW();
	if (!createGLWindow()) return false;
	if (!setupGLAD()) return false;
	initGLObjects();
	return true;
}

int main() {
	// ��ʼ��GL
	if (!initialize()) return -1;

	while (!glfwWindowShouldClose(window)) scene->update(window);

	glfwTerminate();
	return 0;
}
*/

#include "GameManager.h"

int main() {
	GameManager::run();
}