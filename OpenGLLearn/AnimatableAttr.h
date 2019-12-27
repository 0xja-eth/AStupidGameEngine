#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<class T>
struct AnimatableAttr {
	typedef T(*AniFunc)(float time, T cur, T tar, float rate);
	typedef float(*DistFunc)(T cur, T tar);

	AnimatableAttr(T val, float rate, float threshold);

	T cur, tar, last;
	float time, rate, threshold;

	AniFunc aniFunc = NULL;
	DistFunc distFunc = NULL;

	void update(float deltaTime = 0);

	bool isPlaying();
	bool isStopping();
	bool isChanged();

	void setTo(T val);
	void setCurDelta(T val);
	void setTargetDelta(T val);

	void reset();

private:
	T defaultAniFunc();
	float defaultDistFunc();

	T handleAniFunc();
	float handleDistFunc();

};
