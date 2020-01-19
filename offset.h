#ifndef _OFFSET_
#define _OFFSET_

BOOL utilsCompareData(const BYTE* pData, const BYTE* bMask, const char* pszMask);
DWORD utilsFindPattern(DWORD dwAddress, DWORD dwLen, BYTE* bMask, char* pszMask);
LPVOID DetourCreateType(PBYTE pbTargetFunction, PBYTE pbDetourFunction, INT intSize, INT intType);

void* OffsetStudio(DWORD Base, DWORD Size);
void* SpeedHackPtr(DWORD Base, DWORD Size);

void Gateway1_HUD_Frame(void);
void Gateway1_V_CalcRefdef(void);
void Gateway1_CL_CreateMove(void);
void Gateway1_HUD_Redraw(void);
void Gateway1_HUD_PostRunCmd(void);
void Gateway1_HUD_PlayerMove(void);
void Gateway1_HUD_AddEntity(void);
void Gateway1_HUD_Key_Event(void);

#endif