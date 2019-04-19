#pragma once
namespace INI
{
class iIniFileReader
{
public:
	iIniFileReader() {};
	virtual bool load() = 0;
	virtual bool changed() = 0;
	virtual void print() = 0;
	virtual ~iIniFileReader() {};
};
}

