#include "GameManager.h"
#include "PhysicsSystem.h"

const string GameManager::GameTitle = GAME_TITLE;

vector<ManagerBase*> GameManager::managers;

void GameManager::run() {
	try {
		initialize(); // ��ʼ��
		while (!OpenGLAdapter::shouldClose())
			update(); // ��Ϸѭ��
	} catch (GameException& e) {
		cout << e.what() << endl;
	}
	glfwTerminate(); // ����
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
