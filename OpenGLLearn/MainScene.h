#pragma once

#include "Scene.h"

class MainSceneSystem;

/*
 *	ÓÎÏ·Ö÷³¡¾°
 */
class MainScene : public Scene {
public:
	~MainScene();

	void createRock(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca);
	void createEnemy(glm::vec3 pos);

private:
	Shader *commonShader, *lightShader, *skyShader, *bbShader;
	Model *shipAsset, *enemyAsset, *rockAsset, *planetAsset;
	Texture *skyBoxAsset;

	Node *ship, *shipControl, *planet;// , *lightCube, *lightNode, *sky;

protected:
	MainSceneSystem* manager;

	virtual void createSystems();

	virtual void createAssets();
	virtual void createNodes();
	virtual void createComponents();
	virtual void createOthers();

	virtual void updateOthers();
};