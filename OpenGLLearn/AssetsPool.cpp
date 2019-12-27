#include "AssetsPool.h"

/*
vector<ModelAsset*> AssetsManager::modelAssets;
vector<ShaderAsset*> AssetsManager::shaderAssets;

#pragma region Base

Asset* AssetsManager::loadAsset(AssetType type, string filePath, string assetName, bool force) {
	switch (type) {
	case aModel: return loadModelAsset(filePath, assetName, force);
	default: return NULL;
	}
}

Asset* AssetsManager::loadAsset(AssetType type, string assetName, string pathRoot, string fileName, bool force) {
	switch (type) {
	case aModel: return loadModelAsset(assetName, pathRoot, fileName, force);
	default: return NULL;
	}
}

Asset* AssetsManager::loadAsset(AssetType type, string pathRoot, string fileName1, string fileName2, string assetName, bool force) {
	switch (type) {
	case aModel: return loadModelAsset(pathRoot, fileName1, assetName, force);
	case aShader: return loadShaderAsset(pathRoot, fileName1, fileName2, assetName, force);
	default: return NULL;
	}
}

Asset* AssetsManager::addAsset(Asset* asset) {
	switch (asset->getType()) {
	case aModel: return addModelAsset((ModelAsset*)asset);
	case aShader: return addShaderAsset((ShaderAsset*)asset);
	default: return NULL;
	}
}

Asset* AssetsManager::getAsset(AssetType type, string name) {
	switch (type) {
	case aModel: return getModelAsset(name);
	case aShader: return getShaderAsset(name);
	default: return NULL;
	}
}

void AssetsManager::releaseAsset(AssetType type, string name, bool force) {
	switch (type) {
	case aModel: return releaseModelAsset(name, force);
	case aShader: return releaseShaderAsset(name, force);
	}
}

void AssetsManager::releaseAsset(Asset* asset, bool force) {
	releaseAsset(asset->getType(), asset->getName(), force);
}

#pragma endregion

#pragma region Model

ModelAsset* AssetsManager::addModelAsset(ModelAsset* asset) {
	modelAssets.push_back(asset);
	return asset;
}

ModelAsset* AssetsManager::getModelAsset(string name) {
	for (int i = 0; i < modelAssets.size(); i++)
		if (modelAssets[i]->getName() == name) return modelAssets[i];
	return NULL;
}

void AssetsManager::releaseModelAsset(string name, bool force) {
	vector<ModelAsset*>::iterator mit;
	for (mit = modelAssets.begin(); mit < modelAssets.end(); ++mit)
		if ((*mit)->getName() == name) {
			if (force || (*mit)->release()) {
				delete *mit; modelAssets.erase(mit);
			}
			return;
		}
}

ModelAsset* AssetsManager::loadModelAsset(string filePath, string modelName, bool force) {
	ModelAsset* model;
	if (force || (model = getModelAsset(modelName)) == NULL) {
		string root = "", name = "";
		for (int i = 0; i < filePath.size(); i++) {
			char c = filePath[i]; name += c;
			if (c == '/') { root += name; name = ""; }
		}
		model = loadModelAsset(root, name, modelName, true);
	}
	return model;
}

ModelAsset* AssetsManager::loadModelAsset(string pathRoot, string fileName, string modelName, bool force) {
	ModelAsset* model;
	if (force || (model = getModelAsset(modelName)) == NULL) {
		ModelData data;
		readModelAssetObjFile(data, pathRoot, fileName);
		model = addModelAsset(new ModelAsset(pathRoot, fileName, modelName, data));
	}
	return model;
}

#pragma endregion

#pragma region Shader

ShaderAsset* AssetsManager::loadShaderAsset(string pathRoot, string vertexFileName,
	string fragmentFileName, string shaderName, bool force) {
	ShaderAsset* shader;
	if (force || (shader = getShaderAsset(shaderName)) == NULL) 
		shader = addShaderAsset(new ShaderAsset(pathRoot, shaderName, vertexFileName, fragmentFileName));
	return shader;
}

ShaderAsset* AssetsManager::addShaderAsset(ShaderAsset* asset) {
	shaderAssets.push_back(asset);
	return asset;
}

ShaderAsset* AssetsManager::getShaderAsset(string name) {
	for (int i = 0; i < shaderAssets.size(); i++)
		if (shaderAssets[i]->getName() == name) return shaderAssets[i];
	return NULL;
}

void AssetsManager::releaseShaderAsset(string name, bool force ) {
	vector<ShaderAsset*>::iterator sit;
	for (sit = shaderAssets.begin(); sit < shaderAssets.end(); ++sit)
		if ((*sit)->getName() == name) {
			if (force || (*sit)->release()) {
				delete *sit; shaderAssets.erase(sit);
			}
			return;
		}
}

#pragma endregion

stringstream AssetsManager::readFileToStrStream(string pathRoot, string fileName) {
	string filePath = pathRoot + fileName;
	cout << "Loading file " << filePath << endl;
	ifstream file(filePath, ios::binary);
	return fStreamToStrStream(file);
}

stringstream AssetsManager::fStreamToStrStream(ifstream & file) {
	if (!file) throw GameException("Open file Error!");

	vector<char> buf(file.seekg(0, std::ios::end).tellg());
	file.seekg(0, std::ios::beg).read(&buf[0], static_cast<streamsize>(buf.size()));
	file.close();

	return stringstream(string(buf.begin(), buf.end()));
}
*/

