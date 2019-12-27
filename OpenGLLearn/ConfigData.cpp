#include "ConfigData.h"

void ConfigData::addNext(ConfigData* next) {
	if (next == NULL) return;
	this->next.insert(next);
}
void ConfigData::addNext(vector<ConfigData*> nexts) {
	for (int i = 0; i < nexts.size(); i++) addNext(nexts[i]);
}

void ConfigData::clearNexts() {
	next.clear();
}

void ConfigData::config() {
	beforeConfig();
	configChildren();
	doConfig();
	afterConfig();
}

void ConfigData::configChildren() {
	set<ConfigData*>::iterator nit;
	for (nit = next.begin(); nit != next.end(); ++nit)
		(*nit)->config();
}

void ConfigData::beforeConfig() {}
void ConfigData::doConfig() {}
void ConfigData::afterConfig() {}
