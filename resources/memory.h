#ifndef CPATCH_H
#define CPATCH_H

class CMemoryTools
{

	public: 
			 CMemoryTools();
			~CMemoryTools();
			DWORD dwCopyFunction	(DWORD,DWORD);
			DWORD dwCopyRange		(DWORD,DWORD);
			DWORD dwRedirectFunction(DWORD,DWORD,bool = false);
			DWORD dwWriteProcess	(DWORD, void *,void *,DWORD);
			DWORD dwReadProcess		(DWORD, void *,void *,DWORD);
			DWORD dwRedirectCall	(DWORD,DWORD);
			DWORD dwGetPID			(char *);
			DWORD dwFindPattern		(DWORD,DWORD,BYTE *,char *);
			DWORD dwFindPattern		( HMODULE hModule, BYTE* pbMask, char* szMask );
			void  vCreateJMP		(DWORD,DWORD);		
			void  vCreateCall		(DWORD,DWORD);
			void  vPatchBytes		(DWORD,BYTE *);
			DWORD dwReconstructJMP	(DWORD);
			DWORD FindPattern(char *, int , DWORD , DWORD );

			bool bHookEngFunc( PBYTE pbOldFunc, DWORD dwNewFunc );

	private:
			bool  bDataCompare		(const BYTE*, const BYTE*, const char* );
			void  vWrapMemory		(DWORD,DWORD,DWORD);
			DWORD dwOpcodeLength	(DWORD);
};
extern CMemoryTools *pMemoryTools;
#endif