Asset* AssetsPoolBase::load(string assetName, string filePath, bool force /*= false*/) {
	Asset* asset;
	string root = "", name = "";
	for (int i = 0; i < filePath.size(); i++) {
		char c = filePath[i]; name += c;
		if (c == '/') { root += name; name = ""; }
	}
	if (force || (asset = get(root, name)) == NULL) {
		asset = load(assetName, root, name, true);
	}
	return asset;
}

Asset* AssetsPoolBase::load(string assetName, string pathRoot, string fileName, bool force /*= false*/) {
	Asset* asset;
	if (force || (asset = get(pathRoot, fileName)) == NULL)
		asset = add(create(assetName, pathRoot, fileName));
	return asset;
}

Asset* AssetsPoolBase::add(Asset* asset) {
	assets.push_back(asset); return asset;
}

Asset* AssetsPoolBase::get(string name) {
	for (int i = 0; i < assets.size(); i++)
		if (assets[i]->getName() == name) return assets[i];
	return NULL;
}
Asset* AssetsPoolBase::get(string pathRoot, string fileName) {
	for (int i = 0; i < assets.size(); i++)
		if (assets[i]->getPathRoot() == pathRoot && 
			assets[i]->getFileName() == fileName) 
			return assets[i];
	return NULL;
}

void AssetsPoolBase::release(Asset* asset, bool force) {
	release(find(assets.begin(), assets.end(), asset), force);
}

void AssetsPoolBase::release(string name, bool force) {
	vector<Asset*>::iterator ait;
	for (ait = assets.begin(); ait < assets.end(); ++ait)
		if ((*ait)->getName() == name) {
			release(ait); return;
		}
}

void AssetsPoolBase::release(string pathRoot, string fileName, bool force) {
	vector<Asset*>::iterator ait;
	for (ait = assets.begin(); ait < assets.end(); ++ait)
		if ((*ait)->getPathRoot() == pathRoot &&
			(*ait)->getFileName() == fileName) {
			release(ait); return;
		}
}

void AssetsPoolBase::release(vector<Asset*>::iterator ait, bool force /*= false*/) {
	if (force || (*ait)->release()) {
		delete *ait; assets.erase(ait);
	}
}

stringstream AssetsPoolBase::readFileToStrStream(string pathRoot, string fileName) {
	string filePath = pathRoot + fileName;
	cout << "Loading file " << filePath << endl;
	ifstream file(filePath, ios::binary);
	return fStreamToStrStream(file);
}

stringstream AssetsPoolBase::fStreamToStrStream(ifstream & file) {
	if (!file) throw GameException("Open file Error!");

	vector<char> buf(file.seekg(0, std::ios::end).tellg());
	file.seekg(0, std::ios::beg).read(&buf[0], static_cast<streamsize>(buf.size()));
	file.close();

	return stringstream(string(buf.begin(), buf.end()));
}

ShaderAssetPool* ShaderAssetPool::self = new ShaderAssetPool;
ModelAssetPool* ModelAssetPool::self = new ModelAssetPool;
SpriteAssetPool* SpriteAssetPool::self = new SpriteAssetPool;
MaterialAssetPool* MaterialAssetPool::self = new MaterialAssetPool;
TextureAssetPool* TextureAssetPool::self = new TextureAssetPool;

ShaderAssetPool* AssetsPool::Shader = ShaderAssetPool::Get();
ModelAssetPool* AssetsPool::Model = ModelAssetPool::Get();
SpriteAssetPool* AssetsPool::Sprite = SpriteAssetPool::Get();
MaterialAssetPool* AssetsPool::Material = MaterialAssetPool::Get();
TextureAssetPool* AssetsPool::Texture = TextureAssetPool::Get();

#pragma region Shader

