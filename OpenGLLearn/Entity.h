#pragma once

#include "RenderableComponent.h"

#include "Config.h"

using namespace std;

class Camera;
class Model;

class Entity : public RenderableComponent {
public:

	//Entity(string name, string modelName);
	//Entity(string name, string filePath, string modelName);
	//Entity(string name, string pathRoot, string fileName, string modelName);
	Entity(string name, Model* model);

	virtual string cType() { return "Entity"; };

	virtual Entity* copy();

	virtual RenderData* getRenderData();

	void setModel(Model* model);
	Model* getModel() const;

private:
	Model* model;

	vector<RenderData*> effects;

protected:

	virtual bool testCapture(Camera* camera);
};
