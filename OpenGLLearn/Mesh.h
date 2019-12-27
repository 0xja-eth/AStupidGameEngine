#pragma once

#include "Material.h"
#include "Config.h"
#include "RenderData.h"

enum RenderType {
	rArrays, rElement
};

struct Vertex {
	glm::vec3 v;
	glm::vec2 vt;
	glm::vec3 vn;

	Vertex() {};
	Vertex(float x, float y, float z, float tx = 0, float ty = 0,
		float nx = 0, float ny = 0, float nz = 0) :
		Vertex(glm::vec3(x, y, z), glm::vec2(tx, ty), glm::vec3(nx, ny, nz)) {};
	Vertex(glm::vec3 v = glm::vec3(0), glm::vec2 vt = glm::vec2(0),
		glm::vec3 vn = glm::vec3(0)) : v(v), vt(vt), vn(vn) {};
};

class MeshRenderData : public RenderData {
public:

	void setup(RenderType type, vector<Vertex>* vertices, vector<GLuint>* indices);

	void setupMBOAttrs(int pos = 3);

	virtual void setMaterial(Material *m);

	void setParams(Shader* shader, int drawCount = 1);

private:
	RenderType type;

	GLuint VAO, VBO, EBO;

	vector<Vertex>* vertices;
	vector<GLuint>* indices;

	MaterialRenderData* material;

	Shader* shader;
	int drawCount;

	void generateRenderData();

	void generateVertexObjects();
	void bindVertexObjects();
	void setupVertexAttrs();

	virtual void beforeConfig();
	//virtual void doConfig();
	virtual void doRender();

};

class Mesh {
public:
#ifdef USE_VEO
	Mesh(string name, RenderType type = rElement);
#else
	Mesh(string name, RenderType type = rArrays);
#endif // USE_VEO
	~Mesh();

	string getName();

	virtual void setMaterial(Material *m);
	Material* getMaterial();

	int addVertex(glm::vec3 v = glm::vec3(0), glm::vec2 vt = glm::vec2(0), glm::vec3 vn = glm::vec3(0));
	int addVertex(Vertex v);
	void addIndex(int index);

	int getVerticesCount();
	Vertex getVertex(int index);
	//glm::vec3* getVertices(int& cnt);

	virtual void setup();

	void debugShow();

	MeshRenderData* getRenderData();

	static Mesh* SqureMesh(string name, float size=1, 
		glm::vec2 minTP = glm::vec2(0), glm::vec2 maxTP = glm::vec2(1),
		bool inner = false, RenderType type = rArrays);
	static Mesh* RectMesh(string name, glm::vec2 minP, glm::vec2 maxP,
		glm::vec2 minTP = glm::vec2(0), glm::vec2 maxTP = glm::vec2(1),
		bool inner = false, RenderType type = rArrays);
	static Mesh* FourPointsMesh(string name, glm::vec3 p[4],
		glm::vec2 tp[4] = NULL, bool inner = false, RenderType type = rArrays);

	static Mesh* CubeMesh(string name, float size = 1, 
		bool inner = false, RenderType type = rArrays);
	static Mesh* CuboidMesh(string name, glm::vec3 minP, glm::vec3 maxP,
		bool inner = false, RenderType type = rArrays);
	static Mesh* EightPointsMesh(string name, glm::vec3 p[8],
		bool inner = false, RenderType type = rArrays);

private:
	RenderType type;
	string name;

	Material* material;

	vector<Vertex> vertices;
	vector<GLuint> indices;

	MeshRenderData renderData;

	void addVertices(glm::vec3* p, int id1, int id2, int id3, bool inner = false);
	void addVertices(glm::vec3* p, int ids, bool inner = false);

	void addVertices(glm::vec3* p, glm::vec2* tp, int id1, int id2, int id3, bool inner = false);
	void addVertices(glm::vec3* p, glm::vec2* tp, int ids, bool inner = false);
};
