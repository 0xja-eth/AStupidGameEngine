#include "Entity.h"

#include "GameException.h"

#include "AssetsPool.h"

#include "Node.h"
/*
Entity::Entity(string name, string modelName) :
	Entity(name, (Model*)AssetsPool::Model->get(modelName)){}

Entity::Entity(string name, string filePath, string modelName) :
	Entity(name, (Model*)AssetsPool::Model->load(filePath, modelName)) {}

Entity::Entity(string name, string pathRoot, string fileName, string modelName) :
	Entity(name, (Model*)AssetsPool::Model->load(pathRoot, fileName, modelName)) {}
*/
Entity::Entity(string name, Model* model) : RenderableComponent(name) {
	setModel(model);
};

Entity* Entity::copy() {
	Entity* comp = new Entity(getName(), this->model);
	comp->effects = effects;
	return (Entity*)Component::copy(comp);
}

RenderData* Entity::getRenderData() {
	glm::mat4 curModel = getTransform()->curModel();
	ModelRenderData* renderData = model->getRenderData();
	renderData->setParams(shadingShader(), curModel);
	return renderData;
}

void Entity::setModel(Model* model) {
	useAsset(this->model = model);
}

Model* Entity::getModel() const { return model; }

bool Entity::testCapture(Camera* camera) {
	return true;
}
