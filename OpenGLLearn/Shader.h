#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Asset.h"

using namespace std;

class Shader : public Asset {
public:
	// 构造器读取并构建着色器
	Shader(string name, string pathRoot,
		string vertexFileName, string fragmentFileName,
		string geometryFileName = "");

	string aType() { return "Shader"; };

	virtual void setup();

	string getVertexFileName();
	string getFragmentFileName();
	string getVertexFilePath();
	string getFragmentFilePath();

	unsigned int getShaderID();

	// 使用/激活程序
	void activate();

	// uniform工具函数
	void setBool(const string & name, bool value) const;
	void setInt(const string & name, int value) const;
	void setFloat(const string & name, float value) const;
	void setVec3(const string & name, glm::vec3 vec) const;
	void setVec3(const string & name, float x, float y, float z) const;
	void setMat4(const string & name, glm::mat4 value) const;
	void setMat4(const string & name, const GLfloat *value) const;

	// 绑定函数
	void bind(const string & name, GLuint index) const;

private:
	// 程序ID
	unsigned int shaderID;

	string vertexFileName, fragmentFileName, geometryFileName;

	void read(string& vertexCode, string& fragmentCode);
	void complie(string vertexCode, string fragmentCode);

	void checkCompileErrors(unsigned int shader, string type);

};
