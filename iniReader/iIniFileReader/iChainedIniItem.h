#pragma once
#include "iIniItem.h"

namespace INI
{
	class iChainedIniItem
	{
	public:
		iChainedIniItem(iIniItem * iniItem) {};
		virtual ~iChainedIniItem() {};
		virtual void setNext(iChainedIniItem * to) = 0;
		virtual iChainedIniItem * next() = 0;
		virtual iIniItem * get() = 0;
	};
}
