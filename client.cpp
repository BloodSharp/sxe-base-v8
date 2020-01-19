#include "client.h"

cl_clientfunc_t* g_pClient;
cl_enginefuncs_s* g_pEngine;
engine_studio_api_s* g_pStudio;
StudioModelRenderer_t* g_pStudioModelRenderer;
screenfade_t* pScreenFade;

cl_clientfunc_t g_Client;
cl_enginefuncs_s g_Engine;
engine_studio_api_t g_Studio;
StudioModelRenderer_t g_StudioModelRenderer;
glReadPixels_t gl_ReadPixels;

DWORD HudRedraw;
DWORD SpeedPtr;
DWORD dwSpeedptr = 0;

void HUD_Frame(double time)
{
	ThirdPerson();
	return;
}

void HUD_Redraw(float x, int y)
{
	HudRedraw = GetTickCount();
	KeyBinds();
	KzFameCount();
	Snapshot();
	return;
}

void HUD_PlayerMove(struct playermove_s* ppmove, qboolean server)
{
	ONCE_ONLY(PM_InitTextureTypes(ppmove);)

	g_Local.vVelocity = ppmove->velocity;
	g_Local.flFallVelocity = ppmove->flFallVelocity;
	g_Local.flVelocityspeed = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));
	g_Local.flVelocity = ppmove->velocity.Length();
	g_Local.iUsehull = ppmove->usehull;
	g_Local.flVelocity2D = ppmove->velocity.Length2D();
	g_Local.vOrigin = ppmove->origin;
	g_Local.iMovetype = ppmove->movetype;
	g_Local.vView_ofs = ppmove->view_ofs;
	g_Local.vEye = ppmove->origin + ppmove->view_ofs;
	g_Local.iFlags = ppmove->flags;
	g_Local.flWaterlevel = ppmove->waterlevel;
	g_Local.flGravity = ppmove->movevars->gravity;

	//Get Distance to Ground
	{
		Vector vTemp1 = g_Local.vOrigin;
		vTemp1[2] -= 8192;

		pmtrace_t* trace = g_Engine.PM_TraceLine(g_Local.vOrigin, vTemp1, 1, (g_Local.iFlags & FL_DUCKING) ? 1 : 0, -1);

		g_Local.flHeightorigin = abs(trace->endpos.z - g_Local.vOrigin.z);
		g_Local.flGroundangle = acos(trace->plane.normal[2]) / M_PI * 180;

		//Get Distance to Player under me
		{
			pmtrace_t pTrace;

			EV_SetTraceHull((g_Local.iFlags & FL_DUCKING) ? 1 : 0);
			EV_PlayerTrace(g_Local.vOrigin, trace->endpos, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);

			if (pTrace.fraction < 1.0f)
			{
				g_Local.flHeightorigin = abs(pTrace.endpos.z - g_Local.vOrigin.z);

				int i = g_pEngine->pEventAPI->EV_IndexFromTrace(&pTrace);
				if (i > 0 && i < 33)
				{
					float dst = g_Local.vOrigin.z - (((g_Local.iFlags & FL_DUCKING) ? 1 : 0) == 0 ? 32 : 18) - g_Player[i].vOrigin.z - g_Local.flHeightorigin;
					if (dst < 30)
					{
						g_Local.flHeightorigin -= 14.0;
					}
				}
			}
		}
	}
	return;
}

void HUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{
	ItemPreFrame(from, to, cmd, runfuncs, time, random_seed);
	return;
}

int AddEntResult = 1;
void HUD_AddEntity(int type, struct cl_entity_s* ent, const char* modelname)
{
	AddEntResult = 1; 
	cl_entity_s* pLocal = GetLocalPlayer();
	if (pLocal && pLocal->curstate.iuser1 == 4 && pLocal->curstate.iuser2 == ent->index)
		AddEntResult = 0;
	return;
}

int KeyEventResult = 1;
int HUD_Key_Event(int eventcode, int keynum, const char* pszCurrentBinding)
{
	KeyEventResult = 1;
	if (bShowMenu && eventcode)
		KeyEventResult = 0;
	if (cvar.gui_chat)
	{
		if (keys[(int)cvar.gui_chat_key])
		{
			ActivateChat();
			keys[(int)cvar.gui_chat_key] = false;
			KeyEventResult = 0;
		}
		if (keys[(int)cvar.gui_chat_key_team])
		{
			ActivateChatTeam();
			keys[(int)cvar.gui_chat_key_team] = false;
			KeyEventResult = 0;
		}
	}
	return KeyEventResult;
}

void CL_CreateMove(float frametime, struct usercmd_s* cmd, int active)
{
	AdjustSpeed(cvar.wav_speed);
	ListIdHook();
	UpdateAimbot(frametime);
	UpdateWeaponData();
	FastZoom(cmd);
	AimBot(frametime, cmd);
	ContinueFire(cmd);
	ItemPostFrame(cmd);
	Kz(frametime, cmd);
	NoRecoil(cmd);
	NoSpread(cmd);
	AntiAim(cmd);
	CustomFOV();
	return;
}

void PreV_CalcRefdef(struct ref_params_s* pparams)
{
	ClearIndex(pparams);
	g_Local.vStrafeForward = pparams->forward;
	g_Local.iHealthStrafe = pparams->health;
	return;
}

void PostV_CalcRefdef(struct ref_params_s* pparams)
{
	g_Local.vPunchangle = pparams->punchangle;
	g_Local.vForward = pparams->forward;
	V_CalcRefdefRecoil(pparams);
	return;
}

int StudioCheckBBox(void)
{
	if (cvar.rage_active)
		return 1;
	return g_Studio.StudioCheckBBox();
}

void AtRoundStart()
{
	RunHLCommands();
	myFrags.clear();
	mySounds.clear();
	for (unsigned int i = 0; i < 33; i++)
	{
		g_Player[i].sound.timestamp = 0;
		g_Player[i].iHealth = 100;
	}
}

void AdjustSpeed(double x)
{
	if (dwSpeedptr == 0)
	{
		dwSpeedptr = SpeedPtr;
	}
	static double LastSpeed = 1;
	if (x != LastSpeed)
	{
		*(double*)dwSpeedptr = (x * 1000);
		LastSpeed = x;
	}
}