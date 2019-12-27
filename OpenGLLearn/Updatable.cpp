#include "Updatable.h"

bool Updatable::isEnabled() const { return true; }

void Updatable::callBeforeUpdate() {
	if (isEnabled()) beforeUpdate();
}

void Updatable::callDoUpdate() {
	if (isEnabled()) doUpdate();
}

void Updatable::callAfterUpdate() {
	if (isEnabled()) afterUpdate();
}

void Updatable::beforeUpdate() {}

void Updatable::doUpdate() {}

void Updatable::afterUpdate() {}
