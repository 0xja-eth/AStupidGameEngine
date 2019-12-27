#pragma once
#include "RenderableComponent.h"

class Sprite;

/*
 *	图片
 */
class Image : public RenderableComponent {
public:
	//Image(string name, string textureName);
	//Image(string name, string filePath, string textureName);
	//Image(string name, string pathRoot, string fileName, string textureName);
	Image(string name, Sprite* sprite);

	virtual string cType() { return "Image"; };

	virtual Image* copy();

	float getWidth();
	float getHeight();

	float getScreenWidth();
	float getScreenHeight();

	/* 获取到目标尺寸的 Scale */
	/* w, h 范围：0 ~ 2 */
	glm::vec3 getTargetSizeScale(float w, float h);

	/* w, h 范围：0 ~ 2 */
	void setTargetSize(float w, float h, bool reset = true);

	Sprite* getSprite();
	Texture* getTexture();

	void setSprite(Sprite* sprite);

	virtual RenderData* getRenderData();

private:
	Sprite* sprite;
};

