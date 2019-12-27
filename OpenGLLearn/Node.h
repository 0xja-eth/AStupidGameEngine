#pragma once

class Node;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "AnimatableAttr.h"

#include "RenderableComponent.h"
//#include "ConfigurableComponent.h"

#include "Camera.h"

#include "Transform.h"

using namespace std;

/*
 *	游戏结点类，场景图的结点
 */
class Node : public Updatable {
public:
	Node(string name = "", glm::vec3 pos = glm::vec3(0),
		glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1), bool animatable = false);
	Node(string name = "", Transform* transform = NULL);
	~Node();

	Node* copy(bool deep = false);

	string getName();

	Node* getParent();
	Transform* getTransform();

	string getTag();
	void setTag(string tag);

	bool isVisible() const;
	void setVisible(bool val);

	virtual bool isEnabled() const;

	bool isActive() const;
	void setActive(bool val);

	void destroy();

	void addChild(Node* node);
	void removeChild(Node* node);
	bool hasChild(Node* node);

	void removeParent();

	vector<Node*> getChildren();

	Component* addComponent(Component* comp);
	void removeComponent(Component* comp);
	void removeComponentByType(string type);
	void removeComponentByName(string name);
	void removeComponentsByName(string name);

	Component* getComponentByType(string type);
	Component* getComponentByName(string name);
	vector<Component*> getComponentsByName(string name);
	vector<Component*> getComponents();

	void capture(Camera* camera);
	//void config(); void render();

private:
	bool visible = true, active = true;
	bool needDestroy = false;

	string name;
	string tag;

	Node* parent = NULL;
	vector<Node*> children;

	Transform* transform;
	vector<Component*> components;

	static glm::vec3 rotateAniFunc(float time, glm::vec3 cur, glm::vec3 tar, float rate = 0.15);

	void removeComponent(vector<Component*>::iterator cit);

protected:
	vector<Node*>::iterator getChild(Node* node);

	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();

	virtual void updateChildrenDestroy();

	virtual void captureChildren(Camera* camera);
	virtual void captureComponents(Camera* camera);

	//virtual void configComponents();
	//virtual void configChildren();

	//virtual void renderComponents();
	//virtual void renderChildren();
};


