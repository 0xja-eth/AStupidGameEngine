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

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float lastX = SCR_WIDTH / 2; // 上一帧鼠标X位置
float lastY = SCR_HEIGHT / 2; //  上一帧鼠标Y位置

#pragma region 顶点数据定义

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
/*
float vertices[] = {
	// 位置              // 颜色
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
*/

float vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // 右上
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // 右下
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,  // 左下
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,  // 左上
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		 0.5f,  0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // 右上
		 0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // 右下
		-0.5f, -0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f,  // 左下
		-0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f,  // 左上
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		-0.5f,  0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // 后上
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, -1.0f,  0.0f,  0.0f,  // 后下
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // 前下
		-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, -1.0f,  0.0f,  0.0f,  // 前上
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // 后上
		0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,  // 后下
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // 前下
		0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,  // 前上
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		 0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // 右后
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // 右前
		-0.5f, -0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f,  0.0f, -1.0f,  0.0f,  // 左前
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  0.0f, -1.0f,  0.0f,  // 左后
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -    --- 法向量 ---
		 0.5f, 0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // 右后
		 0.5f, 0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // 右前
		-0.5f, 0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, 1.0f,  0.0f,   // 左前
		-0.5f, 0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 1.0f,   0.0f, 1.0f,  0.0f,   // 左后
};

/*
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 1.5f,  // 右下角
	-0.5f, -0.5f, 1.0f, // 左下角
	-0.5f, 0.5f, -1.0f,  // 左上角
	-0.5f, 0.0f, -0.0f,  // 左中角
	0.0f, 0.0f, 3.0f,   // 中中角
};
*/

// 纹理坐标
float texCoords[] = {
	0.0f, 0.0f, // 左下角
	1.0f, 0.0f, // 右下角
	0.5f, 1.0f // 上中
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3,  // 第1个三角形
	1, 2, 3,  // 第2个三角形

	4, 6, 7,  // 第3个三角形
	4, 6, 5,  // 第4个三角形

	8, 10,11, // 第5个三角形
	8, 10,9, // 第6个三角形

	12,13,15, // 第7个三角形
	13,14,15, // 第8个三角形

	16,18,19, // 第9个三角形
	16,18,17, // 第10个三角形

	20,21,23, // 第11个三角形
	21,22,23, // 第12个三角形
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
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
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

// GL 对象
unsigned int VAO, VBO, EBO;
unsigned int lightVAO;
unsigned int texture;

// 设置顶点属性
void setupVertexAttrs() {
	// 设定顶点位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 设置顶点颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 设置顶点纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// 设置法向量坐标属性
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
}

// 设置灯顶点属性（只有位置数据）
void setupLightAttrs() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11  * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

// 初始化顶点和缓冲对象
void initVertexBuffers() {

	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// 绑定顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 绑定索引数组
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	setupVertexAttrs();

	// 绑定灯顶点数组对象
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	setupLightAttrs();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// 配置纹理
void setupTexture() {
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load("box.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(data);
}

// 初始化纹理
void initTexture() {
	// 创建纹理
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	// 边缘重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// 过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 配置边框颜色
	float borderColor[] = BORDER_COLOR;
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	setupTexture();
}

// 初始化GL对象
void initGLObjects() {
	// 深度测试
	glEnable(GL_DEPTH_TEST);
	initVertexBuffers();
	initTexture();
}

// 初始化
bool initialize() {
	initGLFW();
	if (!createGLWindow()) return false;
	if (!setupGLAD()) return false;
	initGLObjects();
	return true;
}

// 更新获取Delta时间
void updateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

// 更新输入时间
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

// 清除界面
void clear() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// 配置观察/投影矩阵
void setupMatrices(glm::mat4& view, glm::mat4& projection) {

	view = camera->GetViewMatrix(); // 观察矩阵
	projection = camera->GetProjectionMatrix(SCR_WIDTH, SCR_HEIGHT); // 投影矩阵
}

// 绘制盒子
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

// 光照
void light(glm::mat4& view, glm::mat4& projection, glm::vec3 lightPos) {
	lightShader->activate();

	lightShader->setMat4("view", glm::value_ptr(view));
	lightShader->setMat4("projection", glm::value_ptr(projection));

	lightShader->setVec3("color", 1.0f, 1.0f, 1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader->setMat4("model", glm::value_ptr(model));

	// 绘制灯立方体对象
	glBindVertexArray(lightVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

// 渲染
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

// 刷新界面
void refresh() {
	clear();
	render();
}

// 更新
void update() {
	updateDeltaTime();
	updateInput(window);

	refresh();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {

	// 初始化GL
	if (!initialize()) return -1;

	// 初始化 Shader 程序
	shader = new Shader("vShader.sd", "fShader.sd");
	lightShader = new Shader("vShader.sd", "lShader.sd");

	// 初始化摄像机
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
