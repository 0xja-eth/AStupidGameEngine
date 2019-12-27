#pragma once
#include "RenderData.h"

#include "RenderableComponent.h"

class Mesh;
class Texture;
class TextureRenderData;
/*
 *	��պ���Ⱦ����
 */
class SkyboxRenderData : public RenderData {
public:
	~SkyboxRenderData();

	void setup(Texture* texture);
	void setup(Shader* shader);

private:
	Shader* shader;

	TextureRenderData* texture;
	Mesh* mesh = NULL;

	void generateMesh();

protected:

	virtual void beforeConfig();

	virtual void beforeRender();
	//virtual void doRender();
	virtual void dafterRender();
};

/*
 *	��պ�
 */
class Skybox : public RenderableComponent {
public:
	//Skybox(string name, string path);
	Skybox(string name, Texture* texture);

	virtual string cType() { return "Skybox"; };

	virtual Skybox* copy();

	virtual RenderData* getRenderData();

private:
	Texture* texture;

	SkyboxRenderData renderData;
protected:
	virtual bool testCapture(Camera* camera);
};

