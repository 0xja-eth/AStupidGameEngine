#include "Material.h"

#include "Shader.h"

void MaterialRenderData::setAttr(string attr, float val) {
	if (attr == "Ns") ns = val;
	if (attr == "Ni") ni = val;
	if (attr == "d") d = val;
}
void MaterialRenderData::setAttr(string attr, int val) {
	if (attr == "illum") illum = val;
}
void MaterialRenderData::setAttr(string attr, glm::vec3 val) {
	if (attr == "Ka") ka = val;
	if (attr == "Kd") kd = val;
	if (attr == "Ks") ks = val;
}
void MaterialRenderData::setAttr(string attr, Texture* val) {
	if (val == NULL) return;
	if (attr == "map_Kd") mapKd = val->getRenderData(Kd);
	if (attr == "map_Ks") mapKs = val->getRenderData(Ks);
	if (attr == "map_Bump") mapBump = val->getRenderData(Bump);
}

void MaterialRenderData::setup() {
	addNext(mapKd); addNext(mapKs); addNext(mapBump);
}

void MaterialRenderData::setParams(Shader* shader) {
	this->shader = shader;
}

void MaterialRenderData::beforeConfig() {
	if (mapKd != NULL) mapKd->setParams(shader);
	if (mapKs != NULL) mapKs->setParams(shader);
	if (mapBump != NULL) mapBump->setParams(shader);
}

void MaterialRenderData::doConfig() {
	string name;
	shader->activate();
	name = MaterialName + "." + MaterialAttrs[0];
	shader->setVec3(name, ka);
	name = MaterialName + "." + MaterialAttrs[1];
	shader->setVec3(name, kd);
	name = MaterialName + "." + MaterialAttrs[2];
	shader->setVec3(name, ks);
	name = MaterialName + "." + MaterialAttrs[4];
	shader->setFloat(name, ns);
	name = MaterialName + "." + MaterialAttrs[1] + TextureSuffix + ValidSuffix;
	shader->setBool(name, mapKd != NULL);
	name = MaterialName + "." + MaterialAttrs[2] + TextureSuffix + ValidSuffix;
	shader->setBool(name, mapKs != NULL);
	name = MaterialName + "." + MaterialAttrs[3] + TextureSuffix + ValidSuffix;
	shader->setBool(name, mapBump != NULL);
	/*
	if (mapKd != NULL) mapKd->render();
	if (mapKs != NULL) mapKs->render();
	if (mapBump != NULL) mapBump->render();
	*/
}

Material::Material(string name) : name(name) {};

Material::~Material() {
	delete mapKd, mapKs, mapBump;
}

string Material::getName() { return name; }

MaterialRenderData* Material::getRenderData() {
	return &renderData;
}

void Material::setAttr(string attr, float val) {
	if (attr == "Ns") ns = val;
	if (attr == "Ni") ni = val;
	if (attr == "d") d = val;
	renderData.setAttr(attr, val);
}
void Material::setAttr(string attr, int val) {
	if (attr == "illum") illum = val;
	renderData.setAttr(attr, val);
}
void Material::setAttr(string attr, glm::vec3 val) {
	if (attr == "Ka") ka = val;
	if (attr == "Kd") kd = val;
	if (attr == "Ks") ks = val;
	renderData.setAttr(attr, val);
}
void Material::setAttr(string attr, Texture* val) {
	if (attr == "map_Kd") mapKd = val;
	if (attr == "map_Bump") mapBump = val;
	if (attr == "map_Ks") mapKs = val;
	renderData.setAttr(attr, val);
}

void Material::setup() {
	if (mapKd != NULL) mapKd->setup();
	if (mapKs != NULL) mapKs->setup();
	if (mapBump != NULL) mapBump->setup();
	renderData.setup();
}

void Material::setAsset(MaterialGroup* asset) {
	this->asset = asset;
}

/*
void MaterialData::config(ShaderAsset *shader) {
	string name;
	name = MaterialName + "." + MaterialAmbientAttrName;
	shader->setVec3(name, ka);
	name = MaterialName + "." + MaterialDiffuseAttrName;
	shader->setVec3(name, kd);
	name = MaterialName + "." + MaterialSpecularAttrName;
	shader->setVec3(name, ks);
	name = MaterialName + "." + MaterialShininessAttrName;
	shader->setFloat(name, ns);

	if (mapKd != NULL) mapKd->config(shader, 1);
	if (mapKs != NULL) mapKs->config(shader, 2);
	if (mapBump != NULL) mapBump->config(shader, 3);
}
*/
MaterialGroup::MaterialGroup(string name, string pathRoot, string fileName) :
	Asset(name, pathRoot, fileName) {}

MaterialGroup::~MaterialGroup() {
	for (int i = 0; i < data.size(); i++) delete data[i];
	data.clear();
}

void MaterialGroup::setup() {
	Asset::setup();
	for (int i = 0; i < data.size(); i++) data[i]->setup();
}

void MaterialGroup::addData(Material* data) {
	this->data.push_back(data);
	data->setAsset(this);
}

Material* MaterialGroup::getData(string name) {
	for (int i = 0; i < data.size(); i++)
		if (data[i]->getName() == name) return data[i];
	return NULL;
}