#pragma once

#include <set>

#include "ManagerBase.h"

#include "RenderData.h"

#include "RenderableComponent.h"
//#include "Camera.h"
//#include "Scene.h"

class Scene;
class Camera;

using namespace std;
/*
 *	‰÷»æœµÕ≥
 */
class RenderSystem : public SystemBase {
public:
	RenderSystem(Scene* scene) : SystemBase(scene) {};

	static RenderSystem* Get();

	virtual string name() { return "RenderSystem"; };

	void captureObject(RenderData* data);
	void captureObject(ConfigData* data);

	void render(Camera* camera);

	virtual void debugShow();
private:
	float lastRenderTime;

	set<ConfigData*> configData;
	set<RenderData*> renderData;

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();

	void captureObjects(Camera* camera);
	void configObjects();
	void renderObjects();
};

