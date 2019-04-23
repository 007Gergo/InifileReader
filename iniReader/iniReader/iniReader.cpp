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
	catch (...)
	{
		printf("[ERR] Somthing went wrong.\n");
		exit(-1);
	}
}
