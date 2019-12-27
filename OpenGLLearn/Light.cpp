#include "Light.h"

const string Light::BindName = "Lights";
const GLuint Light::BindIndex = 1;

bool Light::initialized = false;
Light* Light::lights[MaxLightCount];

GLuint LightConfigData::UBO = 0;

void LightConfigData::generateUBO() {
	if (UBO > 0) return;

	GLuint len = 16 + 4 * 16;

	glGenBuffers(1, &UBO);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, len*MaxLightCount, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, Light::BindIndex, UBO);
}

void LightConfigData::setup(int index, glm::vec3 a, glm::vec3 d, glm::vec3 s, 
	float c, float l, float q, glm::vec3 pos) {
	this->index = index; ambient = a; diffuse = d; specular = s; 
	constant = c; linear = l; quadratic = q;
	position = pos;
}

void LightConfigData::doConfig() {
	GLuint len = 16 + 4 * 16;
	GLuint start = len * index;

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &test);
	glBufferSubData(GL_UNIFORM_BUFFER, start, sizeof(glm::vec3), &position);

	glBufferSubData(GL_UNIFORM_BUFFER, start + 1 * 12, sizeof(float), &constant);
	glBufferSubData(GL_UNIFORM_BUFFER, start + 1 * 12 + 1 * 4, sizeof(float), &linear);
	glBufferSubData(GL_UNIFORM_BUFFER, start + 1 * 12 + 2 * 4, sizeof(float), &quadratic);

	glBufferSubData(GL_UNIFORM_BUFFER, start + 16 + 1 * 16, sizeof(glm::vec3), &ambient);
	glBufferSubData(GL_UNIFORM_BUFFER, start + 16 + 2 * 16, sizeof(glm::vec3), &diffuse);
	glBufferSubData(GL_UNIFORM_BUFFER, start + 16 + 3 * 16, sizeof(glm::vec3), &specular);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Light::initializeLights() {
	for (int i = 0; i < MaxLightCount; i++)
		lights[i] = NULL;
	initialized = true;
}

int Light::getNextIndex() {
	if (!initialized) initializeLights();
	for (int i = 0; i < MaxLightCount; i++)
		if (lights[i] == NULL) return i;
	return -1;
}

Light::Light(string name, glm::vec3 a, glm::vec3 d, glm::vec3 s, float c, float l, float q) :
	ConfigurableComponent(name), ambient(a), diffuse(d), specular(s),
	constant(c), linear(l), quadratic(q) {
	refreshIndex(); configData.generateUBO();
}

Light::~Light() {
	if (index < 0) return;
	lights[index] = NULL;
}

Light* Light::copy() {
	Light* comp = new Light(getName(), ambient, diffuse, 
		specular, constant, linear, quadratic);
	return (Light*)Component::copy(comp);
}

ConfigData* Light::getConfigData() {
	configData.setup(index, ambient, diffuse, specular, 
		constant, linear, quadratic,
		getTransform()->curPosition());
	return &configData;
}

void Light::addShader(Shader* shader) {
	ConfigurableComponent::addShader(shader);
	shader->bind(BindName, BindIndex);
}

int Light::getIndex() { return index; }

void Light::setIndex(int index) {
	this->index = index;
	if (index < 0) return;
	Light* l = lights[index];
	if (l != NULL) l->setIndex(-1);
	lights[index] = this;
}

void Light::doUpdate() {
	ConfigurableComponent::doUpdate();
	refreshIndex();
}

void Light::refreshIndex() {
	if (index < 0) setIndex(getNextIndex());
}

bool Light::testCapture(Camera* camera) {
	return index >= 0;
}