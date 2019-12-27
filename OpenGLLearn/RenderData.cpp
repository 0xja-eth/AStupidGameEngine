#include "RenderData.h"

void RenderData::render() {
	config();

	beforeRender();
	renderChildren();
	doRender();
	afterRender();
}

void RenderData::renderChildren() {
	set<ConfigData*>::iterator nit;
	for (nit = next.begin(); nit != next.end(); ++nit)
		((RenderData*)*nit)->render();
}

void RenderData::configChildren() {}

void RenderData::beforeRender() {}
void RenderData::doRender() {}
void RenderData::afterRender() {}
