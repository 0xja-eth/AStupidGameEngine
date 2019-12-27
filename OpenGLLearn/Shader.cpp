#include "Shader.h"

Shader::Shader(string name, string pathRoot, string vertexFileName,
	string fragmentFileName, string geometryFileName) :
	Asset(name, pathRoot, vertexFileName),
	vertexFileName(vertexFileName), 
	fragmentFileName(fragmentFileName),
	geometryFileName(geometryFileName) { setup(); }

void Shader::setup() {
	Asset::setup();
	string vertexCode, fragmentCode;
	read(vertexCode, fragmentCode);
	complie(vertexCode, fragmentCode);
}

string Shader::getVertexFileName() { return vertexFileName; }
string Shader::getFragmentFileName() { return fragmentFileName; }
string Shader::getVertexFilePath() { return getPathRoot() + vertexFileName; }
string Shader::getFragmentFilePath() { return getPathRoot() + fragmentFileName; }

unsigned int Shader::getShaderID() { return shaderID; }

// activate the shader
void Shader::activate() {
	glUseProgram(shaderID);
}
// utility uniform functions
void Shader::setBool(const string &name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}
void Shader::setInt(const string &name, int value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
void Shader::setFloat(const string &name, float value) const {
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setVec3(const string &name, glm::vec3 vec) const {
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);
}
void Shader::setVec3(const string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), x, y, z);
}

void Shader::setMat4(const string &name, glm::mat4 mat) const {
	setMat4(name, glm::value_ptr(mat));
}
void Shader::setMat4(const string &name, const GLfloat *value) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::bind(const string & name, GLuint index) const {
	GLuint blockIndex = glGetUniformBlockIndex(shaderID, name.c_str());
	glUniformBlockBinding(shaderID, blockIndex, index);
}

void Shader::read(string& vertexCode, string& fragmentCode) {
	ifstream vShaderFile, fShaderFile;

	string vertexPath = pathRoot + vertexFileName;
	string fragmentPath = pathRoot + fragmentFileName;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
}

void Shader::complie(string vertexCode, string fragmentCode) {
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertex);
	glAttachShader(shaderID, fragment);
	glLinkProgram(shaderID);
	checkCompileErrors(shaderID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
}
