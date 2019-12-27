#include "Rigidbody.h"

#include "EventManager.h"

#include "PhysicsSystem.h"

#include "BoundingBox.h"

Rigidbody::Rigidbody(string name, BoundingBox* bb, bool positive) : 
	Rigidbody(name, 1, bb, positive) {}
Rigidbody::Rigidbody(string name, float mass, BoundingBox* bb, bool positive) : 
	Component(name), boundingBox(bb), positive(positive) {
	changeMass(mass);
	PhysicsSystem* phy = PhysicsSystem::Get();
	if (phy != NULL) phy->addRigidbody(this);
}

Rigidbody::~Rigidbody() {
	PhysicsSystem* phy = PhysicsSystem::Get();
	if (phy != NULL) phy->removeRigidbody(this);
}

Rigidbody* Rigidbody::copy() {
	Rigidbody* comp = new Rigidbody(getName(), boundingBox, positive);
	return (Rigidbody*)Component::copy(comp);
}

void Rigidbody::onCarsh(Rigidbody* rb) {
	carshedObjs.push_back(rb);
}

bool Rigidbody::isPositived() {
	return positive;
}
void Rigidbody::setPositive(bool val) {
	positive = val;
}

glm::vec3 Rigidbody::getMaxVelocity() {
	return maxVelocity;
}
void Rigidbody::setMaxVelocity(glm::vec3 mv) {
	maxVelocity = mv;
}

glm::vec3 Rigidbody::getMinVelocity() {
	return minVelocity;
}
void Rigidbody::setMinVelocity(glm::vec3 mv) {
	minVelocity = mv;
}

BoundingBox* Rigidbody::getBoundingBox() {
	return boundingBox;
}

float Rigidbody::getMass() {
	return mass;
}

void Rigidbody::changeMass(float mass) {
	this->mass = max(MinRigidbodyMass, mass);
}

void Rigidbody::addVelocity(glm::vec3 velocity) {
	changeVelocity(this->velocity+velocity);
}

void Rigidbody::changeVelocity(glm::vec3 velocity) {
	float maxX = maxVelocity.x, minX = minVelocity.x;
	float maxY = maxVelocity.y, minY = minVelocity.y;
	float maxZ = maxVelocity.z, minZ = minVelocity.z;

	velocity.x = min(velocity.x, maxX);
	velocity.y = min(velocity.y, maxY);
	velocity.z = min(velocity.z, maxZ);

	velocity.x = max(velocity.x, minX);
	velocity.y = max(velocity.y, minY);
	velocity.z = max(velocity.z, minZ);

	this->velocity = velocity;
}

glm::vec3 Rigidbody::getVelocity() {
	return velocity;
}
/*
void Rigidbody::addAngularVelocity(glm::vec3 velocity) {
	changeAngularVelocity(angularVelocity + velocity);
}

void Rigidbody::changeAngularVelocity(glm::vec3 velocity) {
	angularVelocity = velocity;
}

glm::vec3 Rigidbody::getAngularVelocity() {
	return angularVelocity;
}
*/
glm::vec3 Rigidbody::getMomentum() {
	return velocity * mass;
}

void Rigidbody::applyForce(glm::vec3 f, float time) {
	if (time == 0) time = EventManager::Get()->getDeltaTime();
	applyImpulse(f*time);
}

void Rigidbody::applyImpulse(glm::vec3 impulse) {
	glm::vec3 dv = impulse / mass;
	addVelocity(dv);
}

vector<Rigidbody*> Rigidbody::getCarshObjs() {
	return carshedObjs;
}

void Rigidbody::doUpdate() {
	/*
	if (carshedObjs.size() > 0) {
		cout << "Crashed! ";
		for (int i = 0; i < carshedObjs.size(); i++)
			cout << carshedObjs[i]->getName() << " ";
		cout << endl;
	}*/
	updatePhysics();
}

void Rigidbody::afterUpdate() {
	carshedObjs.clear();
}

void Rigidbody::updatePhysics() {
	float dt = EventManager::Get()->getDeltaTime();
	Transform* tf = getTransform();
	/*
	glm::vec3 da = angularVelocity * dt;
	tf->rotateDelta(da);
	*/
	glm::vec3 ds = velocity * dt;
	glm::vec4 _ds = glm::vec4(ds.x, ds.y, -ds.z, 1);
	_ds = tf->getRotateModel()*_ds;
	tf->moveDelta(_ds);
}
