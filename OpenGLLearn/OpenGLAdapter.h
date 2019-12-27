#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <vector>
#include <cstring>
#include <algorithm>

#include "GameException.h"

using namespace std;

typedef void(*Callback)(GLFWwindow*, double, double, double);
typedef vector<Callback> CallbackList;
typedef CallbackList::iterator CallbackIter;

enum CallbackType {
	FirstCallbackType,
	FramebufferSize, CursorPos, Scroll,
	LastCallbackType,
};

static class OpenGLAdapter {
public:

	static void setup(int width, int height, string title);

	/* ������װ */
	static GLFWwindow* getWindow();

	static void close();

	static bool shouldClose();
	static void swapBuffers();
	static void pollEvents();

	static void clearScreen(glm::vec4 color);

	static bool isKeyPress(int key);

	/* �ص��������� */
	static CallbackList* getCallbackList(CallbackType type);

	static void addCallback(CallbackType type, Callback cb);
	static void removeCallback(CallbackType type, Callback cb);
	static bool hasCallback(CallbackType type, Callback cb);

private:
	static int width, height;
	static string title;

	static GLFWwindow* window;
	static map<CallbackType, CallbackList*> callbackMap;

	/* ��ʼ�����躯�� */
	static void setupCallbackMap();

	static void setupGLFW();
	static void setupGLAD();
	static void setupGLWindow();
	static void setupCallbacks();

	static void setupGLOthers();

	/* �ص��������� */
	static bool hasCallback(CallbackList* cbs, Callback cb);
	static CallbackIter findCallback(CallbackList* cbs, Callback cb);

	static void handleCallback(CallbackType type, GLFWwindow* window,
		double p1 = 0, double p2 = 0, double p3 = 0);

	/* ֧�ֵĻص����� */
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	/* Ĭ�ϻص����� */
	static void defaultFramebufferSizeCallback(GLFWwindow* window, int width, int height);

};
