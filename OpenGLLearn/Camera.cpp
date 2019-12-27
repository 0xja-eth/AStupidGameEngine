
#include "Camera.h"

#include "MainSceneSystem.h"

void CameraConfigData::generateUBO() {
	glGenBuffers(1, &UBO);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 3*sizeof(glm::mat4)+sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, Camera::BindIndex, UBO);
}

void CameraConfigData::setup(glm::vec3 pos, glm::mat4 rot, glm::mat4 proj) {
	position = pos; rotation = rot; projection = proj;
	view = glm::translate(rotation, -position);
}

void CameraConfigData::doConfig() {
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(rotation));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(position));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

const string Camera::BindName = "Camera";
const GLuint Camera::BindIndex = 0;

Camera::Camera(string name, glm::vec3 up) :
	ConfigurableComponent(name),  worldUp(up), zoom(ZOOM) {
	configData.generateUBO();
}

Camera* Camera::copy() {
	Camera* comp = new Camera(getName(), up);
	comp->skybox = skybox;
	comp->zoom = zoom;
	return (Camera*)Component::copy(comp);
}

glm::mat4 Camera::projection(float width, float height) {
	return glm::perspective(glm::radians(zoom), width / height, 0.1f, DL*2.0f);
}
glm::mat4 Camera::rotation() {
	glm::quat quat = getTransform()->curQuat();
	//glm::mat4 view = glm::translate(glm::mat4(1), -pos);
	glm::mat4 view = glm::mat4_cast(quat);
	/*
	glm::vec3 p(
		glm::dot(glm::vec3(view[0][0], view[0][1], view[0][2]), pos),
		glm::dot(glm::vec3(view[1][0], view[1][1], view[1][2]), pos),
		glm::dot(glm::vec3(view[2][0], view[2][1], view[2][2]), pos)
	);
	*/
	/*
	glm::mat4 translate1 = glm::mat4(1);
	translate1 = glm::translate(translate1, -pos);
	glm::mat4 translate2 = glm::mat4(1);
	translate2 = glm::translate(translate2, pos);
	*/
	// ¼ÆËã view£º
	//	glm::vec3 pos = getTransform()->curPosition();
	//	view = glm::translate(view, -pos);
	return view;
}

void Camera::addShader(Shader* shader) {
	ConfigurableComponent::addShader(shader);
	shader->bind(BindName, BindIndex);
}

ConfigData* Camera::getConfigData() {
	int width = GameManager::ScreenWidth;
	int height = GameManager::ScreenHeight;
	configData.setup(getTransform()->curPosition(),
		rotation(), projection(width, height));
	return &configData;
}

Skybox* Camera::getSkybox() { return skybox; }
void Camera::setSkybox(Skybox* skybox) { this->skybox = skybox; }
/*
void Camera::configShader(Shader* shader) {
	ConfigurableComponent::configShader(shader);

	int width = GameManager::ScreenWidth;
	int height = GameManager::ScreenHeight;

	glm::mat4 view = this->view();
	glm::mat4 rotView = this->view(false);
	glm::mat4 projection = this->projection(width, height);
	glm::vec3 position = getTransform()->curPosition();

	shader->setMat4(ViewAttrName, view);
	shader->setMat4(RotateViewAttrName, rotView);
	shader->setMat4(ProjectionAttrName, projection);
	shader->setVec3(ViewPosAttrName, position);
}
*/
void Camera::doUpdate() {

	//node->getTransform()->debugShow(getName());

	if (getTransform()->isChanged()) updateCameraVectors();
	//if (mouseMove) updateCameraEvents();
}

bool Camera::isActived() {
	return ConfigurableComponent::isActived() &&
		this == SceneManager::Get()->
		currentScene()->getActivedCamera();
}

void Camera::updateCameraVectors() {

	glm::vec3 rotation = getTransform()->relativeRotation();
	float pitch = rotation.x, yaw = rotation.y-90;

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up    = glm::normalize(glm::cross(right, front));
}

/*
void Camera::updateCameraEvents() {
	//updateKeyEvents();
	updateMoveEvents();
}
void Camera::updateKeyEvents() {
	if (EventManager::Get()->isKeyPress(GLFW_KEY_W))
		updateCameraMove(Movement(Forward));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_S))
		updateCameraMove(Movement(Backward));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_A))
		updateCameraMove(Movement(Left));
	if (EventManager::Get()->isKeyPress(GLFW_KEY_D))
		updateCameraMove(Movement(Right));
}
void Camera::updateMoveEvents() {
	glm::vec2 deltaMouse = EventManager::Get()->getDeltaMousePos();
	glm::vec2 scrollOffset = EventManager::Get()->getScrollOffset();

	if (deltaMouse != glm::vec2(0))
		updateCameraRotate(deltaMouse*mouseSensitivity);
	if (scrollOffset != glm::vec2(0))
		updateScrollEvents(scrollOffset.y);
}
void Camera::updateCameraMove(Movement direction) {
	float velocity = movementSpeed * EventManager::Get()->getDeltaTime();
	if (direction == Forward) node->getTransform()->moveDelta(front * velocity);
		//Position += Front * velocity;
	if (direction == Backward) node->getTransform()->moveDelta(-front * velocity);
		//Position -= Front * velocity;
	if (direction == Left) node->getTransform()->moveDelta(-right * velocity);
		//Position -= Right * velocity;
	if (direction == Right) node->getTransform()->moveDelta(right * velocity);
		//Position += Right * velocity;
}
void Camera::updateCameraRotate(glm::vec2 offset, GLboolean constrainPitch) {

	glm::vec3 rotation = node->getTransform()->relativeRotation();
	float pitch = rotation.x, yaw = rotation.y;
	pitch += offset.x; yaw -= offset.y;
	
	if (constrainPitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}
	
	getTransform()->rotateTo(pitch, yaw);
	getTransform()->resetRot();

	updateCameraVectors();
}
void Camera::updateScrollEvents(float offset) {
	if (zoom >= 1.0f && zoom <= 45.0f) zoom -= offset;
	if (zoom <= 1.0f) zoom = 1.0f;
	if (zoom >= 45.0f) zoom = 45.0f;
}
*/