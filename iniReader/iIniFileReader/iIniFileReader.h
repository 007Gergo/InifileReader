#pragma once
#include <string>
#include <list>
#include "iIniSection.h"

namespace INI
{
	class iIniFileReader
	{
	public:
		iIniFileReader(const std::string & name) {};
		virtual ~iIniFileReader() {};
		virtual const std::list<iIniSection *> & getSections() const = 0;
		virtual bool load() = 0;
		virtual bool reloadIfChanged() = 0;
		virtual bool hasError() const = 0;
		virtual void print() const = 0;
		virtual iIniSection * findSection(const std::string & name) const = 0;
	};
}
