#pragma once
#include <string>
#include <list>
#include "iIniItem.h"

namespace INI
{
	class iIniSection
	{
	public:
		iIniSection(const std::string & name) {};
		virtual ~iIniSection() {};
		virtual bool add(iIniItem * item) = 0;
		virtual const std::string & getName() const = 0;
		virtual const std::list<iIniItem *> & getIniItems() const = 0;
		virtual iIniItem const * find(const std::string & name) const = 0;
		virtual void print() const = 0;
		virtual bool operator==(const iIniSection& other) const = 0;
		virtual bool operator<(const iIniSection& other) const = 0;
	};
}
