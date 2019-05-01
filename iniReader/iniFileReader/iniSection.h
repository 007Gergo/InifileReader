#pragma once
#include "..\iIniFileReader\iIniSection.h"

namespace INI
{
	class iniSection
		: public iIniSection
	{
	public:
		iniSection(const std::string & name);
		virtual ~iniSection();
		virtual bool add(iIniItem * item);
		virtual const std::string & getName() const;
		virtual const std::list<iIniItem *> & getIniItems() const;
		virtual iIniItem const * find(const std::string & name) const;
		virtual void print() const;
		virtual bool operator==(const iIniSection& other) const;
		virtual bool operator<(const iIniSection& other) const;

	private:
		std::string mName;
		std::list<iIniItem *> mIniItems;
	};
}
