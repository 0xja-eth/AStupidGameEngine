#include "SceneManager.h"
#include "MainScene.h"
#include "GameOverScene.h"

SceneManager* SceneManager::self = new SceneManager;

void SceneManager::setup() {
	Scene* scene = new INIT_SCENE();
	doPushScene(scene);
}

Scene* SceneManager::currentScene() {
	return scenes.empty() ? NULL : scenes.top();
}

void SceneManager::pushScene(Scene* scene) {
	sceneReq.scene = scene;
	sceneReq.type = Push;
}

void SceneManager::gotoScene(Scene* scene) {
	sceneReq.scene = scene;
	sceneReq.type = Goto;
}

void SceneManager::changeScene(Scene* scene) {
	sceneReq.scene = scene;
	sceneReq.type = Change;
}

void SceneManager::clearScene(bool close /*= true*/) {
	sceneReq.close = close;
	sceneReq.type = Clear;
}

void SceneManager::popScene(bool close /*= true*/) {
	sceneReq.close = close;
	sceneReq.type = Pop;
}

void SceneManager::doPushScene(Scene* scene) {
	Scene* old = currentScene();
	if (old != NULL) old->terminate();
	scenes.push(scene);
	scene->start();
}

void SceneManager::doGotoScene(Scene* scene) {
	doClearScene(false);
	doPushScene(scene);
}

void SceneManager::doChangeScene(Scene* scene) {
	scenes.pop(); doPushScene(scene);
}

void SceneManager::doClearScene(bool close) {
	Scene* scene;
	while (!scenes.empty()) {
		scene = currentScene();
		delete scene; scenes.pop();
	}
	if(close) OpenGLAdapter::close();
}

void SceneManager::doPopScene(bool close) {
	Scene* scene = currentScene();
	if (scene == NULL) return;
	delete scene; scenes.pop();

	scene = currentScene();
	if (close && scene == NULL) OpenGLAdapter::close();
	scene->start();
}

void SceneManager::updateSceneRequest() {
	switch (sceneReq.type) {
	case Push: doPushScene(sceneReq.scene); break;
	case Change: doChangeScene(sceneReq.scene); break;
	case Goto: doGotoScene(sceneReq.scene); break;
	case Pop: doPopScene(sceneReq.close); break;
	case Clear: doClearScene(sceneReq.close); break;
	}
	clearSceneRequest();
}

void SceneManager::clearSceneRequest() {
	sceneReq.type = None;
}

void SceneManager::beforeUpdate() {
	Scene* scene = currentScene();
	if (scene != NULL) scene->callBeforeUpdate();
}
void SceneManager::doUpdate() {
	Scene* scene = currentScene();
	if (scene != NULL) scene->callDoUpdate();
}

void SceneManager::afterUpdate() {
	Scene* scene = currentScene();
	if (scene != NULL) scene->callAfterUpdate();
	updateSceneRequest();
}

