#include "EventManager.h"

EventManager* EventManager::self = new EventManager;

void EventManager::setup() {
	setupVariables();
	setupCallbacks();
}

void EventManager::beforeUpdate() {
	OpenGLAdapter::pollEvents();
}

void EventManager::doUpdate() {
	ManagerBase::doUpdate();
	updateDeltaTime();
}

void EventManager::afterUpdate() {
	clearDeltaVariables();
}

void EventManager::setupVariables() {
	lastTime = deltaTime = 0.0f;
	lastMousePos = glm::vec2(
		GameManager::ScreenWidth / 2, GameManager::ScreenHeight / 2);
	deltaMousePos = scrollOffset = glm::vec2(0);
}

void EventManager::setupCallbacks() {
	OpenGLAdapter::addCallback(CursorPos, Callback(cursorPosCallback));
	OpenGLAdapter::addCallback(Scroll, Callback(scrcollCallback));
}

float EventManager::getDeltaTime() { return deltaTime; }
glm::vec2 EventManager::getDeltaMousePos() { return deltaMousePos; }
glm::vec2 EventManager::getScrollOffset() { return scrollOffset; }

float EventManager::getLastTime() { return lastTime; }
glm::vec2 EventManager::getLastMousePos() { return lastMousePos; }

void EventManager::debugShow() {
	cout << "EventsManager::debugShow ====================" << endl;
	cout << "  deltaTime = " << deltaTime << endl;
	cout << "  deltaMousePos = " << deltaMousePos.x << ", " << deltaMousePos.y << endl;
	cout << "  lastTime = " << lastTime << endl;
	cout << "  lastMousePos = " << lastMousePos.x << ", " << lastMousePos.y << endl;
	cout << "  scrollOffset = " << scrollOffset.x << ", " << scrollOffset.y << endl;
}

void EventManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;
	glm::vec2 mousePos = glm::vec2(-ypos, xpos);
	// 如果是第一次移动，lastMousePos = mousePos
	if (firstMouse) {
		self->lastMousePos = mousePos;
		firstMouse = false;
	}
	self->deltaMousePos = mousePos - self->lastMousePos;
	self->lastMousePos = mousePos;
}

void EventManager::scrcollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	self->scrollOffset = glm::vec2(xoffset, yoffset);
}

void EventManager::updateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastTime;
	lastTime = currentFrame;
}

bool EventManager::isKeyPress(int key) {
	return OpenGLAdapter::isKeyPress(key);
}

void EventManager::clearDeltaVariables() {
	deltaTime = 0.0f;
	deltaMousePos = scrollOffset = glm::vec2(0);
}