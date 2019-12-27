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

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastTime = 0.0f; // 上一帧的时间
float lastX = SCR_WIDTH / 2; // 上一帧鼠标X位置
float lastY = SCR_HEIGHT / 2; //  上一帧鼠标Y位置

// 初始化 GLFW
void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

// 配置GLAD
bool setupGLAD() {
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}

// 窗口大小改变回调
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	scene->mouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	scene->scrollCallback(window, xoffset, yoffset);
}
// 回调初始化
void setupCallback() {
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 锁定鼠标
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

// 创建GL窗口
bool createGLWindow() {
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); return false;
	}
	setupCallback();
	return true;
}

// 初始化GL对象
void initGLObjects() {
	// 深度测试
	glEnable(GL_DEPTH_TEST);
	scene = new Scene(SCR_WIDTH, SCR_HEIGHT);
}

// 初始化
bool initialize() {
	initGLFW();
	if (!createGLWindow()) return false;
	if (!setupGLAD()) return false;
	initGLObjects();
	return true;
}

int main() {
	// 初始化GL
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