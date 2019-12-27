#include "Skybox.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"

SkyboxRenderData::~SkyboxRenderData() {
	delete mesh;
}

void SkyboxRenderData::setup(Texture* texture) {
	this->texture = texture->getRenderData();
	addNext(this->texture);
	generateMesh();
}

void SkyboxRenderData::setup(Shader* shader) {
	this->shader = shader;
}

void SkyboxRenderData::generateMesh() {
	if (mesh != NULL) delete mesh;
	mesh = Mesh::CubeMesh("Skybox", 1, true);
	addNext(mesh->getRenderData());
}

void SkyboxRenderData::beforeConfig() {
	mesh->getRenderData()->setParams(shader);
	texture->setParams(shader, "skybox");
}

void SkyboxRenderData::beforeRender() {
	glDepthFunc(GL_LEQUAL);
}
void SkyboxRenderData::dafterRender() {
	glDepthFunc(GL_LESS);
}
/*
Skybox::Skybox(string name, string path) : 
	Skybox(name, AssetsPool::Texture->load(
		path, path ,"", GL_TEXTURE_CUBE_MAP)) {}
*/
Skybox::Skybox(string name, Texture* texture) : texture(texture){
	useAsset(texture);
	renderData.setup(texture);
}

Skybox* Skybox::copy() {
	Skybox* comp = new Skybox(getName(), texture);
	return (Skybox*)Component::copy(comp);
}

RenderData* Skybox::getRenderData() {
	renderData.setup(shadingShader());
	return &renderData;
}
bool Skybox::testCapture(Camera* camera) {
	return this == camera->getSkybox();
}