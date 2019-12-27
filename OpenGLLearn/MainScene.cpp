#include "MainScene.h"

#include "MainSceneSystem.h"

#include "Camera.h"
#include "Entity.h"
#include "Light.h"

#include "Skybox.h";

#include "BoundingBox.h"
#include "Rigidbody.h"

#include "SpaceShipControl.h"
#include "Rock.h"

MainScene::~MainScene() {
	delete manager;
}

void MainScene::createSystems() {
	Scene::createSystems();
	manager = new MainSceneSystem(this);
	addSystem(manager);
}

void MainScene::createAssets() {
	Scene::createAssets();

	// 读取 Shader
	commonShader = (Shader*)AssetsPool::Shader->load(
		"CommonShader", "./", "vShader.glsl", "fShader.glsl", "gShader.glsl", false);
	lightShader = (Shader*)AssetsPool::Shader->load(
		"LightShader", "./", "vShader.glsl", "lShader.glsl", false);
	skyShader = (Shader*)AssetsPool::Shader->load(
		"SkyboxShader", "./", "skyVShader.glsl", "skyFShader.glsl", false);
	bbShader = (Shader*)AssetsPool::Shader->load(
		"BoundingboxShader", "./", "bbVShader.glsl", "bbFShader.glsl", false);

	// 应用全局 Shader
	setGlobalShader(commonShader);
	setGlobalShader(lightShader);
	setGlobalShader(skyShader);
	setGlobalShader(bbShader);

	// 资源加载
	shipAsset = (Model*)AssetsPool::Model->load(
		"Ship", "./Ship/SciFi_Destroyer/", "SciFi_Destroyer.obj", false);
	//  "./Ship/Banshee/", "Banshee.obj", "Ship", false);
	//	"./rock/", "rock.obj", "Robot", false);
	//  "./nanosuit/", "nanosuit.obj", "Robot", false);
	//  "./", "test.obj", "Cube", false);
	enemyAsset = (Model*)AssetsPool::Model->load(
	//	"./Ship/SciFi_Destroyer/", "SciFi_Destroyer.obj", "Ship", false);
		"Enemy", "./Ship/Banshee/", "Banshee.obj", false);
	//	"./rock/", "rock.obj", "Robot", false);
	rockAsset = (Model*)AssetsPool::Model->load(
		"Rock", "./Rock/", "Rock.obj", false);
	planetAsset = (Model*)AssetsPool::Model->load(
		"Planet", "./Planet/", "planet.obj", false);

	skyBoxAsset = AssetsPool::Texture->load(
		"Skybox", "./Sky/", "", GL_TEXTURE_CUBE_MAP);
}

void MainScene::createNodes() {
	Scene::createNodes();
	// 结点创建
	//lightNode = createNode("MainLight", NULL, glm::vec3(0, 0, 0));

	shipControl = createNode("ShipControl", NULL,
		glm::vec3(0), glm::vec3(0), glm::vec3(1), true);
	ship = createNode("Ship", shipControl,
		glm::vec3(0, -0.5, 0), glm::vec3(0, 0, 0), glm::vec3(0.0002), true);
	//lightCube = createNode("LightCube", ship, glm::vec3(0), glm::vec3(0), glm::vec3(0.2));

	planet = createNode("Planet", NULL, 
		glm::vec3(128, 8, -548), glm::vec3(0), glm::vec3(50));

	addNode(cameraNode, shipControl);
	//addNode(lightNode, lightCube);
}

void MainScene::createComponents() {
	Scene::createComponents();

	// 创建组件
	Light* light = new Light("MainLight", 
		glm::vec3(0.9,0.8,0.6), glm::vec3(1,0.85,0.75), 
		glm::vec3(1,1,0.92), 0.6, 0.000000145, 0.0000000007);
	light->addShader(commonShader);
	planet->addComponent(light);

	Entity *planetEntity = new Entity("Planet", planetAsset);
	planetEntity->addShader(commonShader);
	planet->addComponent(planetEntity);

	Skybox* skybox = new Skybox("Sky", skyBoxAsset);
	skybox->addShader(skyShader);
	root->addComponent(skybox);
	activedCamera->setSkybox(skybox);

	Entity *shipEntity = new Entity("Ship", shipAsset);
	shipEntity->addShader(commonShader);
	ship->addComponent(shipEntity);

	BoundingBox* shipBox = new BoundingBox("ShipBox", shipEntity);
	shipBox->addShader(bbShader);
	ship->addComponent(shipBox);

	Rigidbody* shipRb = new Rigidbody("ShipRb", shipBox);
	shipControl->addComponent(shipRb);

	SpaceShipControl* control = new SpaceShipControl("Control", shipRb, ship);
	shipControl->addComponent(control);

	manager->setShipControl(control);
}

void MainScene::createOthers() {
}

void MainScene::createRock(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca) {
	Node* rockNode = createNode("Rock", NULL, pos, rot, sca);
	rockNode->setTag("Rock");
		
	// 创建组件
	Entity* rockEntity = new Entity("Rock", rockAsset);
	rockEntity->addShader(commonShader);
	rockNode->addComponent(rockEntity);
	
	BoundingBox* rockBox = new BoundingBox("RockBox", rockEntity);
	rockBox->addShader(bbShader);
	rockNode->addComponent(rockBox);

	Rigidbody* rockRb = new Rigidbody("RockRb", rockBox, false);
	rockNode->addComponent(rockRb);

	Rock* rock = new Rock("Rock", rockRb);
	rock->addShader(commonShader);
	rockNode->addComponent(rock);
}

void MainScene::createEnemy(glm::vec3 pos) {
	glm::vec3 rot = glm::vec3(0, -90, 0);
	glm::vec3 sca = glm::vec3(0.03);

	Node* enemyNode = createNode("Enemy", NULL, pos, rot, sca);
	enemyNode->setTag("Enemy");

	// 创建组件
	Entity* enemyEntity = new Entity("Enemy", enemyAsset);
	enemyEntity->addShader(commonShader);
	enemyNode->addComponent(enemyEntity);

	Light* light = new Light("EnemyLight", glm::vec3(0.5,0.45,0.6),
		glm::vec3(0.75,0.56,0.7), glm::vec3(0.85,0.6,0.8), 
		1, 0.0001, 0.00000033);
	enemyNode->addComponent(light);
	light->addShader(commonShader);

	BoundingBox* enemyBox = new BoundingBox("EnemyBox", enemyEntity);
	enemyBox->addShader(bbShader);
	enemyNode->addComponent(enemyBox);

	Rigidbody* enemyRb = new Rigidbody("EnemyRb", enemyBox, false);
	enemyNode->addComponent(enemyRb);
}

void MainScene::updateOthers() {

}