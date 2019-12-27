#include "Transform.h"

#include <glm/gtx/matrix_decompose.hpp> 
/*
 *	变换组件
 */
Transform::Transform(string name, glm::vec3 pos /*= glm::vec3(0)*/,
	glm::vec3 rot /*= glm::vec3(0)*/, glm::vec3 sca /*= glm::vec3(1)*/): 
	Component(name) {
	moveTo(pos); rotateTo(rot); scaleTo(sca); reset();
}

Transform* Transform::copy() {
	Transform* comp = new Transform(getName(), 
		position, rotation, scale);
	return (Transform*)Component::copy(comp);
}

glm::mat4 Transform::curModel() const { return model; }

glm::vec3 Transform::curPosition() const { 
	return realPosition;
}
glm::vec3 Transform::curRotation() const {
	return realRotation;
}
glm::quat Transform::curQuat() const {
	return realQuat;
}
glm::vec3 Transform::curScale() const { 
	return realScale;
}

glm::mat4 Transform::getRotateModel(glm::mat4 model /*= glm::mat4(1)*/) {
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	return model;
}

glm::vec3 Transform::relativePosition() const { return position; }
glm::vec3 Transform::relativeRotation() const { return rotation; }
glm::vec3 Transform::relativeScale() const { return scale; }

void Transform::moveTo(glm::vec3 tar) { position = tar; }
void Transform::moveTo(float x, float y, float z) { moveTo(glm::vec3(x, y, z)); }

void Transform::moveDelta(glm::vec3 delta) { position += delta; }
void Transform::moveDelta(float x, float y, float z) { moveDelta(glm::vec3(x, y, z)); }

void Transform::rotateTo(glm::vec3 tar) { rotation = tar; }
void Transform::rotateTo(float x, float y, float z) { rotateTo(glm::vec3(x, y, z)); }

void Transform::rotateDelta(glm::vec3 delta) { rotation += delta; }
void Transform::rotateDelta(float x, float y, float z) { rotateDelta(glm::vec3(x, y, z)); }

void Transform::scaleTo(glm::vec3 tar) { scale = tar; }
void Transform::scaleTo(float x, float y, float z) { scaleTo(glm::vec3(x, y, z)); }

void Transform::scaleDelta(glm::vec3 delta) { scale += delta; }
void Transform::scaleDelta(float x, float y, float z) { scaleDelta(glm::vec3(x, y, z)); }

void Transform::debugShow(string label) {
	if (label != "") cout << label << " ====================" << endl;
	glm::vec3 pos = curPosition(), rot = curRotation(), sca = curScale();
	cout << "  Position: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	cout << "  Rotation: " << rot.x << ", " << rot.y << ", " << rot.z << endl;
	cout << "  Scale: " << sca.x << ", " << sca.y << ", " << sca.z << endl;
}

void Transform::doUpdate() {
	Component::doUpdate();
	//if (isChanged()) 
		updateModel();
}

void Transform::afterUpdate() {
	updateLast();
}

bool Transform::isPlaying() {
	return false;
}

bool Transform::isChanged() {
	return isPositionChanged() || isRotationChanged() || isScaleChanged();
}
bool Transform::isPositionChanged() {
	Transform* parent = getParentTransform();
	return position != lastPosition ||
		(parent != NULL && parent->isPositionChanged());
}
bool Transform::isRotationChanged() {
	Transform* parent = getParentTransform();
	return rotation != lastRotation ||
		(parent != NULL && parent->isRotationChanged());
}
bool Transform::isScaleChanged() {
	Transform* parent = getParentTransform();
	return scale != lastScale ||
		(parent != NULL && parent->isScaleChanged());
}

void Transform::updateModel() {
	Transform* parent = getParentTransform();
	if (parent != NULL) calcModel(parent->model);
	else calcModel();
}

void Transform::updateLast() {
	if (first) {
		first = false; return;
	}
	lastPosition = position;
	lastRotation = rotation;
	lastScale = scale;
}

