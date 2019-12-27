#pragma once
#include "Scene.h"

class GameOverScene : public Scene {
private:
	Shader *uiShader;
	Sprite *backgroundSprite, *startSprite;
	
	Node* background, *startBtn;

protected:
	/*
	unsigned int btnVBO, btnVAO, btnEBO;
	unsigned int VBO, VAO, EBO;
	*/
	virtual void createAssets();
	virtual void createNodes();
	virtual void createComponents();
	virtual void createOthers();

	virtual void updateOthers();
};

