#pragma once
#include "iIniSection.h"

namespace INI
{
	class iChainedIniSection
	{
	public:
		iChainedIniSection(iIniSection * iniSection) {};
		virtual ~iChainedIniSection() {};
		virtual void setNext(iChainedIniSection * to) = 0;
		virtual iChainedIniSection * next() = 0;
		virtual iIniSection * get() = 0;
	};
}