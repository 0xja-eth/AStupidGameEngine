#include "Image.h"

#include "Node.h"
#include "Sprite.h"

Image::Image(string name, Sprite* sprite) : RenderableComponent(name) {
	setSprite(sprite);
}

Image* Image::copy() {
	Image* comp = new Image(getName(), this->sprite);
	return (Image*)Component::copy(comp);
}

float Image::getWidth() {
	return sprite->getWidth();
}
float Image::getHeight() {
	return sprite->getHeight();
}

float Image::getScreenWidth() {
	return sprite->getScreenWidth();
}

float Image::getScreenHeight() {
	return sprite->getScreenHeight();
}

glm::vec3 Image::getTargetSizeScale(float w, float h) {
	return sprite->getTargetSizeScale(w, h);
}

void Image::setTargetSize(float w, float h, bool reset) {
	if (sprite->getStatus() == Undefined) return;
	Transform* tf = getTransform();
	tf->scaleTo(getTargetSizeScale(w, h));
	if (reset) tf->resetSca();
}

Sprite* Image::getSprite() {
	return sprite;
}

Texture* Image::getTexture() {
	return sprite->getTexture();
}

void Image::setSprite(Sprite* sprite) {
	useAsset(this->sprite = sprite);
}

RenderData* Image::getRenderData() {
	glm::mat4 curModel = getTransform()->curModel();
	SpriteRenderData* renderData = sprite->getRenderData();
	renderData->setParams(shadingShader(), curModel);
	return renderData;
}
