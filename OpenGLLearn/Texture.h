#pragma once
#include <glad/glad.h>

#include <vector>

#include "stb_image.h"

#include "RenderData.h"

#include "Asset.h"

using namespace std;

class Shader;

enum UseType {
	Normal, Kd, Ks, Bump, MaxUseType
};

const string TextureSuffix = "Tex";
const string ValidSuffix = "Valid";
/*
const string TextureTypes[] = {
	"ambientTex","diffuseTex","specularTex","bumpTex"
};
*/
const int TextureCount = 6;
const string SkyboxFacesName[TextureCount] = {
	//"right.tga" ,"left.tga" ,"top.tga" ,"bottom.tga" ,"back.tga" ,"front.tga"
	//"right.jpg" ,"left.jpg" ,"top.jpg" ,"bottom.jpg" ,"front.jpg" ,"back.jpg"
	"right.png" ,"left.png" ,"top.png" ,"bottom.png" ,"front.png" ,"back.png"
};

class TextureRenderData : public RenderData {
public:
	TextureRenderData() {};
	TextureRenderData(UseType useType) : index(useType) {};

	void setup(GLuint id, GLuint target);

	void setParams(Shader* shader);
	void setParams(Shader* shader, string name);

private:

	GLuint textureID = 0;
	GLenum target = GL_TEXTURE_2D;

	int index = 0;
	string attrName = "";
	Shader* shader = NULL;

protected:
	virtual void doConfig();
	virtual void afterRender();
};

class Texture : public Asset {

public:

	Texture(string pathRoot, string fileName, string name,
		GLenum target = GL_TEXTURE_2D);
	~Texture();

	virtual string aType() { return "Texture"; };

	GLuint getTextureID();

	int getWidth();
	int getHeight();
	int getChannels();

	bool isLoaded();

	virtual void setup(); // 定义资源（使用的时候自动定义）

	TextureRenderData* getRenderData(UseType type = Normal);
private:

	GLuint textureID;
	GLenum target;

	int width, height, nrChannels;

	bool loaded = false;

	//vector<TextureRenderData*> renderData;
	
	TextureRenderData renderData[MaxUseType];

	void loadTexture();
	void load2DTexture();
	void loadCubeMap();

	void setTexParams();
};
