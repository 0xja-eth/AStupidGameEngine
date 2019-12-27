#pragma once
#include <vector>
#include <set>

using namespace std;

class ConfigData {
public:
	virtual void addNext(ConfigData* next);
	virtual void addNext(vector<ConfigData*> next);

	virtual void clearNexts();

	void config();

protected:
	set<ConfigData*> next;

	virtual void configChildren();
	virtual void beforeConfig();
	virtual void doConfig();
	virtual void afterConfig();
};