void Transform::calcModel(glm::mat4 parent/*=glm::mat4(1)*/) {
	glm::vec3 position = relativePosition();
	glm::vec3 rotation = relativeRotation();
	glm::vec3 scale = relativeScale();
	
	model = parent;
	model = glm::translate(model, position);

	//glm::quat quat = glm::make_quat(rotation);
	model = getRotateModel(model);

	model = glm::scale(model, scale);

	decomposeModel();
}

void Transform::decomposeModel() {
	glm::vec3 skew; glm::vec4 pers;
	glm::decompose(model, realScale, realQuat, realPosition, skew, pers);
	/*if (name == "MainCamera") {
		glm::vec3 pos = relativePosition();
		glm::vec3 rel = relativeRotation();
		cout << "MainCamera =========================================" << endl;
		cout << "  Position: " << pos.x << "," << pos.y << "," << pos.z << endl;
		cout << "  Relative: " << rel.x << "," << rel.y << "," << rel.z << endl;
	}*/
	realRotation = glm::degrees(glm::eulerAngles(realQuat));//+glm::vec3(180);
	/*
	if (name == "MainCamera") {
		cout << "  Quat: " << realQuat.x << "," << realQuat.y << "," << realQuat.z << "," << realQuat.w << endl;
		cout << "  Real: " << realRotation.x << "," << realRotation.y << "," << realRotation.z << endl;
	}*/
}	

void Transform::reset() {
	resetPos(); resetRot(); resetSca();
}
void Transform::resetPos() {}
void Transform::resetRot() {}
void Transform::resetSca() {}

/*
 *	带动画的变换组件
 */

AnimatableTransform::AnimatableTransform(string name /*= ""*/, 
	glm::vec3 pos /*= glm::vec3(0)*/, glm::vec3 rot /*= glm::vec3(0)*/, glm::vec3 sca /*= glm::vec3(1)*/) :
	Transform(name) {
	position = AnimatableAttr<glm::vec3>(pos, positionRate, positionThreshold);
	rotation = AnimatableAttr<glm::vec3>(rot, rotationRate, rotationThreshold);
	scale = AnimatableAttr<glm::vec3>(sca, scaleRate, scaleThreshold);
}

AnimatableTransform* AnimatableTransform::copy() {
	AnimatableTransform* comp = new AnimatableTransform(getName(),
		position.cur, rotation.cur, scale.cur);
	return (AnimatableTransform*)Component::copy(comp);
}

/*
glm::vec3 AnimatableTransform::curPosition() const { return position.cur; }
glm::vec3 AnimatableTransform::curRotation() const { return rotation.cur; }
glm::vec3 AnimatableTransform::curScale() const { return scale.cur; }
*/
// 相对变换
glm::vec3 AnimatableTransform::relativePosition() const { return position.cur; }
glm::vec3 AnimatableTransform::relativeRotation() const { return rotation.cur; }
glm::vec3 AnimatableTransform::relativeScale() const { return scale.cur; }

void AnimatableTransform::moveTo(glm::vec3 tar) { position.setTo(tar); }
void AnimatableTransform::moveDelta(glm::vec3 delta) { position.setTargetDelta(delta); }
void AnimatableTransform::rotateTo(glm::vec3 tar) { rotation.setTo(tar); }
void AnimatableTransform::rotateDelta(glm::vec3 delta) { rotation.setTargetDelta(delta); }
void AnimatableTransform::scaleTo(glm::vec3 tar) { scale.setTo(tar); }
void AnimatableTransform::scaleDelta(glm::vec3 delta) { scale.setTargetDelta(delta); }

void AnimatableTransform::resetPos() { position.reset(); }
void AnimatableTransform::resetRot() { rotation.reset(); }
void AnimatableTransform::resetSca() { scale.reset(); }

void AnimatableTransform::doUpdate() {
	updateAnimations();
	updateTransformProto();
	Transform::doUpdate();
}

bool AnimatableTransform::isPlaying() {
	return position.isPlaying() || rotation.isPlaying() || scale.isPlaying();
}

/*
 *	更新父类变换的属性
 */
void AnimatableTransform::updateTransformProto() {
	Transform::moveTo(relativePosition());
	Transform::rotateTo(relativeRotation());
	Transform::scaleTo(relativeScale());
}

void AnimatableTransform::updateAnimations() {
	position.update(); rotation.update(); scale.update();
}