Shader* ShaderAssetPool::load(string assetName, string pathRoot, string fileName1, string fileName2, string fileName3 /*= ""*/, bool force /*= false*/) {
	Shader* asset;
	if (force || (asset = (Shader*)get(assetName)) == NULL)
		asset = (Shader*)add(create(assetName, pathRoot, fileName1, fileName2, fileName3));
	return asset;
}

Shader* ShaderAssetPool::load(string assetName, string pathRoot, string fileName1, string fileName2, bool force /*= false*/) {
	return load(assetName, pathRoot, fileName1, fileName2, "", force);
}

Shader* ShaderAssetPool::create(string assetName, string pathRoot, string fileName) {
	return NULL;
}

Shader* ShaderAssetPool::create(string assetName, string pathRoot, string fileName1, string fileName2, string fileName3 /*= ""*/) {
	return new Shader(assetName, pathRoot, fileName1, fileName2, fileName3);
}

#pragma endregion

#pragma region Model

Model* ModelAssetPool::create(string assetName, string pathRoot, string fileName) {
	return readObjFile(new Model(assetName, pathRoot, fileName), pathRoot, fileName);
}

Model* ModelAssetPool::readObjFile(Model* asset, string pathRoot, string fileName) {

	Mesh* mesh = NULL;

	vector<glm::vec3> vs, vns;
	vector<glm::vec2> vts;

	stringstream ss = readFileToStrStream(pathRoot, fileName);

	float x, y, z;
	int vcnt = 0, vtcnt = 0, vncnt = 0;
	char type[64], name[128];

	while (!ss.eof()) {
		ss >> type;
		//cout << "<<" << type << ">>" << endl;
		// 解析 mtllib：材质库（可以定义多个）
		if (strcmp(type, "mtllib") == 0) {
			char mtl[128];
			ss.getline(name, 128);
			stringstream ss(name);
			while (ss >> mtl)
				asset->addMaterial((MaterialGroup*)AssetsPool::
					Material->load(mtl, pathRoot, mtl, false));
		} // 解析 vn：法线
		else if (strcmp(type, "o") == 0) {
			ss >> name;
			mesh = asset->getMesh(name);
			if (mesh == NULL) {
				mesh = new Mesh(name);
				asset->addMesh(mesh);
			} else throw GameException("Duplicate object!");
			cout << "Loading object: " << mesh->getName() << endl;
		} else if (mesh != NULL) { // 当当前Object不为空时，才解析后续内容
			// 解析 v：顶点
			if (strcmp(type, "v") == 0) {
				ss >> x >> y >> z; vcnt++;
				//cout << vs.size() << ": " << x << "," << y << "," << z << endl;
				vs.push_back(glm::vec3(x, y, z));
			} // 解析 vt：纹理坐标
			else if (strcmp(type, "vt") == 0) {
				ss >> x >> y; vtcnt++;
				//cout << vts.size() << ": " << x << "," << y << endl;
				vts.push_back(glm::vec2(x, y));
			} // 解析 vn：法线
			else if (strcmp(type, "vn") == 0) {
				ss >> x >> y >> z; vncnt++;
				//cout << vns.size() << ": " << x << "," << y << "," << z << endl;
				vns.push_back(glm::vec3(x, y, z));
			} // 解析 usemtl：使用的材质
			else if (strcmp(type, "usemtl") == 0) {
				ss >> name;
				mesh->setMaterial(asset->getMaterialData(name));
				if (mesh->getMaterial() == NULL)
					throw GameException("Material not found!");
				cout << "Use mtl: " << name << endl;
			} // 解析 f：面
			else if (strcmp(type, "f") == 0) {
				readFaces(ss, mesh, vs, vts, vns, vcnt, vtcnt, vncnt);
			}
		}
	}
	//obj->show();
	cout << "Load completed!" << endl;
	return asset;
}

