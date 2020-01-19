#include "client.h"

char hackdir[256] = "";

void InitOffsets()
{
	DWORD Base = (DWORD)GetModuleHandleA(NULL);
	DWORD Size = 0x02116000;

	g_pClient = (cl_clientfunc_t*)(*(DWORD*)(utilsFindPattern((DWORD)Base, Size, (BYTE*)"\x8B\x44\x24\x04\x6A\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x50\xC7\x05", "xxxxxxx????x????xxx") + 7));
	g_pEngine = (cl_enginefuncs_s*)0x01EB72D8;
	g_pStudio = (engine_studio_api_t*) * (DWORD*)((DWORD)g_pClient->HUD_GetStudioModelInterface + 0x30);
	g_pStudioModelRenderer = (StudioModelRenderer_t*)OffsetStudio(Base, Size);
	SpeedPtr = (DWORD)SpeedHackPtr(Base, Size);
	gl_ReadPixels = (glReadPixels_t)DetourCreateType((PBYTE)GetProcAddress(GetModuleHandle("opengl32.dll"), "glReadPixels"), (PBYTE)& m_glReadPixels, 7, Detour1);
}

void InitClient()
{
	static bool bClientActive = false;
	if (!bClientActive)
	{
		DWORD dwClientPtr = 0x01A179F0;
		DWORD dwClientRealPtr;
		__asm
		{
			push eax;
			push edx;
			mov edx, dwClientPtr;
			mov eax, [edx];
			mov dwClientRealPtr, eax;
			pop edx;
			pop eax;
		}
		memcpy(&g_Client, (LPVOID)dwClientRealPtr, sizeof cl_clientfunc_t);

		g_Client.HUD_Frame = (HUD_FRAME_FUNCTION)& Gateway1_HUD_Frame;
		g_Client.HUD_Redraw = (HUD_REDRAW_FUNCTION)& Gateway1_HUD_Redraw;
		g_Client.HUD_PlayerMove = (HUD_CLIENTMOVE_FUNCTION)& Gateway1_HUD_PlayerMove;
		g_Client.CL_CreateMove = (HUD_CL_CREATEMOVE_FUNCTION)& Gateway1_CL_CreateMove;
		g_Client.V_CalcRefdef = (HUD_V_CALCREFDEF_FUNCTION)& Gateway1_V_CalcRefdef;
		g_Client.HUD_AddEntity = (HUD_ADDENTITY_FUNCTION)& Gateway1_HUD_AddEntity;
		g_Client.HUD_PostRunCmd = (HUD_POSTRUNCMD_FUNCTION)& Gateway1_HUD_PostRunCmd;
		g_Client.HUD_Key_Event = (HUD_KEY_EVENT_FUNCTION)& Gateway1_HUD_Key_Event;

		DWORD dwClientNewPtr = (DWORD)& g_Client;

		__asm
		{
			push eax;
			push edx;
			mov edx, dwClientPtr;
			mov eax, dwClientNewPtr;
			mov[edx], eax;
			pop edx;
			pop eax;
		}

		bClientActive = true;
	}
}

void InitEngine()
{
	static bool bEngineActive = false;
	if (!bEngineActive)
	{
		if (g_pEngine->pfnHookUserMsg && g_pEngine->pfnHookEvent)
		{
			memcpy(&g_Engine, g_pEngine, sizeof(cl_enginefunc_t));
			if (g_pStudio->StudioSetupSkin)
			{
				memcpy(&g_Studio, g_pStudio, sizeof(g_Studio));
				memcpy(&g_StudioModelRenderer, g_pStudioModelRenderer, sizeof(StudioModelRenderer_t));
				g_pStudio->StudioCheckBBox = &StudioCheckBBox;
				HookRendererFunction(StudioRenderModel);
				HookRendererFunction(StudioRenderFinal);
				HookRendererFunction(StudioRenderFinal_Hardware);
				HookRendererFunction(StudioDrawPlayer);
			}
			bEngineActive = true;
		}
	}
}

void InstallCheat()
{
	InitOffsets();
	InitClient();
	InitEngine();
	InitInterpreter();
	InitMsg(); 
	InitSound();
	InitOpenGL();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		GetModuleFileName(hModule, hackdir, 255);
		char* pos = hackdir + strlen(hackdir);
		while (pos >= hackdir && *pos != '\\')
			--pos;
		pos[1] = 0;
		
		InstallCheat();
	}
	return TRUE;
}