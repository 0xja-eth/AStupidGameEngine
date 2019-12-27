#include "Component.h"
#include "Node.h"

Component::~Component() {
	for (int i = 0; i < usingAssets.size(); i++)
		usingAssets[i]->unuse(this);
	usingAssets.clear();
}

Component* Component::copy(Component* comp) {
	// ¿½±´×ÊÔ´
	for (int i = 0; i < usingAssets.size(); ++i)
		comp->useAsset(usingAssets[i]);
	return comp;
}

bool Component::isEnabled() const { return isActived();}

bool Component::isActived() const { return actived; }
void Component::activate() { actived = true; }
void Component::deactivate() { actived = false; }

/*
void Component::useAsset(AssetType type, string name) {
	useAsset(AssetsManager::getAsset(type, name));
}
*/
void Component::useAsset(Asset* asset) {
	if (asset == NULL) return;
	asset = asset->use(this);
	if (asset == NULL) return;
	usingAssets.push_back(asset);
}

Asset* Component::getAsset(string type, string name) {
	for (int i = 0; i < usingAssets.size(); ++i) {
		Asset* asset = usingAssets[i];
		if (asset->aType() == type && asset->getName() == name)
			return asset;
	}
	return NULL;
}

void Component::unuseAsset(string type, string name) {
	unuseAsset(getAssetIter(type, name));
}

void Component::unuseAsset(Asset* asset) {
	unuseAsset(getAssetIter(asset));
}

vector<Asset*>::iterator Component::getAssetIter(string type, string name) {
	vector<Asset*>::iterator ait;
	for (ait = usingAssets.begin(); ait < usingAssets.end(); ++ait)
		if ((*ait)->aType() == type && (*ait)->getName() == name) break;
	return ait;
}
vector<Asset*>::iterator Component::getAssetIter(Asset* asset) {
	return find(usingAssets.begin(), usingAssets.end(), asset);
}
void Component::unuseAsset(vector<Asset*>::iterator asset) {
	if (*asset == NULL) return;
	(*asset)->unuse(this);
	usingAssets.erase(asset);
}

string Component::getName() { return name; }

void Component::setNode(Node* node) { this->node = node;}

Node* Component::getNode() { return node; }
Node* Component::getParentNode() { return node->getParent(); }

string Component::getTag() { return node->getTag(); }
string Component::getNodeName() { return node->getName(); }

Transform* Component::getTransform() { return node->getTransform(); }
Transform* Component::getParentTransform() {
	Node* parent = getParentNode();
	return parent == NULL ? NULL : parent->getTransform(); 
}
/*
void Component::update() {
	if (!actived) return;
	beforeUpdate();
	doUpdate();
	afterUpdate();
}
*/
void Component::beforeUpdate() {}

void Component::doUpdate() {}

void Component::afterUpdate() {}
