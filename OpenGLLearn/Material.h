#pragma once

#include "Texture.h"
#include "RenderData.h"

using namespace std;

const string MaterialName = "material";
const string MaterialAttrs[] = {
	"ambient","diffuse","specular","bump","shininess"
};
/*
const string MaterialAmbientAttrName = "ambient";
const string MaterialDiffuseAttrName = "diffuse";
const string MaterialSpecularAttrName = "specular";
const string MaterialShininessAttrName = "shininess";
*/
class MaterialGroup;

class MaterialRenderData : public RenderData {
public:
	void setAttr(string attr, float val);
	void setAttr(string attr, int val);
	void setAttr(string attr, glm::vec3 val);
	void setAttr(string attr, Texture* val);

	void setup();

	void setParams(Shader* shader);

private:
	Shader* shader;

	glm::vec3 ka, kd, ks;
	float ns, ni, d;
	int illum;

	TextureRenderData *mapKd = NULL, *mapKs = NULL, *mapBump = NULL;

protected:

	virtual void beforeConfig();
	virtual void doConfig();
};

class Material {
public:
	Material(string name);
	~Material();

	string getName();

	MaterialRenderData* getRenderData();

	void setAttr(string attr, float val);
	void setAttr(string attr, int val);
	void setAttr(string attr, glm::vec3 val);
	void setAttr(string attr, Texture* val);

	void setup();
	//void config(Shader *shader);

	void setAsset(MaterialGroup* asset);

private:
	string name;

	glm::vec3 ka, kd, ks;
	float ns, ni, d;
	int illum;

	Texture *mapKd = NULL, *mapKs = NULL, *mapBump = NULL;

	MaterialGroup* asset;

	MaterialRenderData renderData;
};

class MaterialGroup : public Asset {
public:
	MaterialGroup(string pathRoot, string fileName, string name);
	~MaterialGroup();

	virtual string aType() { return "MaterialGroup"; };

	void setup();

	void addData(Material* data);
	Material* getData(string name);

private:
	vector<Material*> data;

};