#pragma once
#include "ConfigData.h"

class RenderData : public ConfigData {
public:
	void render();

protected:
	virtual void configChildren();

	virtual void renderChildren();
	virtual void beforeRender();
	virtual void doRender();
	virtual void afterRender();
};

