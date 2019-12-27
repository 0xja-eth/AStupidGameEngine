#include "GameOverScene.h"

#include "MainScene.h"

#include "Image.h"
/*
float bgVertices[] = {
	// positions             // texture coords
	 1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right
	 1.0f, -1.0f, 0.0f,      1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,      0.0f, 0.0f, // bottom left
	-1.0f,  1.0f, 0.0f,      0.0f, 1.0f  // top left 
};

float startGame[] = {
	0.3f, 0.3f, 0.0f,	1.0f, 1.0f,
	0.3f, -0.3f, 0.0f,	1.0f, 0.0f,
	-0.3f, -0.3f, 0.0f, 0.0f, 0.0f,
	-0.3f, 0.3f, 0.0f,	0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
*/
void GameOverScene::createAssets() {
	Scene::createAssets();

	uiShader = AssetsPool::Shader->load(
		"UIShader", "./", "uiVShader.glsl", "uiFShader.glsl", false);

	backgroundSprite = (Sprite*)AssetsPool::Sprite->load(
		"Background", "image/", "timg.jpg", false);
	startSprite = (Sprite*)AssetsPool::Sprite->load(
		"Start", "image/", "gameover.png", false);
}

void GameOverScene::createNodes() {
	Scene::createNodes();

	background = createNode("Background");
	startBtn = createNode("StartBtn", background, glm::vec3(0,0,0.5));
}

void GameOverScene::createComponents() {
	Scene::createComponents();

	Image* bgImg = new Image("Background", backgroundSprite);
	bgImg->addShader(uiShader);
	background->addComponent(bgImg);

	bgImg->setTargetSize(2, 2);

	Image* startImg = new Image("Start", startSprite);
	startImg->addShader(uiShader);
	startBtn->addComponent(startImg);

	startImg->setTargetSize(1, 1);
}

void GameOverScene::createOthers() {
	/* 原来的代码
	//背景
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//开始游戏
	glGenVertexArrays(1, &btnVAO);
	glGenBuffers(1, &btnVBO);
	glGenBuffers(1, &btnEBO);

	glBindVertexArray(btnVAO);

	glBindBuffer(GL_ARRAY_BUFFER, btnVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(startGame), startGame, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, btnEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	background = (Texture*)AssetsPool::Texture->load("image/", "timg.jpg", "1", false);
	startT = (Texture*)AssetsPool::Texture->load("image/", "gameover.png", "2", false);
	background->setup();
	startT->setup();

	bgShader = (Shader*)AssetsPool::Shader->load(
		"CommonShader", "./", "background.vs", "background.fs", false);
	startShader = (Shader*)AssetsPool::Shader->load(
		"LightShader", "./", "start.vs", "start.fs", false);

	bgShader->activate();
	bgShader->setInt("background", 0);
	startShader->activate();
	startShader->setInt("start", 0);
	*/
}

void GameOverScene::updateOthers() {
	// render container
	/*
	bgShader->activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, background->getTextureID());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	startShader->activate();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(3.2));
	startShader->setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, startT->getTextureID());

	glBindVertexArray(btnVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	*/
	if (EventManager::Get()->isKeyPress(GLFW_KEY_SPACE)) 
		SceneManager::Get()->gotoScene(new MainScene);
}