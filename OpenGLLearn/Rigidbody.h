#pragma once

#include "Component.h"

class BoundingBox;

const float MinRigidbodyMass = 0.000001;
/*
 *	�������
 */
class Rigidbody : public Component {
public:
	Rigidbody(string name, BoundingBox* bb, bool positive = true);
	Rigidbody(string name, float mass, BoundingBox* bb, bool positive = true);
	~Rigidbody();

	virtual string cType() { return "Rigidbody"; };

	virtual Rigidbody* copy();

	void onCarsh(Rigidbody* rb);

	bool isPositived();
	void setPositive(bool val);

	glm::vec3 getMaxVelocity();
	void setMaxVelocity(glm::vec3 mv);

	glm::vec3 getMinVelocity();
	void setMinVelocity(glm::vec3 mv);

	BoundingBox* getBoundingBox();

	float getMass();
	void changeMass(float mass);

	void addVelocity(glm::vec3 velocity);
	void changeVelocity(glm::vec3 velocity);
	glm::vec3 getVelocity();
	/*
	void addAngularVelocity(glm::vec3 velocity);
	void changeAngularVelocity(glm::vec3 velocity);
	glm::vec3 getAngularVelocity();
	*/
	glm::vec3 getMomentum();

	void applyForce(glm::vec3 f, float time=0); // ʩ����
	void applyImpulse(glm::vec3 impulse); // ʩ�ӳ���

	vector<Rigidbody*> getCarshObjs();

private:
	bool positive;

	BoundingBox* boundingBox;

	float mass; // ����
	glm::vec3 velocity; // ˲ʱ�ٶ�
	glm::vec3 maxVelocity = glm::vec3(99999);
	glm::vec3 minVelocity = glm::vec3(0);
	
	//glm::vec3 angularVelocity; // ˲ʱ���ٶ�
	
	vector<Rigidbody*> carshedObjs;

	virtual void doUpdate();
	virtual void afterUpdate();

	void updatePhysics();
};

