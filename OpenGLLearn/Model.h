#pragma once
#include <map>

#include "Material.h"
#include "Config.h"
#include "Mesh.h"

/*
struct RenderData {
	int count = 0;
	GLuint MBO = 0; // ModelBufferObject
	glm::mat4* modelMatrices;
	vector<SubRenderData*> subData;

	Shader* shader;
};
*/
typedef vector<glm::mat4> Models;
typedef map<Shader*, Models*> SModels;
typedef SModels::iterator SMIter;

class ModelRenderData : public RenderData {
public:
	~ModelRenderData();

	virtual void setup();

	void setParams(Shader* shader, glm::mat4 model);

	virtual void addMesh(Mesh* data);

private:
	SModels shaderModels;
	GLuint MBO;

	void clearShaderModels();

	void setupMeshesMBO();

	void configMBO(Models* models);

protected:
	//virtual void configChildren();
	virtual void renderChildren();
	virtual void afterRender();
};

/*
 *	模型资源
 */
class Model : public Asset {
public:
	Model(string name, string pathRoot, string fileName);
	~Model();

	virtual string aType() { return "Model"; };

	void setup();

	Mesh* getMesh(string name);
	MaterialGroup* getMaterial(string name);
	Material* getMaterialData(string name);

	//vector<Vertex> getVertices();
	glm::vec3* getVertices(int& cnt);

	void addMesh(Mesh* data);
	void addMaterial(MaterialGroup* material);

	ModelRenderData* getRenderData();

	//vector<MeshData*>& getData();
	//vector<MaterialAsset*>& getMaterials();

private:
	ModelRenderData renderData;

	vector<Mesh*> data;
	vector<MaterialGroup*> materials;
};
