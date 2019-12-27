#pragma once
#include <map>

#include "Updatable.h"

using namespace std;

/*
 *	�����Զ������������
 */
class ManagerBase : public Updatable {
public:

	virtual bool isEnabled() const;
	virtual void setEnabled(bool val);

	virtual void setup();

	virtual void debugShow();

private:
	bool enabled = true;

protected:
	virtual void doUpdate();

};

class Scene;
/*
 *	�������Զ���ϵͳ����
 */
class SystemBase : public ManagerBase {
public:
	SystemBase(Scene* scene);

	virtual string name() = 0;

protected:
	Scene* scene;
};