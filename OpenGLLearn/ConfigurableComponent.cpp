#include "ConfigurableComponent.h"

#include "RenderSystem.h"

void ConfigurableComponent::addShader(Shader * shader) {
	shaders.push_back(shader);
}
void ConfigurableComponent::deleteShader(Shader * shader) {
	vector<Shader*>::iterator iter = getShader(shader);
	if (iter == shaders.end()) return; // 如果找不到，什么都不做
	shaders.erase(iter);
}
bool ConfigurableComponent::hasShader(Shader* shader) {
	return getShader(shader) != shaders.end();
}
void ConfigurableComponent::clearShaders() {
	shaders.clear();
}
vector<Shader*>::iterator ConfigurableComponent::getShader(Shader* shader) {
	return find(shaders.begin(), shaders.end(), shader);
}

ConfigData* ConfigurableComponent::getConfigData() { return NULL; }

void ConfigurableComponent::capture(Camera* camera) {
	if (isActived() && testCapture(camera)) doCapture();
}
bool ConfigurableComponent::testCapture(Camera* camera) { return true; }
void ConfigurableComponent::doCapture() {
	RenderSystem::Get()->captureObject(getConfigData());
}