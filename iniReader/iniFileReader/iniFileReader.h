#pragma once
#include "..\iIniFileReader\iIniFileReader.h"

namespace INI
{
	class iniFileReader :
		public iIniFileReader
	{
	public:
		iniFileReader(const std::string & name);
		virtual ~iniFileReader();
		virtual const std::list<iIniSection *> & getSections() const;
		virtual bool load();
		virtual bool reloadIfChanged();
		virtual bool hasError() const;
		virtual void print() const;
		virtual iIniSection * findSection(const std::string & name) const;

	private:
		bool isEmptyLine(const std::string & line);
		bool isComment(const std::string & line);
		bool isSection(const std::string & line);
		bool isItem(const std::string & line);

		void clearIniSections(std::list<iIniSection *> & iniSections);
		const std::string getSectionName(const std::string & line, bool & isOk);
		iIniSection * addSection(const std::string & name);
		bool getFileTimes(FILETIME * ftCreate, FILETIME *ftAccess, FILETIME *ftWrite);
		bool isSameSection(const iIniSection * const oldIniSection, const iIniSection * const newIniSection) const;

		bool mHasError;
		std::string mName;
		std::list<iIniSection *> mIniSections;
		FILETIME mFtCreat;
		FILETIME mFtAccess;
		FILETIME mFtWrite;
	};
}
