#pragma once
#include "..\iIniFileReader\iChainedIniItem.h"

namespace INI
{
	class chainedIniItem
		: public iChainedIniItem
	{
	public:
		chainedIniItem(iIniItem * iniItem);
		virtual ~chainedIniItem();
		virtual void setNext(iChainedIniItem * to);
		virtual iChainedIniItem * next();
		virtual iIniItem * get();
	private:
		iIniItem * mIniItem;
		iChainedIniItem * mNext;
	};
}
