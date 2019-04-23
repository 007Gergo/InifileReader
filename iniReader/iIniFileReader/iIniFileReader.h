#pragma once
#include "iChainedIniSection.h"

namespace INI
{
	class iIniFileReader
	{
	public:
		iIniFileReader() {};
		virtual ~iIniFileReader() {};
		virtual iChainedIniSection * getSections() = 0;
		virtual bool load() = 0;
		virtual bool reloadIfChanged() = 0;
		virtual bool hasError() = 0;
		virtual void print() = 0;
		virtual iIniSection * findSection(const char * const name) = 0;
	};
}
