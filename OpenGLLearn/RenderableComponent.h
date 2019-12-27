#pragma once

#include "ConfigurableComponent.h"

const string ModelAttrName = "model";
/*
 *	����Ⱦ�������ֻ����һ�� Shader ��Ⱦ��
 */
class RenderableComponent : public ConfigurableComponent {
public:
	RenderableComponent(string name = "") : ConfigurableComponent(name) {}

	virtual bool isRenderable() { return true; };

	virtual RenderData* getRenderData();

	virtual void setShadingShader(int index);
	virtual void setShadingShader(Shader* shader);
	virtual Shader* shadingShader();

private:
	int shadingShaderIndex = 0;

	ConfigData* getConfigData() { return getRenderData(); };

protected:
	virtual bool testCapture(Camera* camera);
	virtual void doCapture();
};

