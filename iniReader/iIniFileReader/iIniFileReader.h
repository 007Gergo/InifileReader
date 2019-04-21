#pragma once
namespace INI
{
class iIniFileReader
{
public:
	iIniFileReader() {};
	virtual ~iIniFileReader() {};
	virtual bool load() = 0;
	virtual bool changed() = 0;
	virtual bool hasError() = 0;
	virtual void print() = 0;
};
}

