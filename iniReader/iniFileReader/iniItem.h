#pragma once
namespace INI
{
	class iniItem
	{
	public:
		iniItem(const char * const line);
		~iniItem();
		void setKey(const char * const to);
		void setValue(const char * const to);
		bool hasKey();
		bool hasValue();
		const char * const getKey();
		const char * const getValue();
		void print();

	private:
		char* mKey;
		char* mValue;
	};
}
