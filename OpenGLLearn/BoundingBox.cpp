#include "BoundingBox.h"

#include "Entity.h"
#include "Camera.h"

void BoundingBoxRenderData::setup(glm::vec3 minP, glm::vec3 maxP) {
	mesh = Mesh::CuboidMesh("BoundingBox", minP, maxP);
	addNext(mesh->getRenderData());
}

void BoundingBoxRenderData::setup(Shader* shader, glm::mat4 model) {
	this->shader = shader;
	this->model = model;
}

void BoundingBoxRenderData::beforeConfig() {
	if (mesh != NULL)
		mesh->getRenderData()->setParams(shader);
	shader->activate();
	shader->setMat4("model", model);
}

void BoundingBoxRenderData::beforeRender() {
	glDisable(GL_CULL_FACE); // 关闭面剔除
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void BoundingBoxRenderData::afterRender() {
	glEnable(GL_CULL_FACE); // 开启面剔除
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

BoundingBox::BoundingBox(string name, Entity* entity, BoundingType type) :
	RenderableComponent(name), entity(entity), type(type) {}

BoundingBox::~BoundingBox() {}

BoundingBox* BoundingBox::copy() {
	BoundingBox* comp = new BoundingBox(getName(), entity, type);
	return (BoundingBox*)Component::copy(comp);
}

bool BoundingBox::check(glm::vec3 pos) {
	glm::mat4 model = getTransform()->curModel();
	glm::vec4 _pos = glm::vec4(pos.x, pos.y, pos.z, 1);
	glm::vec3 locPos = glm::inverse(model)*_pos;
	return checkLocal(locPos);
}

bool BoundingBox::check(BoundingBox* box) {
	if (box == NULL) return false;
	if (type == box->type && type == AABB) return check2AABB(box);
}

void BoundingBox::setShowBox(bool val) {
	showBox = val;
}

RenderData* BoundingBox::getRenderData() {
	renderData.setup(shadingShader(),
		getTransform()->curModel());
	return &renderData;
}

void BoundingBox::doUpdate() {
	updateShowBox();
	updateBoundingBox();
}

void BoundingBox::updateShowBox() {
	if (EventManager::Get()->isKeyPress(GLFW_KEY_X))
		showBox = !showBox;
}

void BoundingBox::updateBoundingBox() {
	if (entity->getModel() == lastModel) return;
	lastModel = entity->getModel();
	int count = 0;
	glm::vec3* vs = lastModel->getVertices(count);
	generateBoundingBox(vs, count);
}

void BoundingBox::generateBoundingBox(glm::vec3* vs, int count) {
	if (count <= 0) return;
	switch (type) {
	case Sphere: generateSphere(vs, count); break;
	case AABB: generateAABB(vs, count); break;
	case OBB: generateOBB(vs, count); break;
	}
	delete[] vs;
}

void BoundingBox::generateSphere(glm::vec3* vs, int count) {}

void BoundingBox::generateAABB(glm::vec3* vs, int count) {
	glm::vec3 minP = vs[0], maxP = vs[0];
	for (int i = 0; i < count; ++i) {
		glm::vec3 v = vs[i];
		minP.x = min(minP.x, v.x);
		minP.y = min(minP.y, v.y);
		minP.z = min(minP.z, v.z);
		maxP.x = max(maxP.x, v.x);
		maxP.y = max(maxP.y, v.y);
		maxP.z = max(maxP.z, v.z);
	}
	box[0] = minP; box[1] = maxP;
	renderData.setup(minP, maxP);
}

void BoundingBox::generateOBB(glm::vec3* vs, int count) {}

bool BoundingBox::checkLocal(glm::vec3 pos) {
	glm::vec3 minP = box[0], maxP = box[1];
	float x = pos.x, y = pos.y, z = pos.z;
	return minP.x < x && x < maxP.x &&
		minP.y < y && y < maxP.y &&
		minP.z < z && z < maxP.z;
}

bool BoundingBox::check2AABB(BoundingBox* box) {
	glm::mat4 model = box->getTransform()->curModel();
	glm::vec3* full = box->generateFullBox();
	bool res = false;
	for (int i = 0; !res&&i < 8; ++i) {
		glm::vec3 pos = full[i];
		glm::vec4 _pos = glm::vec4(pos.x, pos.y, pos.z, 1);
		glm::vec3 realPos = model * _pos;
		if (check(realPos)) res = true;
	}
	delete[] full;
	return res;
}

glm::vec3* BoundingBox::generateFullBox() {
	glm::vec3 minP = box[0], maxP = box[1];
	glm::vec3* res = new glm::vec3[8];
	res[0] = minP;
	res[1] = glm::vec3(maxP.x, minP.y, minP.z);
	res[2] = glm::vec3(minP.x, maxP.y, minP.z);
	res[3] = glm::vec3(minP.x, minP.y, maxP.z);
	res[4] = glm::vec3(maxP.x, maxP.y, minP.z);
	res[5] = glm::vec3(minP.x, maxP.y, maxP.z);
	res[6] = glm::vec3(maxP.x, minP.y, maxP.z);
	res[7] = maxP;
	return res;
}

bool BoundingBox::testCapture(Camera* camera) {
	return showBox;
}
