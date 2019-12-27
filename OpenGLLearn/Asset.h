#pragma once
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Component;

using namespace std;

enum AssetStatus {
	Undefined, Defined, Using, Released
};

/*
 *	资源基类
 */
class Asset {
public:
	Asset(string name, string pathRoot, string fileName) :
		pathRoot(pathRoot), fileName(fileName), name(name) {};
	~Asset();

	virtual string aType() = 0;

	string getPathRoot();
	string getFileName();
	string getFilePath();

	string getName();
	vector<Component*> getUsingComponents();
	AssetStatus getStatus();

	virtual void setup(); // 定义资源（使用的时候自动定义）

	virtual Asset* use(Component* comp); // 使用资源（只能组件使用）
	virtual bool isUsing(Component* comp); // 资源是否在使用（只能组件使用）
	virtual void unuse(Component* comp); // 解除资源使用
	virtual bool release(bool force=false); // 释放资源

protected:
	string pathRoot, fileName, name;
	vector<Component*> usingComps; // 使用中的组件

	AssetStatus status = Undefined; // 资源状态

	vector<Component*>::iterator getUsing(Component* comp);
};

//#include "Component.h"