void ModelAssetPool::readFaces(stringstream & file, Mesh * mesh,
	vector<glm::vec3>& vs, vector<glm::vec2>& vts, vector<glm::vec3>& vns,
	int vcnt, int vtcnt, int vncnt) {
	char face[128]; int v, vt, vn;
	for (int i = 0; i < 3; i++) {
		v = vt = vn = 0; file >> face;
		if (sscanf_s(face, "%d/%d/%d", &v, &vt, &vn) == 3);
		//cout << face << " => " << v << "," << vt << "," << vn << endl;
		else if (sscanf_s(face, "%d//%d", &v, &vn) == 2);
		//cout << face << " => " << v << "," << vn << endl;
		else if (sscanf_s(face, "%d/%d", &v, &vt) == 2);
		//cout << face << " => " << v << "," << vt << endl;
		else if (sscanf_s(face, "%d", &v) == 1);
		//cout << face << " => " << v << endl;
		else throw GameException("Face format error!");
		v -= 1; vt -= 1; vn -= 1;
		// 解析出每一个ID，加入到数组中
		glm::vec3 vv, vnv; glm::vec2 vtv; // 实际值
		if (v > vs.size()) v -= vcnt;
		if (vt > vts.size()) vt -= vtcnt;
		if (vn > vns.size()) vn -= vncnt;
		vv = vs[v];
		vtv = vt >= 0 ? vts[vt] : glm::vec2(0);
		vnv = vn >= 0 ? vns[vn] : glm::vec3(0);
		//cout << "face " << (i + 1) << " : " << index << " => "
		//	<< v << "," << vt << "," << vn << endl;
#ifdef USE_VEO
		mesh->addIndex(mesh->addVertex(vv, vtv, vnv));
#else
		mesh->addVertex(vv, vtv, vnv, true);
#endif
	}
}

#pragma endregion

#pragma region Sprite

Sprite* SpriteAssetPool::create(string assetName, string pathRoot, string fileName) {
	Sprite* sprite = new Sprite(assetName, pathRoot, fileName);
	sprite->setTexture(TextureAssetPool::Get()->load(assetName, pathRoot, fileName));
	return sprite;
}

#pragma endregion

#pragma region Material

Material* MaterialAssetPool::getData(string pathRoot, string fileName, string name) {
	MaterialGroup* material = (MaterialGroup*)get(pathRoot, fileName);
	return material->getData(name);
}


MaterialGroup* MaterialAssetPool::create(string assetName, string pathRoot, string fileName) {
	MaterialGroup* material = new MaterialGroup(assetName, pathRoot, fileName);
	return readMtlFile(material, pathRoot, assetName);
}

MaterialGroup* MaterialAssetPool::readMtlFile(MaterialGroup* material, string pathRoot, string fileName) {
	Material* mat = NULL;

	stringstream ss = readFileToStrStream(pathRoot, fileName);

	float x, y, z; int val;
	char type[64], name[128];
	string path;

	while (!ss.eof()) {
		ss >> type;
		// 新建并切换当前材质
		if (strcmp(type, "newmtl") == 0) {
			ss >> name;
			mat = material->getData(name);
			if (mat == NULL) {
				mat = new Material(name);
				material->addData(mat);
			} else throw GameException("Duplicate material!");
			cout << "Loading material: " << mat->getName() << endl;
		} else if (mat != NULL) { // 当当前材质不为空时，才解析后续内容
			// 解析 Ns, Ni, d
			if (strcmp(type, "Ns") == 0 ||
				strcmp(type, "Ni") == 0 ||
				strcmp(type, "d") == 0) {
				ss >> x; mat->setAttr(type, x);
				cout << "  " << type << " = " << x << endl;
			}
			// 解析 illum
			else if (strcmp(type, "illum") == 0) {
				ss >> val; mat->setAttr(type, val);
				cout << "  " << type << " = " << val << endl;
			} // 解析 Ka, Kd, Ks
			else if (strcmp(type, "Ka") == 0 ||
				strcmp(type, "Kd") == 0 ||
				strcmp(type, "Ks") == 0) {
				ss >> x >> y >> z; mat->setAttr(type, glm::vec3(x, y, z));
				cout << "  " << type << " = " << x << "," << y << "," << z << endl;
			}
			// 解析 map_Kd, map_Bump, map_Ks
			else if (strcmp(type, "map_Kd") == 0 ||
				strcmp(type, "map_Bump") == 0 ||
				strcmp(type, "map_Ks") == 0) {
				ss >> name; mat->setAttr(type,
					AssetsPool::Texture->load(name, pathRoot, name));
				cout << "  " << type << " = " << path << endl;
			}
		}
	}

	return material;
}

#pragma endregion

#pragma region Texture

Texture* TextureAssetPool::load(string assetName, string pathRoot, string fileName,
	GLenum target /*= GL_TEXTURE_2D*/, bool force /*= false*/) {
	Texture* asset;
	if (force || (asset = (Texture*)get(assetName)) == NULL)
		asset = (Texture*)add(create(assetName, pathRoot, fileName, target));
	return asset;
}

Texture* TextureAssetPool::create(string assetName, string pathRoot, string fileName) {
	return create(assetName, pathRoot, fileName, GL_TEXTURE_2D);
}

Texture* TextureAssetPool::create(string assetName, string pathRoot, string fileName, GLenum target) {
	return new Texture(assetName, pathRoot, fileName, target);
}

#pragma endregion


