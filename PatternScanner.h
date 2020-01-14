#include <Psapi.h>

class PatternScanner
{
public:

	PatternScanner()
	{
		MODULEINFO mymodule;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(NULL), &mymodule, sizeof(MODULEINFO));
		beginaddress = reinterpret_cast<uintptr_t>(mymodule.lpBaseOfDll);
		endaddress = beginaddress + mymodule.SizeOfImage;
	}

	PatternScanner(const char* ModuleName)
	{
		MODULEINFO mymodule;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(ModuleName), &mymodule, sizeof(MODULEINFO));
		beginaddress = reinterpret_cast<uintptr_t>(mymodule.lpBaseOfDll);
		endaddress = beginaddress + mymodule.SizeOfImage;
	}

	uintptr_t __fastcall FindPattern(BYTE* Pattern, const char* mask)
	{
		for (uintptr_t temp = beginaddress; temp < (endaddress - strlen(mask)); temp++)
		{
			for (int i = 0; i < strlen(mask); i++,temp++)
			{
				if (mask[i] != '?' && memcmp((void*)temp,&Pattern[i],1) != 0)
				{
					temp -= i;
					break;
				}

				if (i == (strlen(mask) - 1) && memcmp((void*)temp, &Pattern[i], 1) == 0)
				{
					return temp - i;
				}
			}
		}

		return 0;
	}

	void __fastcall PatchPlace(uintptr_t Place, BYTE* PatchArray, size_t PatchSize)
	{
		VirtualProtect((void*)Place, 1, PAGE_EXECUTE_READWRITE, &protection);
		memcpy((void*)Place, PatchArray, PatchSize);
		VirtualProtect((void*)Place, 1, protection, &protection);
	}

	void __fastcall PatchPlaceOneValue(uintptr_t Place, int Value, size_t Amount)
	{
		VirtualProtect((void*)Place, 1, PAGE_EXECUTE_READWRITE, &protection);
		memset((void*)Place, Value, Amount);
		VirtualProtect((void*)Place, 1, protection, &protection);
	}
private:
	uintptr_t beginaddress = 0, endaddress = 0;
	DWORD protection = NULL;
};