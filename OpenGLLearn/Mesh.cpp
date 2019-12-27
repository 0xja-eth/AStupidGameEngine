#include "Mesh.h"

#include "Shader.h"

void MeshRenderData::setup(RenderType type, vector<Vertex>* vertices, vector<GLuint>* indices) {
	this->type = type;
	this->vertices = vertices;
	this->indices = indices;
	generateRenderData();
}

void MeshRenderData::setMaterial(Material *m) {
	clearNexts();
	material = m->getRenderData();
	addNext(material);
}

void MeshRenderData::setParams(Shader* shader, int drawCount) {
	this->shader = shader;
	this->drawCount = drawCount;
}

void MeshRenderData::generateRenderData() {
	generateVertexObjects();
	bindVertexObjects();
	setupVertexAttrs();
}

void MeshRenderData::generateVertexObjects() {
	// 创建顶点&缓冲区
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	if (type == rElement)
		glGenBuffers(1, &EBO);
}

void MeshRenderData::bindVertexObjects() {
	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// 绑定顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex),
		&(*vertices)[0], GL_STATIC_DRAW);

	if (type == rElement) {
		// 绑定索引数组
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint),
			&(*indices)[0], GL_STATIC_DRAW);
	}
}

void MeshRenderData::setupVertexAttrs() {
	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// 设置顶点属性
	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// 顶点纹理坐标
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vt));
	// 顶点法线
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vn));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshRenderData::setupMBOAttrs(int pos) {
	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// Model Buffers Object
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(pos + 1);
	glVertexAttribPointer(pos + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(pos + 2);
	glVertexAttribPointer(pos + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(pos + 3);
	glVertexAttribPointer(pos + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(pos, 1);
	glVertexAttribDivisor(pos + 1, 1);
	glVertexAttribDivisor(pos + 2, 1);
	glVertexAttribDivisor(pos + 3, 1);
}

void MeshRenderData::beforeConfig() {
	if (material != NULL) material->setParams(shader);
}
/*
void MeshRenderData::doConfig() {

}
*/
void MeshRenderData::doRender() {
	shader->activate();
	// 绑定顶点数组对象
	glBindVertexArray(VAO);
	if (type == rElement)
		glDrawElementsInstanced(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0, drawCount);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	if (type == rArrays)
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertices->size(), drawCount);
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

Mesh::Mesh(string name, RenderType type /*= rVEO*/) :name(name), type(type) {}

/*
TextureAsset* ModelData::getTex(string path) {
	for (int i = 0; i < textures.size(); i++)
		if (textures[i]->getFilePath() == path) return textures[i];
	return NULL;
}
MaterialData* ModelData::getMtl(string name) {
	MaterialData* res = NULL;
	for (int i = 0; i < matrials.size(); i++)
		if ((res = matrials[i]->getData(name)) != NULL) return res;
	return NULL;
}

TextureAsset* ModelData::getOrCreateTex(string path) {
	TextureAsset* tex = getTex(path);
	if (tex == NULL) {
		tex = new TextureAsset(path);
		textures.push_back(tex);
	}
	return tex;
}
*/

Mesh::~Mesh() {
	delete material;
}

string Mesh::getName() { return name; }

void Mesh::setMaterial(Material *m) { 
	renderData.setMaterial(material = m);
}
Material* Mesh::getMaterial() { return material; }

void Mesh::addIndex(int index) {
	indices.push_back(index);
}

int Mesh::getVerticesCount() {
	return vertices.size();
}

Vertex Mesh::getVertex(int index) {
	return vertices[index];
}
/*
glm::vec3* Mesh::getVertices(int& cnt) {
	cnt = vertices.size();
	glm::vec3* res = new glm::vec3[cnt];
	for (int i = 0; i < cnt; ++i)
		res[i] = vertices[i].v;
	return res;
}
*/
int Mesh::addVertex(glm::vec3 v, glm::vec2 vt, glm::vec3 vn) {
	return addVertex(Vertex(v, vt, vn));
}

int Mesh::addVertex(Vertex v) {
	vertices.push_back(v);
	return vertices.size() - 1;
}
void Mesh::setup() {
	if(material!=NULL) material->setup();
	renderData.setup(type, &vertices, &indices);
}
/*
void Mesh::render(Shader *shader) {
	
	material->config(shader);
	// 绘制物体
	glBindVertexArray(VAO);
	if (type == rVEO)
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	if (type == rNormal)
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
	
	// 重置纹理
	glActiveTexture(GL_TEXTURE0);
}
*/
MeshRenderData* Mesh::getRenderData() { 
	return &renderData; 
}

void Mesh::debugShow() {
	cout << "Vertices: " << endl;
	for (int i = 0; i < vertices.size(); i++) {
		Vertex v = vertices[i];
		cout << "vertices[" << i << "] => ";
		cout << "v:" << v.v.x << "," << v.v.y << "," << v.v.z << " | ";
		cout << "vt:" << v.vt.x << "," << v.vt.y << " | ";
		cout << "vn:" << v.vn.x << "," << v.vn.y << "," << v.vn.z << endl;
	}
	cout << "Indices: " << endl;
	for (int i = 0; i < indices.size(); i++) {
		cout << "indices[" << i << "] = " << indices[i] << endl;
	}
}

Mesh* Mesh::SqureMesh(string name, float size/*=1*/, 
	glm::vec2 minTP, glm::vec2 maxTP, bool inner, RenderType type) {
	return RectMesh(name, glm::vec2(-size), glm::vec2(size), minTP, maxTP, inner, type);
}
Mesh* Mesh::RectMesh(string name, glm::vec2 minP, glm::vec2 maxP, 
	glm::vec2 minTP, glm::vec2 maxTP, bool inner, RenderType type) {
	glm::vec3* res = new glm::vec3[4];
	glm::vec2* tex = new glm::vec2[4];
	res[0] = glm::vec3(minP.x, minP.y, 0);
	res[1] = glm::vec3(maxP.x, minP.y, 0);
	res[2] = glm::vec3(minP.x, maxP.y, 0);
	res[3] = glm::vec3(maxP.x, maxP.y, 0);

	tex[0] = minTP;
	tex[1] = glm::vec2(maxTP.x, minTP.y);
	tex[2] = glm::vec2(minTP.x, maxTP.y);
	tex[3] = maxTP;

	return FourPointsMesh(name, res, tex, inner, type);
}
Mesh* Mesh::FourPointsMesh(string name, glm::vec3 p[4], 
	glm::vec2 tp[4], bool inner /*= false*/, RenderType type /*= rArrays*/) {
	Mesh* mesh = new Mesh(name, type);
	switch (type) {
	case rArrays:
		//（012, 132）
		mesh->addVertices(p, tp, 12, inner);
		mesh->addVertices(p, tp, 132, inner);
		break;
	}
	mesh->setup();
	return mesh;
}

Mesh* Mesh::CubeMesh(string name, float size, bool inner, RenderType type) {
	return CuboidMesh(name, glm::vec3(-size), glm::vec3(size), inner, type);
}
Mesh* Mesh::CuboidMesh(string name, glm::vec3 minP, glm::vec3 maxP, bool inner, RenderType type /*= rArrays*/) {
	glm::vec3* res = new glm::vec3[8];
	res[0] = minP;
	res[1] = glm::vec3(maxP.x, minP.y, minP.z);
	res[2] = glm::vec3(minP.x, maxP.y, minP.z);
	res[3] = glm::vec3(minP.x, minP.y, maxP.z);
	res[4] = glm::vec3(maxP.x, maxP.y, minP.z);
	res[5] = glm::vec3(minP.x, maxP.y, maxP.z);
	res[6] = glm::vec3(maxP.x, minP.y, maxP.z);
	res[7] = maxP;
	return EightPointsMesh(name, res, inner, type);
}
Mesh* Mesh::EightPointsMesh(string name, glm::vec3 p[8], bool inner, RenderType type /*= rArrays*/) {
	Mesh* mesh = new Mesh(name, type);
	switch (type) {
	case rArrays:
		// 后（041, 024）
		mesh->addVertices(p, 41, inner);
		mesh->addVertices(p, 24, inner);
		// 左
		mesh->addVertices(p, 320, inner);
		mesh->addVertices(p, 352, inner);
		// 前
		mesh->addVertices(p, 653, inner);
		mesh->addVertices(p, 675, inner);
		// 右
		mesh->addVertices(p, 176, inner);
		mesh->addVertices(p, 147, inner);
		// 上
		mesh->addVertices(p, 274, inner);
		mesh->addVertices(p, 257, inner);
		// 下
		mesh->addVertices(p, 316, inner);
		mesh->addVertices(p, 301, inner);
		break;
	}
	mesh->setup();
	return mesh;
}

void Mesh::addVertices(glm::vec3* p, int id1, int id2, int id3, bool inner) {
	if (inner) addVertices(p, id3, id2, id1, false);
	addVertex(p[id1]); addVertex(p[id2]); addVertex(p[id3]);
}

void Mesh::addVertices(glm::vec3* p, int ids, bool inner) {
	addVertices(p, ids / 100, ids / 10 % 10, ids % 10, inner);
}

void Mesh::addVertices(glm::vec3* p, glm::vec2* tp, int id1, int id2, int id3, bool inner /*= false*/) {
	addVertex(p[id1], tp[id1]); addVertex(p[id2], tp[id2]); addVertex(p[id3], tp[id3]);
}
void Mesh::addVertices(glm::vec3* p, glm::vec2* tp, int ids, bool inner /*= false*/) {
	if (tp == NULL) addVertices(p, tp, ids / 100, ids / 10 % 10, ids % 10, inner);
	else addVertices(p, tp, ids / 100, ids / 10 % 10, ids % 10, inner);
}

