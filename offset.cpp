#include "client.h"

DWORD retaddress;

BOOL utilsCompareData(const BYTE* pData, const BYTE* bMask, const char* pszMask)
{
	for (; *pszMask; ++pszMask, ++pData, ++bMask)
		if (*pszMask == 'x' && *pData != *bMask)
			return FALSE;
	return (*pszMask) == 0;
}

DWORD utilsFindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, char* pszMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (utilsCompareData((BYTE*)(dwAddress + i), bMask, pszMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

LPVOID DetourCreateType(PBYTE pbTargetFunction, PBYTE pbDetourFunction, INT intSize, INT intType)
{
	DWORD dwProtect;
	PBYTE pbDetour = (PBYTE)malloc(intSize + 5);
	INT i;

	VirtualProtect(pbTargetFunction, intSize, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy(pbDetour, pbTargetFunction, intSize);
	pbDetour += intSize;

	*(BYTE*)(pbDetour + 0) = 0xE9;
	*(DWORD*)(pbDetour + 1) = (DWORD)(pbTargetFunction + intSize - pbDetour) - 5;

	switch (intType)
	{
	case 1:
		*(BYTE*)(pbTargetFunction + 0) = 0xB8;
		*(DWORD*)(pbTargetFunction + 1) = (DWORD)(pbDetourFunction);
		*(WORD*)(pbTargetFunction + 5) = 0xE0FF;
		i = 7;
		break;

	case 2:
		*(WORD*)(pbTargetFunction + 0) = 0xC033;
		*(WORD*)(pbTargetFunction + 2) = 0xC085;
		*(WORD*)(pbTargetFunction + 4) = 0x840F;
		*(DWORD*)(pbTargetFunction + 6) = (DWORD)(pbDetourFunction - pbTargetFunction) - 10;
		i = 10;
		break;

	case 3:
		*(WORD*)(pbTargetFunction + 0) = 0xDB33;
		*(WORD*)(pbTargetFunction + 2) = 0xDB85;
		*(WORD*)(pbTargetFunction + 4) = 0x840F;
		*(DWORD*)(pbTargetFunction + 6) = (DWORD)(pbDetourFunction - pbTargetFunction) - 10;
		i = 10;
		break;

	case 4:
		*(WORD*)(pbTargetFunction + 0) = 0xC933;
		*(WORD*)(pbTargetFunction + 2) = 0xC985;
		*(WORD*)(pbTargetFunction + 4) = 0x840F;
		*(DWORD*)(pbTargetFunction + 6) = (DWORD)(pbDetourFunction - pbTargetFunction) - 10;
		i = 10;
		break;

	case 5:
		*(WORD*)(pbTargetFunction + 0) = 0xD233;
		*(WORD*)(pbTargetFunction + 2) = 0xD285;
		*(WORD*)(pbTargetFunction + 4) = 0x840F;
		*(DWORD*)(pbTargetFunction + 6) = (DWORD)(pbDetourFunction - pbTargetFunction) - 10;
		i = 10;
		break;
	}

	for (; i < intSize; i++)* (BYTE*)(pbTargetFunction + i) = 0x90;

	VirtualProtect(pbTargetFunction, intSize, dwProtect, &dwProtect);

	return (pbDetour - intSize);
}

__declspec(naked)void Gateway2_HUD_Frame(void)
{
	__asm
	{
		call HUD_Frame;
		jmp retaddress;
	}
}
DWORD Frame = (DWORD)& Gateway2_HUD_Frame;
__declspec(naked)void Gateway1_HUD_Frame(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x0C] ;
		mov retaddress, esi;
		push Frame
			pop esi;
		mov dword ptr ss : [esp + 0x0C] , esi;
		pop esi;
		ret;
	}
}

__declspec(naked)void Gateway2_V_CalcRefdef(void)
{
	__asm
	{
		call PostV_CalcRefdef;
		jmp retaddress;
	}
}
DWORD CalcRef = (DWORD)& Gateway2_V_CalcRefdef;
__declspec(naked)void Gateway1_V_CalcRefdef(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x10] ;
		push esi;
		call PreV_CalcRefdef;
		add esp, 4;
		mov esi, dword ptr ss : [esp + 0x0c] ;
		mov retaddress, esi;
		push CalcRef;
		pop esi;
		mov dword ptr ss : [esp + 0x0c] , esi;
		pop esi;
		ret;
	}
}

