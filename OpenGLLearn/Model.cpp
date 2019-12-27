#include "Model.h"



ModelRenderData::~ModelRenderData() {
	clearShaderModels();
}

void ModelRenderData::setup() {
	glGenBuffers(1, &MBO);
	setupMeshesMBO();
}

void ModelRenderData::setParams(Shader* shader, glm::mat4 model) {
	Models* models = shaderModels[shader];
	if (models == NULL) shaderModels[shader] = models = new Models;
	models->push_back(model);
}

void ModelRenderData::addMesh(Mesh* data) {
	addNext(data->getRenderData());
}

void ModelRenderData::clearShaderModels() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit)
		delete smit->second;
	shaderModels.clear();
}

void ModelRenderData::setupMeshesMBO() {
	set<ConfigData*>::iterator nit;
	glBindBuffer(GL_ARRAY_BUFFER, MBO);
	for (nit = next.begin(); nit != next.end(); ++nit) {
		MeshRenderData* meshData = (MeshRenderData*)*nit;
		meshData->setupMBOAttrs();
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelRenderData::configMBO(Models* models) {
	if (models == NULL) return;
	glBindBuffer(GL_ARRAY_BUFFER, MBO);
	glBufferData(GL_ARRAY_BUFFER, models->size() * sizeof(glm::mat4),
		&(*models)[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelRenderData::renderChildren() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit) {
		Shader* shader = smit->first;
		Models* models = smit->second;
		configMBO(models);

		set<ConfigData*>::iterator nit;
		for (nit = next.begin(); nit != next.end(); ++nit) {
			MeshRenderData* mesh = (MeshRenderData*)*nit;
			mesh->setParams(shader, models->size());
			mesh->render();
		}
	}
}

void ModelRenderData::afterRender() {
	SMIter smit = shaderModels.begin();
	for (; smit != shaderModels.end(); ++smit)
		if (smit->second != NULL)
			smit->second->clear();
}

Model::Model(string name, string pathRoot, string fileName) :
	Asset(name, pathRoot, fileName) {
	//renderData.setup();
}

Model::~Model() {
	for (int i = 0; i < data.size(); i++) delete data[i];
	data.clear();
}

void Model::setup() {
	Asset::setup();
	for (int i = 0; i < data.size(); i++) data[i]->setup();
	renderData.setup();
}

Mesh* Model::getMesh(string name) {
	for (int i = 0; i < data.size(); i++)
		if (data[i]->getName() == name) return data[i];
	return NULL;
}
MaterialGroup* Model::getMaterial(string name) {
	for (int i = 0; i < materials.size(); i++)
		if (materials[i]->getName() == name) return materials[i];
	return NULL;
}
Material* Model::getMaterialData(string name) {
	Material* res = NULL;
	for (int i = 0; i < materials.size(); i++)
		if ((res = materials[i]->getData(name)) != NULL)
			return res;
	return NULL;
}
/*
vector<Vertex> Model::getVertices() {
	vector<Vertex> res;
	for (int i=0;i<data.size();++i)
		res.insert(res.end(), data[i]->getVertices().begin(), 
			data[i]->getVertices().end());
	return res;
}
*/

glm::vec3* Model::getVertices(int& cnt) {
	int tmp, index = 0; cnt = 0;
	for (int i = 0; i < data.size(); ++i)
		cnt += data[i]->getVerticesCount();
	glm::vec3* res = new glm::vec3[cnt];
	for (int i = 0; i < data.size(); ++i) {
		tmp = data[i]->getVerticesCount();
		for (int j = 0; j < tmp; ++j)
			res[index++] = data[i]->getVertex(j).v;
	}
	return res;
}

void Model::addMesh(Mesh* mesh) { 
	data.push_back(mesh); 
	renderData.addMesh(mesh);
}
void Model::addMaterial(MaterialGroup* material) { 
	materials.push_back(material); 
}

ModelRenderData* Model::getRenderData() {
	return &renderData;
}

//vector<MeshData*>& MeshAsset::getData() { return data; }
//vector<MaterialAsset*>& MeshAsset::getMaterials() { return materials; }

