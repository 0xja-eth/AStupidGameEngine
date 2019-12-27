#pragma once

#include "ManagerBase.h"

#include "OpenGLAdapter.h"
#include "GameManager.h"

//#define DEBUG_SHOW

class EventManager : public ManagerBase {
public:
	EventManager() : ManagerBase() {};

	static EventManager* Get() { 
		return self == NULL ? self = new EventManager() : self; 
	}

	virtual string getName() { return "1Event"; };

	virtual void setup();

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();

	bool isKeyPress(int key);

	void clearDeltaVariables();

	float getDeltaTime();
	glm::vec2 getDeltaMousePos();
	glm::vec2 getScrollOffset();

	float getLastTime();
	glm::vec2 getLastMousePos();

	virtual void debugShow();

private:
	static EventManager* self;

	float deltaTime;
	glm::vec2 deltaMousePos;

	float lastTime;
	glm::vec2 lastMousePos;

	glm::vec2 scrollOffset;

	void setupVariables();
	void setupCallbacks();

	void updateDeltaTime();

	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrcollCallback(GLFWwindow* window, double xoffset, double yoffset);

};

