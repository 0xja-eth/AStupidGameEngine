#pragma once

class Updatable {
public:

	virtual bool isEnabled() const;

	void callBeforeUpdate();
	void callDoUpdate();
	void callAfterUpdate();

protected:
	virtual void beforeUpdate();
	virtual void doUpdate();
	virtual void afterUpdate();
};

