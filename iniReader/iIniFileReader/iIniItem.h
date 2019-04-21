#pragma once
namespace INI
{
class iIniItem
{
public:
	iIniItem(const char * const line) {};
	virtual ~iIniItem() {};
	virtual void setKey(const char * const to) = 0;
	virtual void setValue(const char * const to) = 0;
	virtual bool hasKey() = 0;
	virtual bool hasValue() = 0;
	virtual const char * const getKey() = 0;
	virtual const char * const getValue() = 0;
	virtual void print() = 0;
};
}
