#include "RenderSystem.h"

#include "Scene.h"
#include "Camera.h"

void RenderSystem::beforeUpdate() {
	renderData.clear();
	configData.clear();
}

void RenderSystem::doUpdate() {
	render(scene->getActivedCamera());
}

void RenderSystem::afterUpdate() {
	OpenGLAdapter::swapBuffers();
}

void RenderSystem::render(Camera* camera) {
	float currentTime = glfwGetTime();
	captureObjects(camera); 
	configObjects(); renderObjects();
	lastRenderTime = glfwGetTime() - currentTime;
}

RenderSystem* RenderSystem::Get() {
	Scene* scene = SceneManager::Get()->currentScene();
	return scene->getRenderSystem();
}

void RenderSystem::captureObject(RenderData* data) {
	renderData.insert(data);
}

void RenderSystem::captureObject(ConfigData* data) {
	configData.insert(data);
}

void RenderSystem::debugShow() {
	cout << "Render time: " << lastRenderTime << "s" << endl;
}

void RenderSystem::captureObjects(Camera* camera) {
	Node* root = scene->getRoot();
	root->capture(camera);
}

void RenderSystem::configObjects() {
	set<ConfigData*>::iterator cit = configData.begin();
	for (; cit != configData.end(); ++cit) (*cit)->config();
}
void RenderSystem::renderObjects() {
	set<RenderData*>::iterator rit = renderData.begin();
	for (; rit != renderData.end(); ++rit) (*rit)->render();
}

/*
void RenderManager::renderObjects() {
	SMRCListIter sit = renderList.begin();
	for (; sit != renderList.end(); ++sit) {
		Shader* shader = sit->first;
		MRCList* mList = sit->second;

		MRCListIter mit = mList->begin();
		for (; mit != mList->end(); ++mit) {
			Model* mesh = mit->first;
			RCList* list = mit->second;

			if (mesh == NULL) renderNonMeshObjects(list);
			else renderMeshObjects(mesh, list);
		}
	}
}

void RenderManager::renderNonMeshObjects(RCList* list, Shader* shader) {
	for (int i=0;i<list->size();i++) (*list)[i]->render();
}

void RenderManager::renderMeshObjects(Model* mesh, RCList* list, Shader* shader) {
	int count = list->size();
	map<Shader*, RCList> shaderList;

	glm::mat4* modelMatrices = new glm::mat4[count];
	for (int i = 0; i < list->size(); i++)
		modelMatrices[i] = (*list)[i]->getTransform()->curModel();

	renderRenderData(mesh->getRenderData(count, modelMatrices));
}

void RenderManager::renderRenderData(RenderData* renderData) {
	for (int i = 0; i < renderData->subData.size(); i++) {
		SubRenderData* subData = renderData->subData[i];

		renderMaterialData(subData->materialData, renderData->shader);

		glBindVertexArray(subData->VAO);
		glDrawElementsInstanced(GL_TRIANGLES, subData->indices.size(), 
			GL_UNSIGNED_INT, 0, renderData->count);
		glBindVertexArray(0);
	}
}

void RenderManager::renderMaterialData(MaterialConfigData* materialData, Shader* shader) {
	string name;
	name = MaterialName + "." + MaterialAmbientAttrName;
	shader->setVec3(name, materialData->ka);
	name = MaterialName + "." + MaterialDiffuseAttrName;
	shader->setVec3(name, materialData->kd);
	name = MaterialName + "." + MaterialSpecularAttrName;
	shader->setVec3(name, materialData->ks);
	name = MaterialName + "." + MaterialShininessAttrName;
	shader->setFloat(name, materialData->ns);

	if (materialData->mapKd != NULL) materialData->mapKd->config(shader, 1);
	if (materialData->mapKs != NULL) materialData->mapKs->config(shader, 2);
	if (materialData->mapBump != NULL) materialData->mapBump->config(shader, 3);
}
*/
