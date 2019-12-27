
#pragma once

#include "AnimatableAttr.h"

#include "Component.h"

#define POSITION_RATE 0.15
#define ROTATION_RATE 0.15
#define SCALE_RATE 0.15

#define POSITION_THRESHOLD 0.01
#define ROTATION_THRESHOLD 0.01
#define SCALE_THRESHOLD 0.01

/*
 *	变换组件
 */
class Transform : public Component {
public:

	Transform(string name="", glm::vec3 pos = glm::vec3(0),
		glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1));

	virtual string cType() { return "Transform"; };

	virtual Transform* copy();

	virtual glm::mat4 curModel() const;

	virtual glm::vec3 curPosition() const;
	virtual glm::vec3 curRotation() const;
	virtual glm::quat curQuat() const;
	virtual glm::vec3 curScale() const;

	glm::mat4 getRotateModel(glm::mat4 model = glm::mat4(1));

	// 相对变换
	virtual glm::vec3 relativePosition() const;
	virtual glm::vec3 relativeRotation() const;
	virtual glm::vec3 relativeScale() const;

	virtual void moveTo(glm::vec3 tar);
	virtual void moveTo(float x = 0, float y = 0, float z = 0);

	virtual void moveDelta(glm::vec3 delta);
	virtual void moveDelta(float x = 0, float y = 0, float z = 0);

	virtual void rotateTo(glm::vec3 tar);
	virtual void rotateTo(float x = 0, float y = 0, float z = 0);

	virtual void rotateDelta(glm::vec3 delta);
	virtual void rotateDelta(float x = 0, float y = 0, float z = 0);

	virtual void scaleTo(glm::vec3 tar);
	virtual void scaleTo(float x = 0, float y = 0, float z = 0);

	virtual void scaleDelta(glm::vec3 delta);
	virtual void scaleDelta(float x = 0, float y = 0, float z = 0);

	void debugShow(string label = "");

	virtual bool isPlaying();

	bool isChanged();

	bool isPositionChanged();
	bool isRotationChanged();
	bool isScaleChanged();

	// 瞬间移动到目标位置（用于继承）
	void reset();

	virtual void resetPos();
	virtual void resetRot();
	virtual void resetSca();

private:
	bool first = true;

	glm::mat4 model;
	
	glm::vec3 realPosition = glm::vec3(0);
	glm::vec3 realRotation = glm::vec3(0);
	glm::quat realQuat = glm::quat(0, 0, 0, 1);
	glm::vec3 realScale = glm::vec3(1);

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	glm::vec3 lastPosition = glm::vec3(-1);
	glm::vec3 lastRotation = glm::vec3(-1);
	glm::vec3 lastScale = glm::vec3(-1);

	void updateModel();
	void updateLast();

	virtual void calcModel(glm::mat4 parent = glm::mat4(1));
	virtual void decomposeModel();

protected:
	virtual void doUpdate();
	virtual void afterUpdate();
};

/*
 *	带动画的变换组件
 */
class AnimatableTransform : public Transform {
public:
	AnimatableTransform(string name = "", glm::vec3 pos = glm::vec3(0),
		glm::vec3 rot = glm::vec3(0), glm::vec3 sca = glm::vec3(1));

	virtual AnimatableTransform* copy();
	/*
	virtual glm::vec3 curPosition() const;
	virtual glm::vec3 curRotation() const;
	virtual glm::vec3 curScale() const;
	*/
	// 相对变换
	virtual glm::vec3 relativePosition() const;
	virtual glm::vec3 relativeRotation() const;
	virtual glm::vec3 relativeScale() const;

	virtual void moveTo(glm::vec3 tar);
	virtual void moveDelta(glm::vec3 delta);
	virtual void rotateTo(glm::vec3 tar);
	virtual void rotateDelta(glm::vec3 delta);
	virtual void scaleTo(glm::vec3 tar);
	virtual void scaleDelta(glm::vec3 delta);

	// 瞬间移动到目标位置
	virtual void resetPos();
	virtual void resetRot();
	virtual void resetSca();

	virtual bool isPlaying();

private:
	float positionRate = POSITION_RATE, positionThreshold = POSITION_THRESHOLD;
	float rotationRate = ROTATION_RATE, rotationThreshold = ROTATION_THRESHOLD;
	float scaleRate = SCALE_RATE, scaleThreshold = SCALE_THRESHOLD;

	AnimatableAttr<glm::vec3> position = AnimatableAttr<glm::vec3>(
		glm::vec3(0), positionRate, positionThreshold);
	AnimatableAttr<glm::vec3> rotation = AnimatableAttr<glm::vec3>(
		glm::vec3(0), rotationRate, rotationThreshold);
	AnimatableAttr<glm::vec3> scale = AnimatableAttr<glm::vec3>(
		glm::vec3(1), scaleRate, scaleThreshold);

	void updateTransformProto();
	void updateAnimations();

protected:
	virtual void doUpdate();
};

