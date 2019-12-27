#include "Rock.h"

#include "EventManager.h"

#include "MainSceneSystem.h"

#include "Node.h"

#include "Transform.h"

#include "Rigidbody.h"

Rock::Rock(string name, Rigidbody* rb) : RenderableComponent(name), rb(rb) {
	manager = MainSceneSystem::Get();
	if(manager!=NULL) manager->addRock(this);
	generateRandomVelocity();
}

Rock::~Rock() {
	manager->removeRock(this);
}

Rock* Rock::copy() {
	Rock* comp = new Rock(getName(), rb);
	return (Rock*)Component::copy(comp);
}

RenderData* Rock::getRenderData() {
	return &renderData;
}

void Rock::explode() {
	explodeTime = ExplodeTime;
}

void Rock::generateRandomVelocity() {
	float vx = rand() % int(MaxRockVelocity.x * 100) / 100;
	float vy = rand() % int(MaxRockVelocity.y * 100) / 100;
	float vz = rand() % int(MaxRockVelocity.z * 100) / 100;
	glm::vec3 v = glm::vec3(vx, vy, vz);
	rb->changeVelocity(v);
}

void Rock::doUpdate() {
	updateOutBoundary();
	updateExplode();
}

void Rock::updateOutBoundary() {
	if (checkOutBoundary())
		this->node->destroy();
}

void Rock::updateExplode() {
	if (explodeTime <= 0) return;
	explodeTime -= EventManager::Get()->getDeltaTime();
	if (explodeTime < 0) onExploded();
}

bool Rock::checkOutBoundary() {
	if (manager == NULL) return false;
	glm::vec3 pos = getTransform()->curPosition();
	return manager->checkOutBoundary(pos);
}

float Rock::getExplodeDuration() {
	return (ExplodeTime - explodeTime) / 
		ExplodeTime * (glm::pi<float>() / 2);
}

void Rock::onExploded() {
	this->node->destroy();
}

bool Rock::testCapture(Camera* camera) {
	return false;
}

/*
bool Rock::testCapture(Camera* camera) {
	return explodeTime > 0;
}
*/
void RockEffectRenderData::setup(Shader* shader, float duration) {
	this->shader = shader;
	this->duration = duration;
}

void RockEffectRenderData::doConfig() {
	shader->activate();
	shader->setFloat("exploding", duration>=0);
	shader->setFloat("duration", duration);
}
