#pragma once

#include "GameException.h"

#include "Component.h"

using namespace std;

class Camera;
class Shader;
/*
 *	可配置 Shader 的组件
 */
class ConfigurableComponent: public Component {
public:
	ConfigurableComponent(string name) : Component(name) {}

	virtual bool isConfigurable() { return true; };
	virtual bool isRenderable() { return false; };

	virtual ConfigData* getConfigData();

	void capture(Camera* camera);

	virtual void addShader(Shader* shader);
	virtual void deleteShader(Shader* shader);
	bool hasShader(Shader* shader);

	void clearShaders();

protected:
	vector<Shader*> shaders;

	vector<Shader*>::iterator getShader(Shader* shader);

	virtual bool testCapture(Camera* camera);
	virtual void doCapture();
};

