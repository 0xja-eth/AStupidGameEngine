#include "RenderableComponent.h"
#include "RenderSystem.h"

RenderData* RenderableComponent::getRenderData() { return NULL; }

void RenderableComponent::setShadingShader(Shader* shader) {
	vector<Shader*>::iterator sit = getShader(shader);
	setShadingShader(sit - shaders.begin());
}
void RenderableComponent::setShadingShader(int index) {
	shadingShaderIndex = index;
}

Shader* RenderableComponent::shadingShader() {
	return shaders.size() < shadingShaderIndex + 1 ? NULL : shaders[shadingShaderIndex];
}

bool RenderableComponent::testCapture(Camera* camera) {
	return true;
}
void RenderableComponent::doCapture() {
	RenderSystem::Get()->captureObject(getRenderData());
}