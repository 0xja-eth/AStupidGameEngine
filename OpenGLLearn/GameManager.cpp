#include "GameManager.h"
#include "PhysicsSystem.h"

const string GameManager::GameTitle = GAME_TITLE;

vector<ManagerBase*> GameManager::managers;

void GameManager::run() {
	try {
		initialize(); // 初始化
		while (!OpenGLAdapter::shouldClose())
			update(); // 游戏循环
	} catch (GameException& e) {
		cout << e.what() << endl;
	}
	glfwTerminate(); // 结束
}

void GameManager::initialize() {
	OpenGLAdapter::setup(ScreenWidth, ScreenHeight, GameTitle);

	addManager(EventManager::Get());
	addManager(SceneManager::Get());
	//addManager(RenderManager::Get());
	//addManager(PhysicsManager::Get());
}

void GameManager::update() {
	updateManagers();
}

void GameManager::addManager(ManagerBase* manager) {
	managers.push_back(manager);
	manager->setup();
}

void GameManager::updateManagers() {
	for (int i = 0; i < managers.size(); i++)
		managers[i]->callBeforeUpdate();
	for (int i = 0; i < managers.size(); i++)
		managers[i]->callDoUpdate();
	for (int i = 0; i < managers.size(); i++)
		managers[i]->callAfterUpdate();
}
