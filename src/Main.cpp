#include <iostream>
#include "MemoryPoll.h"

class SomeObject : UseObjectPool(SomeObject)
{
public:

	SomeObject(int val)
		: m_val(val)
	{
		std::cout << "SomeObject" << std::endl;
	}

	~SomeObject()
	{
		std::cout << "~SomeObject" << std::endl;
	}

	void setVal(int value) { m_val = value; }
	int getVal() { return m_val; }

private:

	int m_val;

};

int main()
{
	// Test of create a object using MemoryPoll
	SomeObject* sObject = new SomeObject(1);
	delete sObject;

	return 0;
}
