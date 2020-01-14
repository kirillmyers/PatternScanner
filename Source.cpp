#include <windows.h>
#include "PatternScanner.h"

BYTE Cyka[] = { 0x4C,0x8B,0xDC,0x48,0x83,0xEC,0x58,0x49,0x8D,0x43,0xE8,0x49,0x89,0x43,0xE0 };

int main()
{
	PatternScanner Test;

	PatternScanner TestDll("kernelbase.dll");

	uintptr_t Place = TestDll.FindPattern(Cyka, "xxxxxxxxxxxxxxx");

	if (Place)
	{
		MessageBoxA(NULL, "", "we found", 0);
	}

	return 0;
}