#pragma once
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Component;

using namespace std;

enum AssetStatus {
	Undefined, Defined, Using, Released
};

/*
 *	��Դ����
 */
class Asset {
public:
	Asset(string name, string pathRoot, string fileName) :
		pathRoot(pathRoot), fileName(fileName), name(name) {};
	~Asset();

	virtual string aType() = 0;

	string getPathRoot();
	string getFileName();
	string getFilePath();

	string getName();
	vector<Component*> getUsingComponents();
	AssetStatus getStatus();

	virtual void setup(); // ������Դ��ʹ�õ�ʱ���Զ����壩

	virtual Asset* use(Component* comp); // ʹ����Դ��ֻ�����ʹ�ã�
	virtual bool isUsing(Component* comp); // ��Դ�Ƿ���ʹ�ã�ֻ�����ʹ�ã�
	virtual void unuse(Component* comp); // �����Դʹ��
	virtual bool release(bool force=false); // �ͷ���Դ

protected:
	string pathRoot, fileName, name;
	vector<Component*> usingComps; // ʹ���е����

	AssetStatus status = Undefined; // ��Դ״̬

	vector<Component*>::iterator getUsing(Component* comp);
};

//#include "Component.h"
