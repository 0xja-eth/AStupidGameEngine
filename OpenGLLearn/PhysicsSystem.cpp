#include "PhysicsSystem.h"

#include "SceneManager.h"

#include "BoundingBox.h"
#include "Rigidbody.h"

PhysicsSystem* PhysicsSystem::Get() {
	Scene* scene = SceneManager::Get()->currentScene();
	return scene->getPhysicsSystem();
}

void PhysicsSystem::setup() {

}

void PhysicsSystem::addRigidbody(Rigidbody* rb) {
	rigidbodies.insert(rb);
}

void PhysicsSystem::removeRigidbody(Rigidbody* rb) {
	rigidbodies.erase(rigidbodies.find(rb));
}

void PhysicsSystem::clearRigidbody() {
	rigidbodies.clear();
}

void PhysicsSystem::doUpdate() {
	updateRigidbodies();
}

bool PhysicsSystem::checkCrash(BoundingBox* a, BoundingBox* b) {
	return a != NULL && b != NULL && (a->check(b) || b->check(a));
}
bool PhysicsSystem::checkCrash(Rigidbody* a, Rigidbody* b) {
	return checkCrash(a->getBoundingBox(), b->getBoundingBox());
}

void PhysicsSystem::updateRigidbodies() {
	checkCrashes();
}

void PhysicsSystem::checkCrashes() {
	set<Rigidbody*>::iterator rit = rigidbodies.begin();
	for (; rit != rigidbodies.end(); ++rit) 
		if ((*rit)->isPositived())
			checkCrashesForObj(*rit);
}

void PhysicsSystem::checkCrashesForObj(Rigidbody* obj) {
	set<Rigidbody*>::iterator rit = rigidbodies.begin();
	for (; rit != rigidbodies.end(); ++rit) {
		Rigidbody* b = *rit; if (b == obj) continue;
		if (checkCrash(obj, b)) onCrash(obj, b);
	}
}

void PhysicsSystem::onCrash(Rigidbody* a, Rigidbody* b) {
	a->onCarsh(b); b->onCarsh(a);
}
