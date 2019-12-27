#pragma once

#include <set>

#include <glm/glm.hpp>

#include "ManagerBase.h"

const int DL = 500;
const int GL = 450;

const int W = 150;
const int H = 150;
const float Offset = 0.1;

const int MaxScale = 500;
const int MinScale = 50;

const int MaxRocks = 100;

class MainScene;
class Rock;
class Enemy;
class SpaceShipControl;

class MainSceneSystem : public SystemBase {
public:
	MainSceneSystem(Scene* scene) : SystemBase(scene) {};

	static const string Name;

	static MainSceneSystem* Get();

	virtual string name() { return Name; };

	virtual void setup();

	void setShipControl(SpaceShipControl* ship);

	void addRock(Rock* rock);
	void removeRock(Rock* rock);
	void clearRocks();

	void addEnemy(Enemy* enemy);
	void removeEnemy(Enemy* enemy);
	void clearEnemies();

	bool checkOutBoundary(glm::vec3 pos);

private:
	float level;
	float sumTime = 0;
	float lastRockTime = 0;
	float lastEnemyTime = 0;

	int score = 0;

	bool pause = false;

	SpaceShipControl* ship;

	set<Rock*> rocks;
	set<Enemy*> enemies;

	glm::vec3 dBox[2], gBox[2];

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();

	void updateControl();

	void updateLevel();
	void updateDetectBox();
	void updateGenBox();

	void updateEntities();

	void updateRocks();
	void updateEnemies();

	void createRock();
	void createRocks(int count);

	void createEnemy();
	void createEnemies(int count);

	glm::vec3 generateRandomPoint();
	glm::vec3 generateRandomRotate();
	glm::vec3 generateRandomScale();
};
