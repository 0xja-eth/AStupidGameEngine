#include "ManagerBase.h"
#include "GameManager.h"

bool ManagerBase::isEnabled() const {
	return enabled;
}

void ManagerBase::setEnabled(bool val) {
	enabled = val;
}

void ManagerBase::setup() {}

void ManagerBase::doUpdate() {
#ifdef DEBUG_SHOW
	debugShow();
#endif
}

void ManagerBase::debugShow() {}

SystemBase::SystemBase(Scene* scene) : scene(scene) {
	setup();
}
