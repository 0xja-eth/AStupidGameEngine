#include "MainSceneSystem.h"

#include <ctime>

#include "MainScene.h"

#include "SpaceShipControl.h"

#include "Rock.h"
#include "Enemy.h"

const string MainSceneSystem::Name = "MainSceneSystem";

MainSceneSystem* MainSceneSystem::Get() {
	Scene* scene = SceneManager::Get()->currentScene();
	return (MainSceneSystem*)scene->getSystem(Name);
}

void MainSceneSystem::setup() {
	srand(time(0));
}

void MainSceneSystem::setShipControl(SpaceShipControl* ship) {
	this->ship = ship;
	updateDetectBox();
	updateGenBox();
	createRocks(5);
	createEnemies(2);
}

void MainSceneSystem::addRock(Rock* rock) {
	rocks.insert(rock);
}

void MainSceneSystem::removeRock(Rock* rock) {
	rocks.erase(rocks.find(rock));
}

void MainSceneSystem::clearRocks() {
	rocks.clear();
}

void MainSceneSystem::addEnemy(Enemy* enemy) {
	enemies.insert(enemy);
}

void MainSceneSystem::removeEnemy(Enemy* enemy) {
	enemies.erase(enemies.find(enemy));
}

void MainSceneSystem::clearEnemies() {
	enemies.clear();
}

bool MainSceneSystem::checkOutBoundary(glm::vec3 pos) {
	glm::vec3 minP = dBox[0], maxP = dBox[1];
	return !(minP.x < pos.x && pos.x < maxP.x &&
		minP.y < pos.y && pos.y < maxP.y &&
		minP.z < pos.z && pos.z < maxP.z);
}

void MainSceneSystem::beforeUpdate() {

}

void MainSceneSystem::doUpdate() {
	updateControl();
	updateDetectBox();
	updateGenBox();
	updateEntities();
}

void MainSceneSystem::afterUpdate() {

}

void MainSceneSystem::updateControl() {
	//if (EventManager::Get()->isKeyPress(GLFW_KEY_SPACE)) pause = !pause;
	if (pause) return;

	if (EventManager::Get()->isKeyPress(GLFW_KEY_W))
		ship->processMove(Movement(Forward));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_S))
		ship->processMove(Movement(Backward));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_A))
		ship->processMove(Movement(Left));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_D))
		ship->processMove(Movement(Right));

	if (EventManager::Get()->isKeyPress(GLFW_KEY_UP))
		ship->processMove(Movement(Up));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_DOWN))
		ship->processMove(Movement(Down));
}

void MainSceneSystem::updateLevel() {
	sumTime += EventManager::Get()->getDeltaTime();
}

void MainSceneSystem::updateDetectBox() {
	glm::vec3 pos = ship->getTransform()->curPosition();
	dBox[0] = glm::vec3(pos.x - W / 2, pos.y - H / 2, pos.z - DL * (1 - Offset));
	dBox[1] = glm::vec3(pos.x + W / 2, pos.y + H / 2, pos.z + DL * Offset);
}

void MainSceneSystem::updateGenBox() {
	gBox[0] = dBox[0];
	gBox[1] = glm::vec3(dBox[1].x, dBox[1].y, dBox[0].z + (DL - GL));
}

void MainSceneSystem::updateEntities() {
	updateEnemies();
	updateRocks();
}

void MainSceneSystem::updateEnemies() {
	float curTime = EventManager::Get()->getLastTime();
	float delta = (rand() % 300) / 100.0 + 10;
	if (curTime - lastEnemyTime >= delta)
		createEnemy();
}

void MainSceneSystem::createEnemy() {
	lastEnemyTime = EventManager::Get()->getLastTime();
	glm::vec3 pos = generateRandomPoint();

	((MainScene*)scene)->createEnemy(pos);
}

void MainSceneSystem::createEnemies(int count) {
	for (int i = 0; i < count; i++) createEnemy();
}

void MainSceneSystem::updateRocks() {
	if (rocks.size() > MaxRocks) return;
	float curTime = EventManager::Get()->getLastTime();
	float delta = (rand() % 200) / 100.0;
	if (curTime - lastRockTime >= delta)
		createRock();
}

void MainSceneSystem::createRock() {
	lastRockTime = EventManager::Get()->getLastTime();
	glm::vec3 pos = generateRandomPoint();
	glm::vec3 rot = generateRandomRotate();
	glm::vec3 sca = generateRandomScale();

	((MainScene*)scene)->createRock(pos, rot, sca);
}

void MainSceneSystem::createRocks(int count) {
	for (int i = 0; i < count; i++) createRock();
}

glm::vec3 MainSceneSystem::generateRandomPoint() {
	float px = rand() % int((gBox[1].x - gBox[0].x) + 1) + gBox[0].x;
	float py = rand() % int((gBox[1].y - gBox[0].y) + 1) + gBox[0].y;
	float pz = rand() % int((gBox[1].z - gBox[0].z) + 1) + gBox[0].z;
	return glm::vec3(px, py, pz);
}

glm::vec3 MainSceneSystem::generateRandomRotate() {
	float rx = rand() % 180, ry = rand() % 180, rz = rand() % 180;
	return glm::vec3(rx, ry, rz);
}

glm::vec3 MainSceneSystem::generateRandomScale() {
	float scale = (rand() % (MaxScale - MinScale + 1) + MinScale) / 100.0;
	return glm::vec3(scale);
}
