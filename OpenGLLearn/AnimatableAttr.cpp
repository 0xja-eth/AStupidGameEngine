#include "AnimatableAttr.h"

template<class T>
AnimatableAttr<T>::AnimatableAttr(T val, float rate, float threshold) :
	cur(val), tar(val), rate(rate), threshold(threshold), time(0) {}
template<>
AnimatableAttr<glm::vec3>::AnimatableAttr(glm::vec3 val, float rate, float threshold) :
	cur(val), tar(val), rate(rate), threshold(threshold), time(0) {}

#include "EventManager.h"

template<class T>
void AnimatableAttr<T>::reset() { cur = tar; time = 0; }
void AnimatableAttr<glm::vec3>::reset() { cur = tar; time = 0; }

template<class T>
void AnimatableAttr<T>::setCurDelta(T val) { tar = cur + val; }
template<class T>
void AnimatableAttr<T>::setTargetDelta(T val) { tar += val; }
template<class T>
void AnimatableAttr<T>::setTo(T val) { tar = val; }

void AnimatableAttr<glm::vec3>::setTo(glm::vec3 val) { tar = val; }
void AnimatableAttr<glm::vec3>::setTargetDelta(glm::vec3 val) { tar += val; }

template<class T>
bool AnimatableAttr<T>::isPlaying() { return cur != tar; }

bool AnimatableAttr<glm::vec3>::isPlaying() { return cur != tar; }

template<class T>
bool AnimatableAttr<T>::isStopping() { return handleDistFunc() < threshold; }
template<class T>
bool AnimatableAttr<T>::isChanged() { return cur != last; }

bool AnimatableAttr<glm::vec3>::isChanged() { return cur != last; }

template<class T>
void AnimatableAttr<T>::update(float deltaTime) {
	if (deltaTime <= 0) deltaTime = EventManager::getDeltaTime();
	last = cur;
	if (!isPlaying()) return;
	if (isStopping()) reset();
	else {
		time += deltaTime;
		cur = handleAniFunc();
	}
}

void AnimatableAttr<glm::vec3>::update(float deltaTime) {
	last = cur;
	if (!isPlaying()) return;
	if (isStopping()) reset();
	else {
		time += deltaTime;
		cur = handleAniFunc();
	}
}

template<class T>
T AnimatableAttr<T>::handleAniFunc() {
	return aniFunc == NULL ? defaultAniFunc() : aniFunc(time, cur, tar, rate);
}
template<class T>
float AnimatableAttr<T>::handleDistFunc() {
	return aniFunc == NULL ? defaultDistFunc() : distFunc(cur, tar);
}

template<class T>
T AnimatableAttr<T>::defaultAniFunc() {
	T delta = (tar - cur); delta *= rate;
	return cur + delta;
}

float AnimatableAttr<int>::defaultDistFunc() {
	return tar - cur;
}
float AnimatableAttr<float>::defaultDistFunc() {
	return tar - cur;
}
float AnimatableAttr<double>::defaultDistFunc() {
	return tar - cur;
}
float AnimatableAttr<glm::vec2>::defaultDistFunc() {
	return glm::distance(cur, tar);
}
float AnimatableAttr<glm::vec3>::defaultDistFunc() {
	return glm::distance(cur, tar);
}
float AnimatableAttr<glm::vec4>::defaultDistFunc() {
	return glm::distance(cur, tar);
}
template<class T>
float AnimatableAttr<T>::defaultDistFunc() {
	return 0;
}
