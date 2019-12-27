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
 *	2D����
 */
class Sprite : public Asset {
public:
	Sprite(string name, string pathRoot, string fileName) :
		Asset(name, pathRoot, fileName) {};
	~Sprite();

	virtual string aType() { return "Sprite"; }

	virtual void setup(); // ������Դ��ʹ�õ�ʱ���Զ����壩

	float getWidth();
	float getHeight();

	float getScreenWidth();
	float getScreenHeight();

	/* ��ȡ��Ŀ��ߴ�� Scale */
	/* w, h ��Χ��0 ~ 2 */
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

