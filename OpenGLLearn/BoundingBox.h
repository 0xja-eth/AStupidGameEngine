#pragma once

#include "RenderableComponent.h"

enum BoundingType {
	Sphere, AABB, OBB
};

class BoundingBoxRenderData : public RenderData {
public:
	void setup(glm::vec3 minP, glm::vec3 maxP);
	void setup(Shader* shader, glm::mat4 model);

private:
	glm::mat4 model;

	Shader* shader;
	Mesh* mesh;

	virtual void beforeConfig();

	virtual void beforeRender();
	//virtual void doRender();
	virtual void afterRender();
};

class Entity;
class Camera;
/*
 *	包围盒组件
 */
class BoundingBox : public RenderableComponent {
public:
	BoundingBox(string name, Entity* entity, BoundingType type=AABB);
	~BoundingBox();

	virtual string cType() { return "BoundingBox"; };

	virtual BoundingBox* copy();

	bool check(glm::vec3 pos);
	bool check(BoundingBox* box);

	void setShowBox(bool val);

	virtual RenderData* getRenderData();

private:
	bool showBox = false;

	Entity* entity;
	BoundingType type;

	// 包围盒的2个端点（AABB, OBB）
	glm::vec3 box[2];
	// 球型包围体数据（球心，半径）
	glm::vec4 sphere;
	
	Model* lastModel;

	BoundingBoxRenderData renderData;

	virtual void doUpdate();

	void updateShowBox();
	void updateBoundingBox();

	void generateBoundingBox(glm::vec3* vs, int count);

	void generateSphere(glm::vec3* vs, int count);
	void generateAABB(glm::vec3* vs, int count);
	void generateOBB(glm::vec3* vs, int count);

	bool checkLocal(glm::vec3 pos);
	bool check2AABB(BoundingBox* box);

	glm::vec3* generateFullBox();

	virtual bool testCapture(Camera* camera);
};

