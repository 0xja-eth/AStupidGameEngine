#include "Sprite.h"

#include "GameManager.h"

#include "Mesh.h"
#include "Texture.h"

void SpriteRenderData::setup(Texture* texture, Mesh* mesh) {
	this->texture = texture->getRenderData();
	this->mesh = mesh->getRenderData();
	addNext(this->texture);
	addNext(this->mesh);

	glGenBuffers(1, &MBO);
	setupMeshesMBO();
}

void SpriteRenderData::setParams(Shader* shader, glm::mat4 model) {
	Models* models = shaderModels[shader];
	if (models == NULL) shaderModels[shader] = models = new Models;
	models->push_back(model);
}

void SpriteRenderData::clearShaderModels() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit)
		delete smit->second;
	shaderModels.clear();
}

void SpriteRenderData::setupMeshesMBO() {
	set<ConfigData*>::iterator nit;
	glBindBuffer(GL_ARRAY_BUFFER, MBO);
	mesh->setupMBOAttrs(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderData::configMBO(Models* models) {
	if (models == NULL) return;
	glBindBuffer(GL_ARRAY_BUFFER, MBO);
	glBufferData(GL_ARRAY_BUFFER, models->size() * sizeof(glm::mat4),
		&(*models)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderData::beforeRender() {
	glDisable(GL_DEPTH_TEST); // 关闭深度测试
}

void SpriteRenderData::renderChildren() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit) {
		Shader* shader = smit->first;
		Models* models = smit->second;
		configMBO(models);

		texture->setParams(shader, "img");
		texture->render();

		mesh->setParams(shader, models->size());
		mesh->render();
	}
}

void SpriteRenderData::afterRender() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit)
		if (smit->second != NULL)
			smit->second->clear();
	glEnable(GL_DEPTH_TEST); // 开启深度测试
}

Sprite::~Sprite() {
	delete mesh;
}

void Sprite::setup() {
	if (texture == NULL || mesh == NULL) return;
	Asset::setup(); mesh->setup();
	renderData.setup(texture, mesh);
}

float Sprite::getWidth() {
	return texture->getWidth();
}
float Sprite::getHeight() {
	return texture->getHeight();
}

float Sprite::getScreenWidth() {
	return 2 * (getWidth() / GameManager::ScreenWidth);
}
float Sprite::getScreenHeight() {
	return 2 * (getHeight() / GameManager::ScreenHeight);
}

glm::vec3 Sprite::getTargetSizeScale(float w, float h) {
	float sw = w / getScreenWidth();
	float sh = h / getScreenHeight();
	return glm::vec3(sw, sh, 1);
}

Texture* Sprite::getTexture() {
	return texture;
}

void Sprite::setTexture(Texture* texture) {
	this->texture = texture;
	texture->setup();
	generateMesh();
}

SpriteRenderData* Sprite::getRenderData() {
	return &renderData;
}

void Sprite::generateMesh() {
	if (mesh != NULL) delete mesh;
	float sw = getScreenWidth(), sh = getScreenHeight();
	mesh = Mesh::RectMesh("Sprite", 
		glm::vec2(-sw/2,-sh/2), glm::vec2(sw/2, sh/2),  // 原点在 0, 0
		glm::vec2(0, 1), glm::vec2(1, 0));
}
