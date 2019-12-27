#pragma once
#include "RenderableComponent.h"

#include "Rigidbody.h"

#include "MainSceneSystem.h"

class Camera;
class Rigidbody;
class MainSceneSystem;
class EnemyEffectRenderData : public RenderData {
};

const glm::vec3 MaxEnemyVelocity = glm::vec3(0, 0, 20);

class Enemy : public RenderableComponent {
public:
	Enemy(string name, Rigidbody* rb);
	~Enemy();

	virtual string cType() { return "Enemy"; };

	virtual Enemy* copy();

	virtual RenderData* getRenderData();

private:

	float explodeTime = -1;

	Rigidbody* rb;

	MainSceneSystem* manager;

	EnemyEffectRenderData renderData;

	void generateRandomVelocity();

	virtual void doUpdate();

	void updateOutBoundary();

	bool checkOutBoundary();

	virtual bool testCapture(Camera* camera);
};

