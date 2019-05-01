#pragma once
#include "..\iIniFileReader\iIniItem.h"

namespace INI
{
	class iniItem
		: public iIniItem
	{
	public:
		iniItem(const std::string& line);
		virtual ~iniItem();
		virtual void setKey(const std::string&  to);
		virtual void setValue(const std::string&  to);
		virtual const std::string& getKey() const;
		virtual const std::string& getValue() const;
		virtual void print() const;
		virtual bool operator==(const iIniItem& other) const;
		virtual bool operator<(const iIniItem& other) const;

	private:
		std::string mKey;
		std::string mValue;
	};
}
