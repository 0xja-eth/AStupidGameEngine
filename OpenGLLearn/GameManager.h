#pragma once

#include <cstdlib>
#include <iostream>
#include <cstring>

#include "OpenGLAdapter.h"
#include "EventManager.h"
#include "SceneManager.h"

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 780

#define GAME_TITLE "Game"

/*
 *	游戏最主要的管理类
 */
static class GameManager {
public:
	static const int ScreenWidth = SCREEN_WIDTH;
	static const int ScreenHeight = SCREEN_HEIGHT;

	static const string GameTitle;

	static void run();

	static void addManager(ManagerBase* manager);
	static void updateManagers();

private:
	static vector<ManagerBase*> managers;

	static void initialize();

	static void update();
};

