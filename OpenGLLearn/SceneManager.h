#pragma once

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <stack>

#include "ManagerBase.h"

#include "GameManager.h"
#include "Scene.h"

#define INIT_SCENE MainScene //GameOverScene

using namespace std;

enum SceneChangeType {
	None, Push, Goto, Change, Pop, Clear
};

/*
 *	场景变更请求
 */
struct SceneChangeRequest {
	Scene* scene = NULL;
	SceneChangeType type = None;
	bool close = false;
};

/*
 *	游戏场景管理类
 */
class SceneManager : public ManagerBase {
public:
	static SceneManager* Get() { 
		return self == NULL ? self = new SceneManager() : self;
	}

	virtual void setup();

	Scene* currentScene();
	void pushScene(Scene* scene);
	void gotoScene(Scene* scene);
	void changeScene(Scene* scene);
	void clearScene(bool close = true);
	void popScene(bool close = true);

	void clearSceneRequest();

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();
	
private:
	static SceneManager* self;

	stack<Scene*> scenes;

	SceneChangeRequest sceneReq;

	SceneManager() : ManagerBase() {};

	void doPushScene(Scene* scene);
	void doGotoScene(Scene* scene);
	void doChangeScene(Scene* scene);
	void doClearScene(bool close = true);
	void doPopScene(bool close = true);

	void updateSceneRequest();

};

