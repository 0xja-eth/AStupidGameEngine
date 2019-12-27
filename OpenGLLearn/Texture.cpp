#include "Shader.h"
#include "Material.h"
#include "Texture.h"

void TextureRenderData::setup(GLuint id, GLuint target) {
	textureID = id; this->target = target; 
}

void TextureRenderData::setParams(Shader* shader) {
	string name = MaterialName + "." + MaterialAttrs[index] + TextureSuffix;
	setParams(shader, name);
}
void TextureRenderData::setParams(Shader* shader, string name) {
	this->shader = shader; attrName = name;
}

// ÅäÖÃ Shader
void TextureRenderData::doConfig() {
	shader->activate();

	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(target, textureID);

	// ÉèÖÃ uniform ÎÆÀí
	shader->setInt(attrName, index);
}

void TextureRenderData::afterRender() {
	//glActiveTexture(GL_TEXTURE0);
}

Texture::Texture(string name, string pathRoot, string fileName, GLenum target /*= GL_TEXTURE_2D*/) :
	Asset(name, pathRoot, fileName), target(target) {
	renderData[Normal] = TextureRenderData(Normal);
	renderData[Kd] = TextureRenderData(Kd);
	renderData[Ks] = TextureRenderData(Ks);
	renderData[Bump] = TextureRenderData(Bump);
};

Texture::~Texture() {
	/*for (int i = 0;i< renderData.size();++i) 
		delete renderData[i];
	renderData.clear();*/
}

GLuint Texture::getTextureID() { return textureID; }

int Texture::getWidth() { return width; }
int Texture::getHeight() { return height; }
int Texture::getChannels() { return nrChannels; }

bool Texture::isLoaded() { return loaded; }

void Texture::setup() {
	Asset::setup();
	if (loaded) return;
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);

	loadTexture(); 
	
	glBindTexture(target, 0); 

	for (int i = 0; i < MaxUseType; ++i)
	//	renderData.push_back(new TextureRenderData((UseType)i));
		renderData[i].setup(textureID, target);
}

TextureRenderData* Texture::getRenderData(UseType type) {
	return &renderData[type];
}

void Texture::loadTexture() {
	cout << "Loading texture " << getFilePath() << endl;
	if (target == GL_TEXTURE_2D) load2DTexture();
	if (target == GL_TEXTURE_CUBE_MAP) loadCubeMap();
	setTexParams();
	loaded = true;
}

void Texture::load2DTexture() {
	unsigned char *data = stbi_load(getFilePath().c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		GLenum format = GL_RED;
		switch (nrChannels) {
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}

		cout << "Loaded successfully towards " << textureID <<
			", width: " << width << ", height: " << height << ", nrChannels: " << nrChannels << endl;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	} else throw "Failed to load texture!";
	stbi_image_free(data);
}

void Texture::loadCubeMap() {
	for (unsigned int i = 0; i < TextureCount; i++) {
		string filePath = getPathRoot() + SkyboxFacesName[i];

		cout << "Loading texture " << filePath << endl;

		unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

		if (data) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else throw "Failed to load texture!";
		stbi_image_free(data);
	}
}

void Texture::setTexParams() {
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (target == GL_TEXTURE_2D) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	if (target == GL_TEXTURE_CUBE_MAP) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}