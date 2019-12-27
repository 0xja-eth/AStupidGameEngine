#include "Node.h"

Node::Node(string name /*= ""*/, glm::vec3 pos /*= glm::vec3(0)*/,
	glm::vec3 rot /*= glm::vec3(0)*/, glm::vec3 sca /*= glm::vec3(1)*/, bool animatable) :
	Node(name, animatable ? new AnimatableTransform(name, pos, rot, sca) : 
		new Transform(name, pos, rot, sca)) {}

Node::Node(string name /*= ""*/, Transform* transform /*= NULL*/) : name(name) {
	if (transform == NULL) transform = new Transform(name);
	addComponent((Component*)transform);
	this->transform = transform;
}

Node::~Node() {
	removeParent();
	for (int i = 0; i < components.size(); i++)
		delete components[i];
	for (int i = 0; i < children.size(); i++)
		delete children[i];
	transform = NULL;
	components.clear();
	children.clear();
}

Node* Node::copy(bool deep) {
	Node* node = new Node(name, transform->copy());
	if (deep) // 递归拷贝子结点
		for (int i = 0; i < children.size(); i++) {
			Node* child = children[i]->copy(deep);
			node->addChild(child);
		}
	// 拷贝组件
	for(int i=0;i<components.size();++i)
		if(components[i]->cType()!="Transform")
			node->addComponent(components[i]->copy());
	return node;
}

string Node::getName() { return name; }

Transform* Node::getTransform() { return transform; }

string Node::getTag() { return tag; }
void Node::setTag(string tag) { this->tag = tag; }

Node* Node::getParent() { return parent; }

bool Node::isVisible() const { return visible; }
void Node::setVisible(bool val) { visible = val; }

bool Node::isEnabled() const { return isActive();}

bool Node::isActive() const { return active; }
void Node::setActive(bool val) { active = val; }

std::vector<Node*> Node::getChildren() { return children; }

void Node::destroy() { needDestroy = true; }

void Node::removeParent() {
	if (parent == NULL) return;
	parent->removeChild(this);
}

void Node::addChild(Node* node) {
	if (node->parent != NULL) return; // 如果已经有父亲，则无法添加
	children.push_back(node);
	node->parent = this;
}

void Node::removeChild(Node* node) {
	vector<Node*>::iterator child = getChild(node);
	if (child == children.end()) return;
	(*child)->parent = NULL;
	children.erase(child);
}

bool Node::hasChild(Node* node) {
	return getChild(node) != children.end();
}

vector<Node*>::iterator Node::getChild(Node* node) {
	return find(children.begin(), children.end(), node);
}

#pragma region 组件控制

Component* Node::addComponent(Component* comp) {
	if (getComponentByType(comp->cType()) == NULL) {
		components.push_back(comp);
		comp->setNode(this);
		return comp;
	}
	return NULL;
}

void Node::removeComponent(Component* comp) {
	if (comp->cType() == "Transform") return;
	removeComponent(find(components.begin(), components.end(), comp));
	comp->setNode(NULL);
}
void Node::removeComponent(vector<Component*>::iterator cit) {
	if (cit == components.end()) return;
	if ((*cit)->cType() == "Transform") return;
	components.erase(cit);
}

void Node::removeComponentByType(string type) {
	if (type == "Transform") return;
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->cType() == type) removeComponent(cit);
}
void Node::removeComponentByName(string name) {
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->getName() == name) {
			removeComponent(cit); break;
		}
}
void Node::removeComponentsByName(string name) {
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->getName() == name) removeComponent(cit);
}

Component* Node::getComponentByType(string type) {
	if (type == "Transform") return transform;
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->cType() == type) return (*cit);
	return NULL;
}
Component* Node::getComponentByName(string name) {
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->getName() == name) return (*cit);
	return NULL;
}
vector<Component*> Node::getComponentsByName(string name) {
	vector<Component*> res;
	vector<Component*>::iterator cit;
	for (cit = components.begin(); cit < components.end(); ++cit)
		if ((*cit)->getName() == name) res.push_back(*cit);
	return res;
}
vector<Component*> Node::getComponents() { return components; }

#pragma endregion

void Node::beforeUpdate() {
	for (int i = 0; i < components.size(); i++)
		components[i]->callBeforeUpdate();
	for (int i = 0; i < children.size(); i++)
		children[i]->callBeforeUpdate();
}

void Node::doUpdate() {
	for (int i = 0; i < components.size(); i++)
		components[i]->callDoUpdate();
	for (int i = 0; i < children.size(); i++)
		children[i]->callDoUpdate();
}

void Node::afterUpdate() {
	for (int i = 0; i < components.size(); i++)
		components[i]->callAfterUpdate();
	for (int i = 0; i < children.size(); i++)
		children[i]->callAfterUpdate();
	updateChildrenDestroy();
}

void Node::capture(Camera* camera) {
	if (!visible) return;
	captureComponents(camera);
	captureChildren(camera);
}

/*
void Node::config() {
	if (!visible) return;
	configComponents();
	configChildren();
}
void Node::render() {
	if (!visible) return;
	renderComponents();
	renderChildren();
}
*/

void Node::updateChildrenDestroy() {
	for (int i = children.size()-1; i >= 0; --i)
		if (children[i]->needDestroy) delete children[i];
}

void Node::captureChildren(Camera* camera) {
	for (int i = 0; i < children.size(); i++)
		children[i]->capture(camera);
}

void Node::captureComponents(Camera* camera) {
	for (int i = 0; i < components.size(); i++) 
		if (components[i]->isConfigurable()) 
			((ConfigurableComponent*)components[i])->capture(camera);
}

/*
void Node::configChildren() {
	for (int i = 0; i < children.size(); i++)
		children[i]->config();
}
void Node::renderChildren() {
	for (int i = 0; i < children.size(); i++)
		children[i]->render();
}
*/
/*
void Node::configComponents() {
	for (int i = 0; i < components.size(); i++) {
		Component* comp = components[i];
		if (comp->isConfigurable())
			((ConfigurableComponent*)comp)->config();
	}
}
void Node::renderComponents() {
	for (int i = 0; i < components.size(); i++) {
		Component* comp = components[i];
		if (comp->isRenderable())
			((RenderableComponent*)comp)->render();
	}
}
*/
glm::vec3 Node::rotateAniFunc(float time, glm::vec3 cur, glm::vec3 tar, float rate /*= ROTATION_RATE*/) {
	// 使用四元数进行旋转
	return glm::vec3(0);
}
