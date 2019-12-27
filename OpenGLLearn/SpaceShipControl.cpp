#include "SpaceShipControl.h"

#include "MainScene.h"
#include "GameOverScene.h"

#include "Rigidbody.h"

SpaceShipControl::SpaceShipControl(string name, Rigidbody* rb, Node* ship,
	float moveSpeed /*= DefaultSpeed*/, 
	float rotateSpeed /*= DefaultRotateSpeed*/,
	float maxSpeed /*= MaxMoveSpeed*/) : Component(name),
	rb(rb), ship(ship), moveSpeed(moveSpeed), 
	rotateSpeed(rotateSpeed), maxSpeed(maxSpeed) {
	initRot = ship->getTransform()->relativeRotation();
	rb->changeVelocity(glm::vec3(0, 0, moveSpeed));
	rb->setMaxVelocity(glm::vec3(maxSpeed/4, 0, maxSpeed));
}

SpaceShipControl::~SpaceShipControl() {}

void SpaceShipControl::doUpdate() {
	Component::doUpdate();
	//updatePause();
	//if (pause) return;
	//updateMoving();
	vector<Rigidbody*> rbs = rb->getCarshObjs();
	if (rbs.size() > 0)
		SceneManager::Get()->gotoScene(new GameOverScene);
	updateReset();
}

void SpaceShipControl::updateMoving() {
	glm::vec3 pos = node->getTransform()->relativePosition();
	bool flag = false;
	/*
	if (pos.z < -ZRange) { pos.z = ZRange; flag = true; }
	if (pos.z > ZRange) { pos.z = -ZRange; flag = true; }
	if (pos.x < -XRange) { pos.x = XRange; flag = true; }
	if (pos.x > XRange) { pos.x = -XRange; flag = true; }
	*/
	if (flag) {
		node->getTransform()->moveTo(pos);
		node->getTransform()->resetPos();
	}
	//glm::vec3 pos = node->getTransform()->curPosition();
	//cout << "Ship:" << endl;
	//node->getTransform()->debugShow();
	cout << "Position: " << pos.x << "," << pos.y << "," << pos.z << endl;
}

void SpaceShipControl::updateReset() {
	if (!rotating) 
		ship->getTransform()->rotateTo(initRot);
	rotating = false;
}

SpaceShipControl* SpaceShipControl::copy() {
	SpaceShipControl* comp = new SpaceShipControl(getName(), 
		rb, ship, moveSpeed, rotateSpeed, maxSpeed);
	return (SpaceShipControl*)Component::copy(comp);
}

void SpaceShipControl::processMove(Movement direction) {
	if (direction == Forward) 
		rb->applyForce(glm::vec3(0, 0, 50));
	if (direction == Backward) 
		rb->applyForce(glm::vec3(0, 0, -150));
	if (direction == Up) 
		getTransform()->moveDelta(glm::vec3(0, 0.5, 0));
	if (direction == Down) 
		getTransform()->moveDelta(glm::vec3(0, -0.5, 0));
	if (direction == Right || direction == Left)
		processTurn(direction);
}

void SpaceShipControl::processTurn(Movement direction) {
	rotating = true;
	float dt = EventManager::Get()->getDeltaTime();
	float rotateDelta = rotateSpeed * dt;
	glm::vec3 srot = ship->getTransform()->relativeRotation();
	glm::vec3 rot = getTransform()->relativeRotation();
	if (direction == Right) {
		srot.z = glm::clamp(srot.z - rotateDelta, -MaxRotateAngle, MaxRotateAngle);
		rot.y = rot.y - rotateDelta;
	}
	if (direction == Left) {
		srot.z = glm::clamp(srot.z + rotateDelta, -MaxRotateAngle, MaxRotateAngle);
		rot.y = rot.y + rotateDelta;
	}
	ship->getTransform()->rotateTo(srot);
	getTransform()->rotateTo(rot);
}

