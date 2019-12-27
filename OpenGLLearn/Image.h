#pragma once
#include "RenderableComponent.h"

class Sprite;

/*
 *	ͼƬ
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

	/* ��ȡ��Ŀ��ߴ�� Scale */
	/* w, h ��Χ��0 ~ 2 */
	glm::vec3 getTargetSizeScale(float w, float h);

	/* w, h ��Χ��0 ~ 2 */
	void setTargetSize(float w, float h, bool reset = true);

	Sprite* getSprite();
	Texture* getTexture();

	void setSprite(Sprite* sprite);

	virtual RenderData* getRenderData();

private:
	Sprite* sprite;
};

