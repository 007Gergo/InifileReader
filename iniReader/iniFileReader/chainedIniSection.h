#pragma once
#include "..\iIniFileReader\iChainedIniSection.h"

namespace INI
{
	class chainedIniSection
		: public iChainedIniSection
	{
	public:
		chainedIniSection(iIniSection * iniSection);
		virtual ~chainedIniSection();
		virtual void setNext(iChainedIniSection * to);
		virtual iChainedIniSection * next();
		virtual iIniSection * get();
	private:
		iIniSection * mIniSection;
		iChainedIniSection * mNext;
	};
}
