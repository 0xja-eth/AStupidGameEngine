#pragma once
#include "Model.h"

#include "Asset.h"

class Mesh;
class MeshRenderData;
class Texture;
class TextureRenderData;

class SpriteRenderData : public RenderData {
public:

	void setup(Texture* texture, Mesh* mesh);

	void setParams(Shader* shader, glm::mat4 model);

private:
	SModels shaderModels;
	GLuint MBO;

	MeshRenderData* mesh;
	TextureRenderData* texture;

	void clearShaderModels();

	void setupMeshesMBO();

	void configMBO(Models* models);

protected:
	virtual void beforeRender();
	virtual void renderChildren();
	virtual void afterRender();
};

/*
 *	2D精灵
 */
class Sprite : public Asset {
public:
	Sprite(string name, string pathRoot, string fileName) :
		Asset(name, pathRoot, fileName) {};
	~Sprite();

	virtual string aType() { return "Sprite"; }

	virtual void setup(); // 定义资源（使用的时候自动定义）

	float getWidth();
	float getHeight();

	float getScreenWidth();
	float getScreenHeight();

	/* 获取到目标尺寸的 Scale */
	/* w, h 范围：0 ~ 2 */
	glm::vec3 getTargetSizeScale(float w, float h);

	Texture* getTexture();

	void setTexture(Texture* texture);

	SpriteRenderData* getRenderData();

private:
	Mesh* mesh;
	Texture* texture;

	SpriteRenderData renderData;

	void generateMesh();
};

