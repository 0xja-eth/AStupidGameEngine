#include "OpenGLAdapter.h"

int OpenGLAdapter::width;
int OpenGLAdapter::height;

string OpenGLAdapter::title;

GLFWwindow* OpenGLAdapter::window;
map<CallbackType, CallbackList*> OpenGLAdapter::callbackMap;

/*
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

typedef void(*callback)(double, double, double);
typedef vector<callback> cbs;
typedef pair<int, cbs> cbp;

map<int, vector<callback>*> callbackMap;

void cb1(double a, double b) {
	cout << "cb1:" << a << ", " << b << endl;
}
void cb2(double a, double b, double c) {
	cout << "cb2:" << a << ", " << b << ", " << c << endl;
}
void cb3(double a, double b, double c) {
	cout << "cb3:" << a << ", " << b << ", " << c << endl;
}
void cb4(double a, double b, double c) {
	cout << "cb4:" << a << ", " << b << ", " << c << endl;
}

int main() {
	cout << "Hello W3Cschool!";

	callbackMap[1] = new cbs();
	callbackMap[2] = new cbs();
	callbackMap[3] = new cbs();

	cbs* callbacks = callbackMap.find(1)->second;

	callbacks->push_back(callback(cb1));
	callbacks->push_back(cb2);
	callbacks->push_back(callback(cb3));

	cout << "Test 1:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks->erase(find(callbacks->begin(), callbacks->end(), cb2));

	cout << "Test 2:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks->push_back(callback(cb3));

	cout << "Test 3:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks->push_back(callback(cb3));

	cout << "Test 4:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks->erase(find(callbacks->begin(), callbacks->end(), cb3));

	cout << "Test 5:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks = callbackMap.find(1)->second;

	cout << "Test 6:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	callbacks = callbackMap.find(2)->second;

	cout << "Test 7:" << endl;

	for (int i = 0; i < callbacks->size(); i++)
		(*callbacks)[i](i, i*i, 2.0);

	return 0;
}

*/

#pragma region 初始化

void OpenGLAdapter::setup(int width, int height, string title) {
	OpenGLAdapter::width = width;
	OpenGLAdapter::height = height;
	OpenGLAdapter::title = title;

	setupCallbackMap();
	setupGLFW(); setupGLAD();
	setupGLOthers();
}

void OpenGLAdapter::setupCallbackMap() {
	// 对 map 中每个枚举类型都进行初始化
	for (CallbackType t = CallbackType(FirstCallbackType+1);
		t < LastCallbackType; t = CallbackType(t + 1))
		callbackMap[t] = new CallbackList();

	// 添加默认的回调函数
	addCallback(FramebufferSize, Callback(defaultFramebufferSizeCallback));
}

void OpenGLAdapter::setupGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	setupGLWindow();
}

void OpenGLAdapter::setupGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw GameException("Failed to initialize GLAD.");
}

void OpenGLAdapter::setupGLWindow() {
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) throw GameException("Failed to create GLFW window.");
	setupCallbacks();
}

void OpenGLAdapter::setupCallbacks() {
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 锁定鼠标
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

void OpenGLAdapter::setupGLOthers() {
	glEnable(GL_CULL_FACE); // 开启面剔除
	glEnable(GL_DEPTH_TEST); // 开启深度测试
}

#pragma endregion

#pragma region 基本封装

GLFWwindow* OpenGLAdapter::getWindow() { return window; }

void OpenGLAdapter::close() {
	glfwSetWindowShouldClose(window, true);
}

bool OpenGLAdapter::shouldClose() { return glfwWindowShouldClose(window); }

void OpenGLAdapter::swapBuffers() { glfwSwapBuffers(window); }

void OpenGLAdapter::pollEvents() { glfwPollEvents(); }

bool OpenGLAdapter::isKeyPress(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void OpenGLAdapter::clearScreen(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#pragma endregion

#pragma region 回调函数管理

CallbackList* OpenGLAdapter::getCallbackList(CallbackType type) {
	return callbackMap.find(type)->second;
}

void OpenGLAdapter::addCallback(CallbackType type, Callback cb) {
	CallbackList* cbs = getCallbackList(type);
	if (hasCallback(cbs, cb)) return;
	cbs->push_back(cb);
}
void OpenGLAdapter::removeCallback(CallbackType type, Callback cb) {
	CallbackList* cbs = getCallbackList(type);
	CallbackIter cbi = findCallback(cbs, cb);
	if (cbi == cbs->end()) return; // 如果不存在
	cbs->erase(cbi);
}

bool OpenGLAdapter::hasCallback(CallbackType type, Callback cb) {
	CallbackList* cbs = getCallbackList(type);
	return hasCallback(cbs, cb);
}
bool OpenGLAdapter::hasCallback(CallbackList* cbs, Callback cb) {
	return findCallback(cbs, cb) != cbs->end();
}

CallbackIter OpenGLAdapter::findCallback(CallbackList* cbs, Callback cb) {
	return find(cbs->begin(), cbs->end(), cb);
}

void OpenGLAdapter::handleCallback(CallbackType type, 
	GLFWwindow* window, double p1, double p2, double p3) {
	//cout << "handlingCallback: " << type << ": " << p1 << ", " << p2 << ", " << p3 << endl;

	CallbackList* cbs = getCallbackList(type);
	//cout << "  callbackList.size() = " << cbs->size() << endl;
	for (int i = 0; i < cbs->size(); i++) (*cbs)[i](window, p1, p2, p3);
}

#pragma endregion

#pragma region 回调函数配置

void OpenGLAdapter::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	handleCallback(FramebufferSize, window, width, height);
}
void OpenGLAdapter::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	handleCallback(CursorPos, window, xpos, ypos);
	/* 原本回调函数：
	static bool firstMouse = true;
	if (firstMouse) {
		lastX = xpos; lastY = ypos; firstMouse = false;
	}

	float xoffset = xpos - lastX, yoffset = lastY - ypos;
	lastX = xpos; lastY = ypos;

	Scene* scene = SceneManager::currentScene();
	scene->mouseCallback(xoffset, yoffset);
	*/
}
void OpenGLAdapter::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	handleCallback(Scroll, window, xoffset, yoffset);
	/* 原本回调函数：
	Scene* scene = SceneManager::currentScene();
	scene->scrollCallback(xoffset, yoffset);
	*/
}

void OpenGLAdapter::defaultFramebufferSizeCallback(
	GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

#pragma endregion
