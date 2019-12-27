#pragma once

#include "Config.h"

#include "GameException.h"

#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include "Texture.h"
#include "Material.h"
#include "Sprite.h"
/*
 *	资源管理器
 */
/*
static class AssetsManager {
public:
	static Asset* loadAsset(AssetType type, string pathRoot, string fileName1, string fileName2, string assetName, bool force = false);
	static Asset* loadAsset(AssetType type, string pathRoot, string fileName, string assetName, bool force = false);
	static Asset* loadAsset(AssetType type, string filePath, string assetName, bool force = false);
	static Asset* addAsset(Asset* asset);
	static Asset* getAsset(AssetType type, string name);
	static void releaseAsset(AssetType type, string name, bool force = false);
	static void releaseAsset(Asset* asset, bool force = false);

private:
	static vector<ModelAsset*> modelAssets;
	static vector<ShaderAsset*> shaderAssets;

#pragma region Model

	static ModelAsset* loadModelAsset(string filePath, string modelName, bool force = false);
	static ModelAsset* loadModelAsset(string pathRoot, string fileName, string modelName, bool force = false);

	static ModelAsset* addModelAsset(ModelAsset* asset);
	static ModelAsset* getModelAsset(string name);
	static void releaseModelAsset(string name, bool force = false);

	static void readModelAssetObjFile(ModelData& data, string pathRoot, string fileName);
	static void readModelAssetMtlFile(ModelData& data, string pathRoot, string fileName);
	static void readModelAssetFaces(ModelData& data, stringstream & file, Mesh * obj,
		vector<glm::vec3>& vs, vector<glm::vec2>& vts, vector<glm::vec3>& vns,
		int vcnt, int vtcnt, int vncnt);

#pragma endregion

#pragma region Shader

	static ShaderAsset* loadShaderAsset(string pathRoot, string vertexFileName,
		string fragmentFileName, string shaderName, bool force = false);

	static ShaderAsset* addShaderAsset(ShaderAsset* asset);
	static ShaderAsset* getShaderAsset(string name);
	static void releaseShaderAsset(string name, bool force = false);

#pragma endregion

	static stringstream readFileToStrStream(string pathRoot, string fileName);
	static stringstream fStreamToStrStream(ifstream & file);
};
*/

/*
 *	资源管理器
 */
class AssetsPoolBase {
public:
	virtual Asset* load(string assetName, string pathRoot, string fileName, bool force = false);
	virtual Asset* load(string assetName, string filePath, bool force = false);

	virtual Asset* get(string name);
	virtual Asset* get(string pathRoot, string fileName);

	virtual void release(Asset* asset, bool force = false);
	virtual void release(string name, bool force = false);
	virtual void release(string pathRoot, string fileName, bool force = false);

	virtual string type() = 0;
private:

	virtual void release(vector<Asset*>::iterator ait, bool force = false);
protected:
	vector<Asset*> assets;

	virtual Asset* add(Asset* asset);

	virtual Asset* create(string assetName, string pathRoot, string fileName) = 0;

	static stringstream readFileToStrStream(string pathRoot, string fileName);
	static stringstream fStreamToStrStream(ifstream & file);
};

/*
 *	Shader 资源管理器
 */
class ShaderAssetPool : public AssetsPoolBase {
public:
	static ShaderAssetPool* Get() { return self == NULL ? self = new ShaderAssetPool() : self; }

	// 覆盖无用父类函数
	//Asset* load(string assetName, string filePath, bool force = false) { return NULL; };
	//Asset* load(string assetName, string pathRoot, string fileName, bool force = false) { return NULL; };

	Shader* load(string assetName, string pathRoot, string fileName1,
		string fileName2, string fileName3 = "", bool force = false);

	Shader* load(string assetName, string pathRoot, string fileName1,
		string fileName2, bool force = false);

	virtual string type() { return "Shader"; }

private:
	static ShaderAssetPool* self;

	virtual Shader* create(string assetName, string pathRoot, string fileName);

	virtual Shader* create(string assetName, string pathRoot, string fileName1,
		string fileName2, string fileName3 = "");

};

/*
 *	网格资源管理器
 */
class ModelAssetPool : public AssetsPoolBase {
public:
	static ModelAssetPool* Get() { return self == NULL ? self = new ModelAssetPool() : self; }

	virtual string type() { return "Model"; }

private:
	static ModelAssetPool* self;

	virtual Model* create(string assetName, string pathRoot, string fileName);

	static Model* readObjFile(Model* mesh, string pathRoot, string fileName);
	static void readFaces(stringstream & file, Mesh * mesh,
		vector<glm::vec3>& vs, vector<glm::vec2>& vts, vector<glm::vec3>& vns,
		int vcnt, int vtcnt, int vncnt);
};

/*
 *	纹理资源管理器
 */
class SpriteAssetPool : public AssetsPoolBase {
public:
	static SpriteAssetPool* Get() { return self == NULL ? self = new SpriteAssetPool() : self; }

	//Sprite* load(string assetName, string pathRoot, string fileName,
	//	GLenum target = GL_TEXTURE_2D, bool force = false);

	virtual string type() { return "Sprite"; }

private:
	static SpriteAssetPool* self;

	virtual Sprite* create(string assetName, string pathRoot, string fileName);
};

/*
 *	材质资源管理器
 */
class MaterialAssetPool : public AssetsPoolBase {
public:
	static MaterialAssetPool* Get() { return self == NULL ? self = new MaterialAssetPool() : self; }

	virtual string type() { return "MaterialGroup"; }

	virtual Material* getData(string pathRoot, string fileName, string name);

private:
	static MaterialAssetPool* self;

	virtual MaterialGroup* create(string assetName, string pathRoot, string fileName);

	static MaterialGroup* readMtlFile(MaterialGroup* material, string pathRoot, string fileName);
};

/*
 *	纹理资源管理器
 */
class TextureAssetPool : public AssetsPoolBase {
public:
	static TextureAssetPool* Get() { return self == NULL ? self = new TextureAssetPool() : self; }

	Texture* load(string assetName, string pathRoot, string fileName,
		GLenum target = GL_TEXTURE_2D, bool force = false);

	virtual string type() { return "Texture"; }

private:
	static TextureAssetPool* self;

	virtual Texture* create(string assetName, string pathRoot, string fileName);

	virtual Texture* create(string assetName, string pathRoot, string fileName, GLenum target);
};

/*
 *	资源管理器（组）
 */
static class AssetsPool {
public:
	static ShaderAssetPool* Shader;
	static ModelAssetPool* Model;
	static SpriteAssetPool* Sprite;
	static MaterialAssetPool* Material;
	static TextureAssetPool* Texture;
};