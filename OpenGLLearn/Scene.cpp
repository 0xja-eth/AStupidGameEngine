#include "Scene.h"

#include "RenderSystem.h"
#include "PhysicsSystem.h"

Scene::~Scene() {
	terminate();
}

#pragma region 初始化/创建物体

void Scene::start() {
	if (!initialized) {
		createObjects();
		initialized = true;
	} else play();
}

void Scene::play() {
	actived = true;
}

void Scene::pause() {
	clearScreen();
	actived = false;
}

void Scene::terminate() {
	pause(); clearNodes();
}

bool Scene::isEnabled() const
{
	return actived;
}

void Scene::createObjects() {
	createRoot();
	createSystems();
	createAssets(); createNodes();
	createComponents(); createOthers();
}

void Scene::createRoot() {
	root = new Node("root", NULL);
}

void Scene::addSystem(SystemBase* manager) {
	systems.push_back(manager);
}

void Scene::createSystems() {
	addSystem(physicsSystem = new PhysicsSystem(this));
	addSystem(renderSystem = new RenderSystem(this));
}

void Scene::createAssets() {}
void Scene::createNodes() {
	cameraNode = createNode("MainCamera", NULL,
		glm::vec3(0, 2, 10), glm::vec3(-5, 0, 0), glm::vec3(1));
}
void Scene::createComponents() {
	activedCamera = new Camera("MainCamera");
	cameraNode->addComponent(activedCamera);
	addGlobalConfigComp(activedCamera);
}
void Scene::createOthers() {}

#pragma endregion

#pragma region 基本操作

void Scene::beforeUpdate() {
	updateInput();
	beforeUpdateSystems();
	root->callBeforeUpdate();
}

void Scene::doUpdate() {
	clearScreen();
	doUpdateSystems();
	root->callDoUpdate();
	updateOthers();
}

void Scene::afterUpdate() {
	afterUpdateSystems();
	root->callAfterUpdate();
}

void Scene::setActivedCamera(Camera* camera) {
	activedCamera = camera;
}
Camera* Scene::getActivedCamera() {
	return activedCamera;
}

RenderSystem* Scene::getRenderSystem() {
	return renderSystem;
}
PhysicsSystem* Scene::getPhysicsSystem() {
	return physicsSystem;
}

SystemBase* Scene::getSystem(string name) {
	for (int i = 0; i < systems.size(); ++i)
		if (systems[i]->name() == name) return systems[i];
	return NULL;
}

void Scene::updateInput() {
	if (EventManager::Get()->isKeyPress(GLFW_KEY_ESCAPE))
		OpenGLAdapter::close();
}

void Scene::beforeUpdateSystems() {
	for (int i=0; i < systems.size(); ++i)
		systems[i]->callBeforeUpdate();
}
void Scene::doUpdateSystems() {
	for (int i = 0; i < systems.size(); ++i)
		systems[i]->callDoUpdate();
}
void Scene::afterUpdateSystems() {
	for (int i = 0; i < systems.size(); ++i)
		systems[i]->callAfterUpdate();
}

void Scene::updateOthers() {}

void Scene::clearScreen() {
	OpenGLAdapter::clearScreen(backgroundColor);
}

Node* Scene::getRoot() { return root; }

#pragma endregion

#pragma region 节点操作

void Scene::clearNodes() {
	delete root; 
	root = NULL;
	activedCamera = NULL;
	for (int i = 0; i < removedNodes.size(); i++)
		delete removedNodes[i];
	removedNodes.clear();
	globalConfigComps.clear();
}

Node* Scene::createNode(string name, Node* parent, glm::vec3 pos,
	glm::vec3 rot, glm::vec3 sca, bool animatable) {
	return addNode(new Node(name, pos, rot, sca, animatable), parent);
}
Node* Scene::addNode(Node* node, Node* parent) {
	if (parent == NULL) parent = root;
	node->removeParent();
	parent->addChild(node); 
	return node;
}
void Scene::removeNode(Node* node) {
	Node* parent = node->getParent();
	if(parent != NULL)
		parent->removeChild(node);
	removedNodes.push_back(node);
}

void Scene::addGlobalConfigComp(ConfigurableComponent* comp) {
	if(!hasGlobalConfigComp(comp)) globalConfigComps.push_back(comp);
}

bool Scene::hasGlobalConfigComp(ConfigurableComponent* comp) {
	return find(globalConfigComps.begin(), globalConfigComps.end(), comp) != globalConfigComps.end();
}

#pragma endregion

void Scene::setGlobalShader(Shader* shader) {
	for (int i = 0; i < globalConfigComps.size(); i++)
		globalConfigComps[i]->addShader(shader);
}
