#include "client.h"

PlayerInfoLocal g_Local;
PlayerInfo g_Player[33];
player_extra_info_t g_PlayerExtraInfoList[33];
player_entity_info_t g_PlayerEntityInfoList[1024];

bool IsLocalAlive(cl_entity_s* pLocal)
{
	if ((pLocal->curstate.iuser1 == OBS_NONE) && 
		(g_Local.iTeam == 1 || g_Local.iTeam == 2) && 
		(g_Local.vView_ofs[2] != PM_DEAD_VIEWHEIGHT) && 
		(g_Local.iHealth >= 1))
		return true;
	else
		return false;
}

void UpdateLocalPlayer()
{
	cl_entity_s* pLocal = GetLocalPlayer();

	g_Local.pEnt = g_pEngine->GetMaxClients() && pLocal ? pLocal : NULL;
	g_Local.iIndex = g_Local.pEnt ? g_Local.pEnt->index : NULL;
	g_Local.bAlive = g_Local.pEnt ? IsLocalAlive(g_Local.pEnt) : NULL;
	g_Local.flEdgeDistance = g_Local.pEnt ? EdgeDistance() : NULL;
	g_Local.flDamage = g_Local.pEnt ? Damage() : NULL;
}

bool IsPlayerAlive(int i)
{
	if (g_Player[i].pEnt &&
		g_Player[i].pEnt->player &&
		g_Player[i].pEnt->index != g_Local.iIndex &&
		(g_Player[i].pEnt->index < 33 && g_Player[i].pEnt->index >= 0) &&
		(g_Player[i].iTeam == 1 || g_Player[i].iTeam == 2) &&
		!g_Player[i].pEnt->curstate.mins.IsZero() &&
		!g_Player[i].pEnt->curstate.maxs.IsZero() &&
		!(Cstrike_SequenceInfo[g_Player[i].pEnt->curstate.sequence] == SEQUENCE_DIE) &&
		(g_Player[i].pEnt->model->name != 0 || g_Player[i].pEnt->model->name != "") &&
		g_Player[i].bAliveInScoreTab &&
		(g_Local.pEnt) &&
		!(g_Player[i].pEnt->curstate.messagenum < g_Local.pEnt->curstate.messagenum) &&
		!(g_Local.pEnt->curstate.iuser1 == OBS_IN_EYE && g_Local.pEnt->curstate.iuser2 == g_Player[i].pEnt->index))
		return true;
	else
		return false;
}

void GetPlayerInfo(int i)
{
	pfnGetPlayerInfo(i, &g_Player[i].entinfo);
	if (!g_Player[i].entinfo.name) { g_Player[i].entinfo.name = "\\missing-name\\"; }
	if (!g_Player[i].entinfo.model) { g_Player[i].entinfo.model = "unknown model"; }
}

void UpdatePlayer()
{
	for (unsigned int i = 0; i < 33; ++i)
	{
		if (i != g_Local.iIndex)
		{
			cl_entity_s* ent = GetEntityByIndex(i);

			GetPlayerInfo(i);

			g_Player[i].pEnt = ent;
			g_Player[i].bAlive = g_Player[i].pEnt ? IsPlayerAlive(i) : NULL;
			g_Player[i].vVelocity = g_Player[i].pEnt ? (g_Player[i].pEnt->curstate.origin - g_Player[i].pEnt->prevstate.origin) : NULL;
			g_Player[i].flDist = g_Player[i].pEnt ? (g_Player[i].pEnt->curstate.origin.Distance(g_Local.vOrigin)) : NULL;
			g_Player[i].vOrigin = g_Player[i].pEnt ? (g_Player[i].pEnt->curstate.origin) : NULL;
			g_Player[i].bDucked = g_Player[i].pEnt ? (ent->curstate.usehull? true : false) : NULL;
			g_Player[i].flFrametime = g_Player[i].pEnt ? (g_Player[i].pEnt->curstate.animtime - g_Player[i].pEnt->prevstate.animtime) : NULL;
			if (g_Player[i].flFrametime == NULL)
				g_Player[i].flFrametime = g_Local.flFrametime;
		}
	}
}

