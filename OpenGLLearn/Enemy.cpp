#include "Enemy.h"

#include "Node.h"
#include "Transform.h"

Enemy::Enemy(string name, Rigidbody* rb) {
	manager = MainSceneSystem::Get();
	if (manager != NULL) manager->addEnemy(this);
	generateRandomVelocity();
}

Enemy::~Enemy() {
	manager->removeEnemy(this);
}

Enemy* Enemy::copy() {
	Enemy* comp = new Enemy(getName(), rb);
	return (Enemy*)Component::copy(comp);
}

RenderData* Enemy::getRenderData() {
	return &renderData;
}

void Enemy::generateRandomVelocity() {
	float vz = rand() % int(MaxEnemyVelocity.z/2 * 100) / 100 + MaxEnemyVelocity.z/2;
	glm::vec3 v = glm::vec3(0, 0, vz);
	rb->changeVelocity(v);
}

void Enemy::doUpdate() {
	updateOutBoundary();
}

void Enemy::updateOutBoundary() {
	if (checkOutBoundary())
		this->node->destroy();
}

bool Enemy::checkOutBoundary() {
	if (manager == NULL) return false;
	glm::vec3 pos = getTransform()->curPosition();
	return manager->checkOutBoundary(pos);
}

bool Enemy::testCapture(Camera* camera) {
	return false;
}