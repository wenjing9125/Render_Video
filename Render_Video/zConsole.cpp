#include "StdAfx.h"
#include "Console.h"

CConsole::CConsole(void)
{
	AllocConsole();  
	int hCrun;      
	hCrun = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);  
	FILE* hFile  = _fdopen(hCrun, "w");  
	// use default stream buffer
	setvbuf(hFile, NULL, _IONBF, 0);  
	*stdout = *hFile;  
	//test 
	//_cprintf("test console by _cprintf/n", 0);
	//std::cout < testconsolebystdoutnspan>
}

CConsole::~CConsole(void)
{
	FreeConsole();  
}