__declspec(naked)void Gateway2_CL_CreateMove(void)
{
	__asm
	{
		call CL_CreateMove;
		jmp retaddress;
	}
}
DWORD CreateMove = (DWORD)& Gateway2_CL_CreateMove;
__declspec(naked)void Gateway1_CL_CreateMove(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x28] ;
		mov retaddress, esi;
		push CreateMove;
		pop esi;
		mov dword ptr ss : [esp + 0x28] , esi;
		pop esi;
		ret;
	}
}

__declspec(naked)void Gateway2_HUD_Redraw(void)
{
	__asm
	{
		call HUD_Redraw;
		jmp retaddress;
	}
}
DWORD Redraw = (DWORD)& Gateway2_HUD_Redraw;
__declspec(naked)void Gateway1_HUD_Redraw(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x10] ;
		mov retaddress, esi;
		push Redraw
			pop esi;
		mov dword ptr ss : [esp + 0x10] , esi;
		pop esi;
		ret;
	}
}

__declspec(naked)void Gateway2_HUD_PostRunCmd(void)
{
	__asm
	{
		call HUD_PostRunCmd;
		jmp retaddress;
	}
}
DWORD PostRunCmd = (DWORD)& Gateway2_HUD_PostRunCmd;
__declspec(naked)void Gateway1_HUD_PostRunCmd(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x38] ;
		mov retaddress, esi;
		push PostRunCmd
			pop esi;
		mov dword ptr ss : [esp + 0x38] , esi;
		pop esi;
		ret;
	}
}

__declspec(naked)void Gateway2_HUD_PlayerMove(void)
{
	__asm
	{
		call HUD_PlayerMove;
		jmp retaddress;
	}
}
DWORD PlayerMove = (DWORD)& Gateway2_HUD_PlayerMove;
__declspec(naked)void Gateway1_HUD_PlayerMove(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x10] ;
		mov retaddress, esi;
		push PlayerMove
			pop esi;
		mov dword ptr ss : [esp + 0x10] , esi;
		pop esi;
		ret;
	}
}

extern int AddEntResult;
__declspec(naked)void Gateway2_HUD_AddEntity(void)
{
	__asm
	{
		mov AddEntResult, eax;
		call HUD_AddEntity;
		mov eax, AddEntResult;
		jmp retaddress;
	}
}
DWORD AddEnt = (DWORD)& Gateway2_HUD_AddEntity;
__declspec(naked)void Gateway1_HUD_AddEntity(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x14] ;
		mov retaddress, esi;
		push AddEnt
			pop esi;
		mov dword ptr ss : [esp + 0x14] , esi;
		pop esi;
		ret;
	}
}

extern int KeyEventResult;
__declspec(naked)void Gateway2_HUD_Key_Event(void)
{
	__asm
	{
		mov KeyEventResult, eax;
		call HUD_Key_Event;
		mov eax, KeyEventResult;
		jmp retaddress;
	}
}
DWORD KeyEvent = (DWORD)& Gateway2_HUD_Key_Event;
__declspec(naked)void Gateway1_HUD_Key_Event(void)
{
	__asm
	{
		push esi;
		mov esi, dword ptr ss : [esp + 0x14] ;
		mov retaddress, esi;
		push KeyEvent
			pop esi;
		mov dword ptr ss : [esp + 0x14] , esi;
		pop esi;
		ret;
	}
}

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const unsigned char*) Buff1, (const unsigned char*) Buff2, (unsigned int) Size)
bool __comparemem(const unsigned char* Buff1, const unsigned char* Buff2, unsigned int Size)
{
	for (unsigned int i = 0; i < Size; i++, Buff2++, Buff1++)
	{
		if ((*Buff1 != *Buff2) && (*Buff2 != 0xFF))
			return false;
	}
	return true;
}

