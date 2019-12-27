#pragma once

#include <set>

#include "ManagerBase.h"

using namespace std;

class Scene;
class BoundingBox;
class Rigidbody;
/*
 *	物理系统
 */
class PhysicsSystem : public SystemBase {
public:
	PhysicsSystem(Scene* scene) : SystemBase(scene) {};

	static PhysicsSystem* Get();

	virtual string name() { return "PhysicsSystem"; };

	virtual void setup();

	void addRigidbody(Rigidbody* rb);
	void removeRigidbody(Rigidbody* rb);
	void clearRigidbody();

	bool checkCrash(BoundingBox* a, BoundingBox* b);
	bool checkCrash(Rigidbody* a, Rigidbody* b);

private:
	set<Rigidbody*> rigidbodies;

	void updateRigidbodies();
	void checkCrashes();
	void checkCrashesForObj(Rigidbody* obj);

	void onCrash(Rigidbody* a, Rigidbody* b);

	virtual void doUpdate();

};

