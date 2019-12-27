#pragma once

class Light;

#include "ConfigurableComponent.h"
#include "ConfigData.h"

#include "Node.h"

using namespace std;

const unsigned int MaxLightCount = 50;
/*
const string LightAttrName = "lights[%d]";

const string LightConstantAttrName = "constant";
const string LightLinearAttrName = "linear";
const string LightQuadraticAttrName = "quadratic";

const string LightAmbientAttrName = "ambient";
const string LightDiffuseAttrName = "diffuse";
const string LightSpecularAttrName = "specular";

const string LightPositionAttrName = "position";
*/
class LightConfigData : public ConfigData {
public:
	void generateUBO();

	void setup(int index, glm::vec3 a, glm::vec3 d, glm::vec3 s, 
		float c, float l, float q, glm::vec3 pos);

private:
	static GLuint UBO;

	int index;
	float constant, linear, quadratic;
	glm::vec3 ambient, diffuse, specular, position;

protected:
	virtual void doConfig();
};

/*
 *	游戏光照组件
 */
class Light : public ConfigurableComponent {
public:

	static const string BindName;
	static const GLuint BindIndex;

	static bool initialized;
	static Light* lights[MaxLightCount];

	static void initializeLights();
	static int getNextIndex();

	Light(string name, glm::vec3 a = glm::vec3(0.6f),
		glm::vec3 d = glm::vec3(0.8f), glm::vec3 s = glm::vec3(1),
		float c = 1, float l = 0.09, float q = 0.032);
	~Light();

	virtual string cType() { return "Light"; };

	virtual Light* copy();

	virtual ConfigData* getConfigData();

	virtual void addShader(Shader* shader);

	int getIndex();
	void setIndex(int index);

	virtual void doUpdate();

private:
	int index = -1;

	float constant, linear, quadratic;
	glm::vec3 ambient, diffuse, specular;

	LightConfigData configData;

	void refreshIndex();

	virtual bool testCapture(Camera* camera);
	//virtual void doCapture();
	//virtual void configShader(Shader* shader);

};