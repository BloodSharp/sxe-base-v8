#include "../client.h"

CMemoryTools * pMemoryTools = new CMemoryTools;
#define MakePtr( cast, ptr, addValue )( cast )( ( DWORD )( ptr ) + ( DWORD )( addValue ) )

void CMemoryTools::vPatchBytes(DWORD dwAddress,BYTE *bPatch)
{
	int			iLen = ( int ) strlen( ( const char* ) bPatch ); //can't deal with \x00 s
	DWORD		dwOldProtect=0;
	VirtualProtect((DWORD*)dwAddress,iLen,0x40,&dwOldProtect);
	memcpy((void*)dwAddress,(void*)bPatch,iLen);
	VirtualProtect((DWORD*)dwAddress,iLen,dwOldProtect,&dwOldProtect);
}

bool CMemoryTools::bDataCompare(const BYTE* pData, const BYTE* bMask, const char* pszMask)
{
	for(;*pszMask;++pszMask,++pData,++bMask)
		if(*pszMask=='x' && *pData!=*bMask ) 
			return false;
	return (*pszMask)==0;
}

DWORD CMemoryTools::dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * pszMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,pszMask ) )
			return ( DWORD )( dwAddress + i );
	
	return 0;
}

DWORD	CMemoryTools::dwFindPattern	( HMODULE hModule, BYTE* pbMask, char* szMask )
{
	if ( hModule == NULL || hModule == INVALID_HANDLE_VALUE )
		return NULL;

	PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast< PIMAGE_DOS_HEADER >( hModule );

	if( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return NULL;

	PIMAGE_NT_HEADERS pNTHeader	= MakePtr( PIMAGE_NT_HEADERS, pDosHeader,pDosHeader->e_lfanew );

	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	DWORD dwAddress =	(DWORD) hModule + pNTHeader->OptionalHeader.BaseOfCode;
	DWORD dwSize	=	pNTHeader->OptionalHeader.SizeOfCode;

	return dwFindPattern ( dwAddress, dwSize, pbMask, szMask );
}


DWORD CMemoryTools::FindPattern(char *pattern, int len, DWORD dwStart, DWORD dwLen)
{
	char *m = (char *)dwStart;

	for(; (DWORD)m < (dwStart + dwLen); m++)
		if(!memcmp(m, pattern, len))
			return (DWORD)m;

	return NULL;
}

DWORD CMemoryTools::dwCopyRange(DWORD dwFrom,DWORD dwTo)
{
	DWORD dwOldProtect=0;
	DWORD dwLen		  = dwTo-dwFrom;
	VirtualProtect((DWORD*)dwFrom,dwLen,PAGE_EXECUTE_READWRITE,&dwOldProtect);
	void *pCopy = VirtualAlloc(0,dwLen,0x1000|0x2000,0x40);
	vWrapMemory(dwFrom,(DWORD)pCopy,dwLen); //fix calls/jmps
	VirtualProtect((DWORD*)dwFrom,dwLen,dwOldProtect,&dwOldProtect);
	return (DWORD)pCopy;
}


DWORD CMemoryTools::dwCopyFunction(DWORD dwAddress,DWORD dwMin)
{
	DWORD		dwLen=0;
	
	while(dwLen < dwMin) dwLen += dwOpcodeLength(dwAddress+dwLen);
	
	void *pTrampoline = VirtualAlloc(0,dwLen+5,0x1000|0x2000,0x40);
	vWrapMemory(dwAddress,(DWORD)pTrampoline,dwLen);
	vCreateJMP ((DWORD)pTrampoline+dwLen,dwAddress+dwLen);
	return (DWORD)pTrampoline;	
}

DWORD CMemoryTools::dwRedirectCall(DWORD dwAddress,DWORD dwNewCall)
{
	DWORD dwRetval = dwReconstructJMP(dwAddress);
	vCreateCall(dwAddress,dwNewCall);
	return dwRetval;
}

DWORD CMemoryTools::dwRedirectFunction(DWORD dwAddress,DWORD dwNew,bool boAlgo)
{
	DWORD		dwOldProtect=0;
	DWORD		dwLen=0;
	DWORD       dwMin=(!boAlgo)?5:8;
	VirtualProtect((void*)dwAddress,16,PAGE_EXECUTE_READWRITE,&dwOldProtect);
	DWORD		pTrampoline	   = dwCopyFunction(dwAddress,dwMin);
	if(boAlgo)
	{
		*(WORD*) (dwAddress)   = '\xFF\xF6';	//push esi
	    *(BYTE*) (dwAddress+2) = '\x5E';		//pop  esi
		*(BYTE*) (dwAddress+3) = 0xE9;
		*(DWORD*)(dwAddress+4) = (DWORD)dwNew - (DWORD)dwAddress - 8;
	}
	else
	{
		vCreateJMP(dwAddress,dwNew);
	}
	VirtualProtect((void*)dwAddress,16,dwOldProtect,&dwOldProtect);
	return pTrampoline;
}

DWORD CMemoryTools::dwReconstructJMP(DWORD dwAddress)
{
	DWORD  dwJMP = *(DWORD*)(dwAddress+1);
	dwJMP		+=  dwAddress+5;
	return dwJMP;
}

//todo: fix cond. jmps
void CMemoryTools::vWrapMemory(DWORD dwAddress,DWORD dwNew,DWORD dwMinLen)
{
	DWORD dwLen =0;
	while(dwLen < dwMinLen)
	{
		DWORD dwOpcodeLen = dwOpcodeLength(dwAddress+dwLen);
		memcpy((void*)(dwNew+dwLen),(void*)(dwAddress+dwLen),dwOpcodeLen);
		if( *(BYTE*)(dwAddress+dwLen) == 0xE9 )
			vCreateJMP(dwNew+dwLen,dwReconstructJMP(dwAddress+dwLen));
		if( *(BYTE*)(dwAddress+dwLen) == 0xE8 )
			vCreateCall(dwNew+dwLen,dwReconstructJMP(dwAddress+dwLen));
		 dwLen += dwOpcodeLen;
	}
}


void CMemoryTools::vCreateJMP(DWORD dwAddress,DWORD dwNew)
{
	DWORD		dwOldProtect=0;
	VirtualProtect((DWORD*)dwAddress,5,0x40,&dwOldProtect);
	*(BYTE*) (dwAddress)	= 0xE9;	//jmp
	*(DWORD*)(dwAddress+1)	= dwNew - (dwAddress+5);	//relative offset
	VirtualProtect((DWORD*)dwAddress,5,dwOldProtect,&dwOldProtect);
}

void CMemoryTools::vCreateCall(DWORD dwAddress,DWORD dwNew)
{
	DWORD		dwOldProtect=0;
	VirtualProtect((DWORD*)dwAddress,5,0x40,&dwOldProtect);
	*(BYTE*) (dwAddress)	= 0xE8;	//call
	*(DWORD*)(dwAddress+1)	= dwNew - (dwAddress+5);
	VirtualProtect((DWORD*)dwAddress,5,dwOldProtect,&dwOldProtect);
}

DWORD CMemoryTools::dwWriteProcess (DWORD dwPid, void * pAddr, void * pBuf, DWORD dwLen)
{
	DWORD dwOld, dwWriteLen;
	HANDLE hProcess = OpenProcess (PROCESS_VM_OPERATION|PROCESS_VM_WRITE, FALSE, dwPid);

	if ( !hProcess ){
		return 0;
	}
	if (!VirtualProtectEx (hProcess , pAddr, dwLen, PAGE_READWRITE, &dwOld)
	 || !WriteProcessMemory(hProcess, pAddr, pBuf,  dwLen, &dwWriteLen)
	 || !VirtualProtectEx (hProcess , pAddr, dwLen, dwOld, &dwOld) ){
	
		 dwWriteLen = 0;
	 }
	CloseHandle (hProcess);
    return dwWriteLen;
}


DWORD CMemoryTools::dwReadProcess (DWORD dwPid, void * pAddr, void * pBuf, DWORD dwLen)
{
	DWORD dwOld, dwReadLen;
	HANDLE hProcess = OpenProcess (PROCESS_VM_OPERATION|PROCESS_VM_READ, FALSE, dwPid);

	if ( !hProcess ){
		return 0;
	}
	if (!VirtualProtectEx (hProcess, pAddr, dwLen, PAGE_READONLY, &dwOld)
	 || !ReadProcessMemory(hProcess, pAddr, pBuf, dwLen, &dwReadLen)
	 || !VirtualProtectEx (hProcess, pAddr, dwLen, dwOld, &dwOld) ){
	
		 dwReadLen = 0;
	 }
	CloseHandle (hProcess);
    return dwReadLen;
}

DWORD CMemoryTools::dwGetPID(char *szName)
{
	HANDLE			hProc;
	PROCESSENTRY32  hProcEntry;
	hProcEntry.dwSize = sizeof(PROCESSENTRY32);
	hProc			  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if( hProc == INVALID_HANDLE_VALUE ){
		return 0;
	}
	if( Process32First(hProc, &hProcEntry) ){
		while( Process32Next(hProc, &hProcEntry) ){
			if( !stricmp(hProcEntry.szExeFile,szName) ){
				CloseHandle( hProc );
				return hProcEntry.th32ProcessID;
			}
		}
	}
	CloseHandle( hProc );
	return 0;
}

DWORD CMemoryTools::dwOpcodeLength(DWORD dwAddress)
{
	disasm_struct diza;
	disasm((BYTE*)dwAddress,&diza);
	if ( ( diza.disasm_flag == C_ERROR ) || (diza.disasm_flag & C_BAD) )
		return 1; 
	return diza.disasm_len;
}
bool CMemoryTools::bHookEngFunc( PBYTE pbOldFunc, DWORD dwNewFunc )
{
	int iCounter = 0;

	for( int i = 0; i < 25 ; i++ )
	{
		if( pbOldFunc[i] == 0xFF && pbOldFunc[i + 1] == 0x15 ) // CALL DWORD PTR
		{
			iCounter = i + 2;
			break;
		}
		if( pbOldFunc[i] == 0xC3 && pbOldFunc[i + 1] == 0x90 ) // RETN NOP
		{
			return false;
		}
	}

	if( iCounter == 0 ) // 25 bytes exceeded ... we can assume that we failed
		return false;

	PDWORD pdwAddress = ( PDWORD ) &pbOldFunc[iCounter];
	PDWORD pdwPatchLoc = ( PDWORD )* pdwAddress;

	*pdwPatchLoc = dwNewFunc;
}

CMemoryTools::CMemoryTools()
{
}

CMemoryTools::~CMemoryTools()
{
}
