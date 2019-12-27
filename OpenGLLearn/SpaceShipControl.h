#pragma once
#include "Component.h"

const float DefaultMoveSpeed = 0;
const float DefaultRotateSpeed = 360;
const float MaxMoveSpeed = 500;
const float MaxRotateAngle = 45;

class Rigidbody;

enum Movement {
	Forward, Backward, Left, Right, Up, Down
};

/*
 *	”Ó÷Ê∑…¥¨
 */
class SpaceShipControl : public Component {
public:
	SpaceShipControl(string name, Rigidbody* rb, Node* ship, 
		float moveSpeed = DefaultMoveSpeed, 
		float rotateSpeed = DefaultRotateSpeed,
		float maxSpeed = MaxMoveSpeed);
	~SpaceShipControl();

	virtual string cType() { return "SpaceShipControl"; };

	virtual SpaceShipControl* copy();

	void processMove(Movement direction);
	void processTurn(Movement direction);

private:
	Node* ship;
	Rigidbody* rb;

	glm::vec3 initRot;

	float moveSpeed;
	float rotateSpeed;
	float maxSpeed;

	bool rotating = false;

	virtual void doUpdate();

	void updateMoving();
	void updateReset();

};

