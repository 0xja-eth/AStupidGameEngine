#pragma once

class Camera;

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneManager.h"
#include "EventManager.h"
#include "RenderSystem.h"

#include "ConfigurableComponent.h"

//#include "Skybox.h"

#include <vector>

#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f

/*
const string ViewAttrName = "view";
const string RotateViewAttrName = "rotView";
const string ProjectionAttrName = "projection";
const string ViewPosAttrName = "viewPos";
*/

class Skybox;

class CameraConfigData : public ConfigData {
public:
	void generateUBO();

	void setup(glm::vec3 pos, glm::mat4 rot, glm::mat4 proj);

private:
	GLuint UBO;

	glm::vec3 position;
	glm::mat4 rotation, projection, view;

	vector<Shader*> shaders;

protected:
	virtual void doConfig();
};

/*
 *	ÉãÏñ»ú×é¼þ
 */
class Camera : public ConfigurableComponent {
public:

	static const string BindName;
	static const GLuint BindIndex;

	// Constructor with vectors
	Camera(string name, glm::vec3 up =
		glm::vec3(0.0f, 1.0f, 0.0f));
		//bool mouseMove = false);

	virtual string cType() { return "Camera"; };

	virtual Camera* copy();

	glm::mat4 projection(float width, float height);
	glm::mat4 rotation();

	virtual void addShader(Shader* shader);

	virtual ConfigData* getConfigData();

	Skybox* getSkybox();
	void setSkybox(Skybox* skybox);

	virtual bool isActived();

private:
	//bool mouseMove;

	glm::vec3 front, up, right;
	glm::vec3 worldUp;

	Skybox* skybox = NULL;

	CameraConfigData configData;

	//float movementSpeed;
	//float mouseSensitivity;
	float zoom;

	virtual void doUpdate();

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
	/*
	void updateCameraEvents();

	void updateKeyEvents();
	void updateMoveEvents();

	void updateCameraMove(Movement direction);
	void updateCameraRotate(glm::vec2 offset, GLboolean constrainPitch = true);
	void updateScrollEvents(float yoffset);
	*/
};
