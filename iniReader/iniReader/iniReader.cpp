#include "pch.h"
#include <stdio.h>
#include <windows.h>
#include "..\iniFileReader\iniFileReader.h"

int main(int argv, char** argc)
{
	try
	{
		if (argv != 2)
		{
			printf("Usage: %s inifile\n", argc[0]);
			exit(-1);
		}
		INI::iIniFileReader * ini = new INI::iniFileReader(argc[1]);
		ini->load();
		ini->print();
		while (!ini->reloadIfChanged())
		{
			Sleep(1000);
		}
		ini->print();
		delete ini;
	}
	catch (std::exception & e)
	{
		std::string errMessage = "[ERR] In main(): ";
		errMessage += e.what();
		printf("%s\n", errMessage.c_str());
		exit(-1);
	}
}
