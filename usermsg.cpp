#include "client.h"

pfnUserMsgHook TeamInfoOrg = NULL;
pfnUserMsgHook ScoreAttribOrg = NULL;
pfnUserMsgHook SetFOVOrg = NULL;
pfnUserMsgHook ResetHUDOrg = NULL;
pfnUserMsgHook DeathMsgOrg = NULL;
pfnUserMsgHook HealthOrg = NULL;
DWORD msgOrg = 0;
deque<DeathInfos> myFrags;

int TeamInfo(const char* pszName, int iSize, void* pbuf) 
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	char* szTeam = READ_STRING();

	if (id >= 0 && id < 33)
	{
		if (!lstrcmpA(szTeam, "TERRORIST"))
		{
			g_Player[id].iTeam = 1;
			if (id == g_Local.iIndex) { g_Local.iTeam = 1; }
		}
		else if (!lstrcmpA(szTeam, "CT"))
		{
			g_Player[id].iTeam = 2;
			if (id == g_Local.iIndex) { g_Local.iTeam = 2; }
		}
		else
		{
			g_Player[id].iTeam = 0;
			if (id == g_Local.iIndex) { g_Local.iTeam = 0; }
		}
	}
	return (*TeamInfoOrg)(pszName, iSize, pbuf);
}

int ScoreAttrib(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	int info = READ_BYTE();
	g_Player[id].vip = (info & (1 << 2));
	g_Player[id].bAliveInScoreTab = !(info & (1 << 0));
	return (*ScoreAttribOrg)(pszName, iSize, pbuf);
}

int SetFOV(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int iFOV = READ_BYTE();
	g_Local.iFOV = iFOV;
	return (*SetFOVOrg)(pszName, iSize, pbuf);
}

int ResetHUD(const char* pszName, int iSize, void* pbuf)
{
	AtRoundStart();
	return (*ResetHUDOrg)(pszName, iSize, pbuf);
}

int DeathMsg(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = READ_BYTE();

	if (killer != victim && killer == g_Local.iIndex && victim > 0 && victim < 33)
		dwReactionTime = GetTickCount();

	if (victim != g_Local.iIndex && victim > 0 && victim < 33)
		g_Player[victim].iHealth = 100;

	if (victim != g_Local.iIndex && killer == g_Local.iIndex && victim > 0 && victim < 33)
	{
		if (cvar.esp_headshot_mark && headshot)
		{
			DeathInfos deathInfos;
			deathInfos.to = g_PlayerExtraInfoList[victim].vBone[7];
			deathInfos.timestamp = GetTickCount();
			myFrags.push_back(deathInfos);
		}
		if (cvar.esp_kill_lightning && CheckDrawEngine())
		{
			for (unsigned int i = 0; i < 5; i++)
			{
				Vector origin;
				Vector direction;

				origin[0] = g_Player[victim].pEnt->origin[0];
				origin[1] = g_Player[victim].pEnt->origin[1];
				origin[2] = g_Player[victim].pEnt->origin[2] + 30;

				direction[0] = g_Player[victim].pEnt->origin[0];
				direction[1] = g_Player[victim].pEnt->origin[1];
				direction[2] = g_Player[victim].pEnt->origin[2] + 6000;

				int beamindex = g_Engine.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
				g_Engine.pEfxAPI->R_BeamPoints(origin, direction, beamindex, 1, 1.f, 1, 1, 0, 0, 0, cvar.color_red, cvar.color_green, cvar.color_blue);
			}
		}
	}

	return (*DeathMsgOrg)(pszName, iSize, pbuf);
}

int Health(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.iHealth = READ_BYTE();
	return (*HealthOrg)(pszName, iSize, pbuf);
}

DWORD GetUserMsgHook(PCHAR name) 
{
	PBYTE address = (PBYTE)msgOrg;
	PUserMgsList uml = *(PUserMgsList*)(*(PDWORD)(((address += 0x1A) + *(PDWORD)(address + 1) + 5) + 0x0D));

	int i = 0;
	while (uml) 
	{
		if (i == 200) 
			break;
		if (!strncmp(uml->msgname, name, 16))
			return uml->address;
		uml = uml->next;
		i++;
	}
	return 0;
}

void InitMsg()
{
	msgOrg = (DWORD)g_Engine.pfnHookUserMsg;

	static bool bApplied = false;
	if (bApplied == false)
	{
		TeamInfoOrg = (pfnUserMsgHook)GetUserMsgHook("TeamInfo\0");
		if (TeamInfoOrg)g_Engine.pfnHookUserMsg("TeamInfo\0", TeamInfo);
		ScoreAttribOrg = (pfnUserMsgHook)GetUserMsgHook("ScoreAttrib\0");
		if (ScoreAttribOrg)g_Engine.pfnHookUserMsg("ScoreAttrib\0", ScoreAttrib);
		SetFOVOrg = (pfnUserMsgHook)GetUserMsgHook("SetFOV\0");
		if (SetFOVOrg)g_Engine.pfnHookUserMsg("SetFOV\0", SetFOV);
		ResetHUDOrg = (pfnUserMsgHook)GetUserMsgHook("ResetHUD\0");
		if (ResetHUDOrg)g_Engine.pfnHookUserMsg("ResetHUD\0", ResetHUD);
		DeathMsgOrg = (pfnUserMsgHook)GetUserMsgHook("DeathMsg\0");
		if (DeathMsgOrg)g_Engine.pfnHookUserMsg("DeathMsg\0", DeathMsg);
		HealthOrg = (pfnUserMsgHook)GetUserMsgHook("Health\0");
		if (HealthOrg)g_Engine.pfnHookUserMsg("Health\0", Health);

		bApplied = true;
	}
}