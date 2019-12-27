#pragma once

#include "Updatable.h"

#include "AssetsPool.h"

using namespace std;

class Node;
class Transform;

/*
 *	所有组件基类
 */
class Component : public Updatable {
public:
	Component(string name = "") : name(name) {};
	virtual ~Component();

	virtual string cType() = 0;

	virtual bool isConfigurable() { return false; };
	virtual bool isRenderable() { return false; };

	virtual Component* copy(Component* comp);
	virtual Component* copy() = 0;

	virtual bool isEnabled() const;

	virtual bool isActived() const;
	void activate();
	void deactivate();

	void useAsset(Asset* asset);
	Asset* getAsset(string type, string name);
	void unuseAsset(string type, string name);
	void unuseAsset(Asset* asset);

	string getName();

	void setNode(Node* node);

	Node* getNode();
	Node* getParentNode();

	string getTag();
	string getNodeName();

	Transform* getTransform();
	Transform* getParentTransform();

private:
	bool actived = true;

protected:
	string name;

	Node* node;

	vector<Asset*> usingAssets;

	vector<Asset*>::iterator getAssetIter(string type, string name);
	vector<Asset*>::iterator getAssetIter(Asset* asset);

	void unuseAsset(vector<Asset*>::iterator asset);

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();
};