void ThirdPerson()
{
	if (cvar.esp_chase_cam && g_Local.bAlive && CheckDrawEngine())
	{
		g_pEngine->pfnGetCvarPointer("chase_active")->value = 1;
		g_pEngine->pfnGetCvarPointer("r_drawviewmodel")->value = 0;
	}
	else
	{
		g_pEngine->pfnGetCvarPointer("chase_active")->value = 0;
		g_pEngine->pfnGetCvarPointer("r_drawviewmodel")->value = 1;
	}
}

char* GetWeaponName(int weaponmodel)
{
	static char weapon[50];
	weapon[0] = 0;

	model_s* mdl = GetModelByIndex(weaponmodel);
	if (!mdl) { return weapon; }

	char* name = mdl->name;  if (!name) { return weapon; }
	int len = strlen(name);  if (len > 48 || len < 10) { return weapon; }

	strcpy(weapon, name + 9); len -= 9;
	if (len > 4)weapon[len - 4] = (char)0;

	return weapon;
}

unsigned int Cstrike_SequenceInfo[] =
{
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, // 0..9   
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 10..19 
	1,	2,	0,	1,	1,	2,	0,	1,	1,	2, // 20..29 
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 30..39 
	1,	2,	0,	1,	2,	0,	1,	2,	0,	1, // 40..49 
	2,	0,	1,	2,	0,	0,	0,	8,	0,	8, // 50..59 
	0, 16,	0, 16,	0,	0,	1,	1,	2,	0, // 60..69 
	1,	1,	2,	0,	1,	0,	1,	0,	1,	2, // 70..79 
	0,	1,	2, 	32, 40, 32, 40, 32, 32, 32, // 80..89
	33, 64, 33, 34, 64, 65, 34, 32, 32, 4, // 90..99
	4,	4,	4,	4,	4,	4,	4,	4,	4,	4, // 100..109
	4                                      	// 110
};

bool ModelGuerilla(int i)
{
	if (strstr(g_Player[i].entinfo.model, "guerilla\0"))
		return true;
	return false;
}

bool ModelArctic(int i)
{
	if (strstr(g_Player[i].entinfo.model, "arctic\0"))
		return true;
	return false;
}

bool ModelLeet(int i)
{
	if (strstr(g_Player[i].entinfo.model, "leet\0"))
		return true;
	return false;
}

bool ModelSas(int i)
{
	if (strstr(g_Player[i].entinfo.model, "sas\0"))
		return true;
	return false;
}

bool ModelUrban(int i)
{
	if (strstr(g_Player[i].entinfo.model, "urban\0"))
		return true;
	return false;
}

bool ModelTerror(int i)
{
	if (strstr(g_Player[i].entinfo.model, "terror\0"))
		return true;
	return false;
}

bool ModelGign(int i)
{
	if (strstr(g_Player[i].entinfo.model, "gign\0"))
		return true;
	return false;
}

bool ModelGsg9(int i)
{
	if (strstr(g_Player[i].entinfo.model, "gsg9\0"))
		return true;
	return false;
}

bool bPathFree(float* pflFrom, float* pflTo)
{
	pmtrace_t pTrace;
	EV_SetTraceHull(2);
	EV_PlayerTrace(pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
	return (bool)(pTrace.fraction >= 1.0f);
}

void RunHLCommands()
{
	static bool run = true;
	if (run)
	{
		g_Engine.pfnClientCmd("hud_fastswitch 1");
		g_Engine.pfnClientCmd("cl_showfps 1");
		g_Engine.pfnClientCmd("unbind ins");
		g_Engine.pfnClientCmd("unbind del");
		g_Engine.pfnClientCmd("unbind alt");
		g_Engine.pfnClientCmd("unbind f12");
		g_Engine.pfnClientCmd("rate 999999");
		g_Engine.pfnClientCmd("cl_updaterate 1000");
		g_Engine.pfnClientCmd("cl_cmdrate 1000");
		g_Engine.pfnClientCmd("cl_rate 9999");
		g_Engine.pfnClientCmd("ex_interp 0.1");
		run = false;
	}
}

bool CheckDrawEngine()
{
	if (DrawVisuals && GetTickCount() - HudRedraw <= 100)
		return true;
	else
		return false;
}

bool CheckDraw()
{
	if (GetTickCount() - HudRedraw <= 100)
		return true;
	else
		return false;
}