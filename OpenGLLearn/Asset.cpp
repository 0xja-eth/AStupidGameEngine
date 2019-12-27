#include "Asset.h"

Asset::~Asset() { release(true); }

string Asset::getPathRoot() { return pathRoot; }
string Asset::getFileName() { return fileName; }
string Asset::getFilePath() { return pathRoot + fileName; }
string Asset::getName() { return name; }
vector<Component*> Asset::getUsingComponents() { return usingComps; }
AssetStatus Asset::getStatus() { return status; }

void Asset::setup() {
	if (status == Released) return;
	status = Defined;
}

Asset* Asset::use(Component* comp) {
	if (status == Released) return NULL;
	if (status == Undefined) setup();
	status = Using;
	if (!isUsing(comp))
		usingComps.push_back(comp);
	return this;
}

bool Asset::isUsing(Component* comp) {
	if (status == Released) return false;
	return getUsing(comp) != usingComps.end();
}

void Asset::unuse(Component* comp) {
	if (status == Released) return;
	vector<Component*>::iterator uit = getUsing(comp);
	usingComps.erase(uit);
	if (usingComps.size() <= 0) status = Defined;
}

bool Asset::release(bool force/*=false*/) {
	if (status == Released) return true;
	if (status == Using && !force) return false;
	usingComps.clear();
	status = Released;
	return true;
}

vector<Component*>::iterator Asset::getUsing(Component* comp) {
	return find(usingComps.begin(), usingComps.end(), comp);
}