#define FindMemoryClone(start, end, clone, clonesize) __findmemoryclone((const unsigned long) start, (const unsigned long) end, (const unsigned long) clone, (unsigned int) clonesize)
void* __findmemoryclone(const unsigned long start, const unsigned long end, const unsigned long clone, unsigned int clonesize)
{
	for (unsigned long ul = (unsigned long)start; (ul + clonesize) < end; ul++)
	{
		if (CompareMemory(ul, clone, clonesize))
			return (void*)ul;
	}
	return NULL;
}

#define FindReference(start, end, address, type) __findreference((const unsigned long) start, (const unsigned long) end, (const unsigned long) address, (const unsigned char) type)
void* __findreference(const unsigned long start, const unsigned long end, const unsigned long address, const unsigned char type)
{
	unsigned char Pattern[5];

	if (type == 1)
	{
		Pattern[0] = 0x68;
		*(unsigned long*)& Pattern[1] = address;
	}

	return FindMemoryClone(start, end, Pattern, sizeof(Pattern) - 1);
}

#define CompareMemory2(Buff1, Buff2, Size) __comparemem2((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
BOOL __comparemem2(const UCHAR* buff1, const UCHAR* buff2, UINT size)
{
	for (UINT i = 0; i < size; i++, buff1++, buff2++)
	{
		if ((*buff1 != *buff2) && (*buff2 != 0xFF))
			return FALSE;
	}
	return TRUE;
}

#define FindMemoryClone2(Start, End, Clone, Size) __findmemoryclone2((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
ULONG __findmemoryclone2(const ULONG start, const ULONG end, const ULONG clone, UINT size)
{
	for (ULONG ul = start; (ul + size) < end; ul++)
	{
		if (CompareMemory2(ul, clone, size))
			return ul;
	}
	return NULL;
}

#define FindReference2(Start, End, Address) __findreference2((const ULONG)Start, (const ULONG)End, (const ULONG)Address)
ULONG __findreference2(const ULONG start, const ULONG end, const ULONG address)
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68;
	*(ULONG*)& Pattern[1] = address;
	return FindMemoryClone2(start, end, Pattern, sizeof(Pattern) - 1);
}

void* OffsetStudio(DWORD Base, DWORD Size)
{
	PCHAR ModelrString = "cl_himodels";

	ModelrString = (char*)FindMemoryClone(Base, Base + Size, ModelrString, strlen(ModelrString));
	if (ModelrString == NULL)
		return NULL;

	void* pInit = (void*)FindReference(Base, Base + Size, ModelrString, 1);
	if (pInit == NULL)
		return NULL;

	for (unsigned char* b = (unsigned char*)pInit; (DWORD)b > ((DWORD)pInit - 0x10); b--)
	{
		if (*(b - 1) == 0x90 && *(b - 2) == 0x90)
			return (void*)((DWORD)FindMemoryClone(Base, Base + Size, &b, sizeof(DWORD)) - sizeof(DWORD));
	}

	return NULL;
}

void* SpeedHackPtr(DWORD Base, DWORD Size)
{
	DWORD Old = NULL;
	PCHAR String = "Texture load: %6.1fms";

	DWORD Address = (DWORD)FindMemoryClone(Base, Base + Size, String, strlen(String));
	void* SpeedPtr = (void*) * (DWORD*)(FindReference2(Base, Base + Size, Address) - 7);

	VirtualProtect(SpeedPtr, sizeof(double), PAGE_READWRITE, &Old);

	return SpeedPtr;
}
