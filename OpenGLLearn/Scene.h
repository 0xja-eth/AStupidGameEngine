#pragma once

class Scene;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Config.h"

#include "AssetsPool.h"

#include "Node.h"

using namespace std;

class RenderSystem;
class PhysicsSystem;

#define DEFAULT_BACKGROUND_COLOR glm::vec4(0.2,0.15,0.2,0.1)

/*
 *	游戏场景类
 */
class Scene : public Updatable {
public:

	Scene(glm::vec4 backgroundColor = DEFAULT_BACKGROUND_COLOR) :
		backgroundColor(backgroundColor) {};
	~Scene();

	void start();
	void play();
	void pause();
	void terminate();

	virtual bool isEnabled() const;

	void clearScreen();

	Node* getRoot();

	Node* createNode(string name, Node* parent = NULL,
		glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), 
		glm::vec3 sca = glm::vec3(1), bool animatable = false);

	// 附加或者移动结点
	Node* addNode(Node* node, Node* parent = NULL);
	void removeNode(Node* node);

	void setActivedCamera(Camera* camera);
	Camera* getActivedCamera();

	RenderSystem* getRenderSystem();
	PhysicsSystem* getPhysicsSystem();

	SystemBase* getSystem(string name);

private:
	bool actived = true;
	bool initialized = false;

	void createObjects();

	void createRoot();

	void beforeUpdateSystems();
	void doUpdateSystems();
	void afterUpdateSystems();

protected:

	glm::vec4 backgroundColor;

	Node *root, *cameraNode;

	Camera *activedCamera;

	vector<Node*> removedNodes;

	vector<ConfigurableComponent*> globalConfigComps;

	vector<SystemBase*> systems;

	RenderSystem* renderSystem;
	PhysicsSystem* physicsSystem;

	void addSystem(SystemBase* manager);

	virtual void createSystems();

	virtual void createAssets();
	virtual void createNodes();
	virtual void createComponents();
	virtual void createOthers();

	void setGlobalShader(Shader* shader);

	void addGlobalConfigComp(ConfigurableComponent* comp);
	bool hasGlobalConfigComp(ConfigurableComponent* comp);

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();

	virtual void updateInput();

	virtual void updateOthers();

	void clearNodes();
};
