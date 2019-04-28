#pragma once
#include <string>

namespace INI
{
	class iIniItem
	{
	public:
		iIniItem(const std::string& line) {};
		virtual ~iIniItem() {};
		virtual void setKey(const std::string& to) = 0;
		virtual void setValue(const std::string& to) = 0;
		virtual const std::string& getKey() const = 0;
		virtual const std::string& getValue() const = 0;
		virtual void print() const = 0;
		virtual bool operator==(const iIniItem& other) const = 0;
		virtual bool operator<(const iIniItem& other) const = 0;
	};
}
