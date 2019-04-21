#pragma once
#include "..\iIniFileReader\iIniItem.h"
namespace INI
{
	class iniItem 
		: public iIniItem
	{
	public:
		iniItem(const char * const line);
		virtual ~iniItem();
		virtual void setKey(const char * const to);
		virtual void setValue(const char * const to);
		virtual bool hasKey();
		virtual bool hasValue();
		virtual const char * const getKey();
		virtual const char * const getValue();
		virtual void print();

	private:
		char* mKey;
		char* mValue;
	};
}
