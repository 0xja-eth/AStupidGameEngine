#pragma once

#include "RenderableComponent.h"

class MainSceneSystem;
class Rigidbody;

class RockEffectRenderData : public RenderData {
public:
	void setup(Shader* shader, float duration);
private:
	Shader* shader;
	float duration;

	virtual void doConfig();
};

const float ExplodeTime = 2;
const glm::vec3 MaxRockVelocity = glm::vec3(5, 5, 5);

class Rock : public RenderableComponent {
public:
	Rock(string name, Rigidbody* rb);
	~Rock();

	virtual string cType() { return "Rock"; };

	virtual Rock* copy();

	virtual RenderData* getRenderData();

	void explode();

private:

	float explodeTime = -1;

	Rigidbody* rb;

	MainSceneSystem* manager;

	RockEffectRenderData renderData;

	void generateRandomVelocity();

	virtual void doUpdate();

	void updateOutBoundary();
	void updateExplode();

	bool checkOutBoundary();

	float getExplodeDuration();

	void onExploded();

	virtual bool testCapture(Camera* camera);
};

