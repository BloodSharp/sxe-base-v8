#ifndef _CLIENTS_
#define _CLIENTS_

#pragma once

#pragma warning(disable:4005)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4715)
#pragma warning(disable:4819)
#pragma warning(disable:4996)
#pragma warning(disable:4091)
#pragma warning(disable:4305)
#pragma warning(disable:4409)
#pragma warning(disable:4410)
#pragma warning(disable:4806)

using namespace std;

extern char hackdir [256];

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#include <tlhelp32.h>
#include <iostream>
#include <array>
#include <deque>
#define _USE_MATH_DEFINES
#include <math.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "imgui/soil.h"
#include "imgui/gl.h"
#include "imgui/glu.h"

#include "vector/vector.h"
#include "vector/qangle.h"

#include "SDK/engine/wrect.h"
#include "SDK/engine/cl_dll.h"
#include "SDK/engine/cdll_int.h"
#include "SDK/engine/entity_types.h"
#include "SDK/engine/pmtrace.h"
#include "SDK/engine/pm_defs.h"
#include "SDK/engine/pm_movevars.h"
#include "SDK/engine/r_efx.h"
#include "SDK/engine/studio.h"
#include "SDK/engine/studio_event.h"
#include "SDK/engine/triangleapi.h"
#include "SDK/engine/cvardef.h"
#include "SDK/common/com_model.h"
#include "SDK/common/event_api.h"
#include "SDK/common/ref_params.h"
#include "SDK/common/screenfade.h"
#include "SDK/common/net_api.h"
#include "SDK/misc/r_studioint.h"

#include "resources/memory.h"
#include "resources/ade32.h"
#include "resources/defs.h"
#include "resources/gl3winit.h"
#include "resources/stringfinder.h"
#include "resources/engine.h"
#include "resources/parsemsg.h"
#include "resources/textures.h"

#include "opengl.h"
#include "gui.h"
#include "cvar.h"
#include "interpreter.h"
#include "menu.h"
#include "usermsg.h"
#include "soundesp.h"
#include "weapons.h"
#include "player.h"
#include "snapshot.h"
#include "bigboy.h"
#include "esp.h"
#include "color.h"
#include "offset.h"
#include "studio.h"
#include "nospread.h"
#include "norecoil.h"
#include "aimbot.h"
#include "idhook.h"
#include "skeleton.h"
#include "backdrop.h"
#include "chat.h"
#include "console.h"
#include "strafe.h"

typedef int					(*INITIALIZE_FUNCTION)					(struct cl_enginefuncs_s*, int);
typedef void				(*HUD_INIT_FUNCTION)					(void);
typedef int					(*HUD_VIDINIT_FUNCTION)					(void);
typedef int					(*HUD_REDRAW_FUNCTION)					(float, int);
typedef int					(*HUD_UPDATECLIENTDATA_FUNCTION)		(struct client_data_s*, float);
typedef void				(*HUD_RESET_FUNCTION)					(void);
typedef void				(*HUD_CLIENTMOVE_FUNCTION)				(struct playermove_s* ppmove, qboolean server);
typedef void				(*HUD_CLIENTMOVEINIT_FUNCTION)			(struct playermove_s* ppmove);
typedef char				(*HUD_TEXTURETYPE_FUNCTION)				(char* name);
typedef void				(*HUD_IN_ACTIVATEMOUSE_FUNCTION)		(void);
typedef void				(*HUD_IN_DEACTIVATEMOUSE_FUNCTION)		(void);
typedef void				(*HUD_IN_MOUSEEVENT_FUNCTION)			(int mstate);
typedef void				(*HUD_IN_CLEARSTATES_FUNCTION)			(void);
typedef void				(*HUD_IN_ACCUMULATE_FUNCTION)			(void);
typedef void				(*HUD_CL_CREATEMOVE_FUNCTION)			(float frametime, struct usercmd_s* cmd, int active);
typedef int					(*HUD_CL_ISTHIRDPERSON_FUNCTION)		(void);
typedef void				(*HUD_CL_GETCAMERAOFFSETS_FUNCTION)		(float* offs);
typedef struct kbutton_s*   (*HUD_KB_FIND_FUNCTION)					(const char* name);
typedef void				(*HUD_CAMTHINK_FUNCTION)				(void);
typedef void				(*HUD_V_CALCREFDEF_FUNCTION)			(struct ref_params_s* pparams);
typedef int					(*HUD_ADDENTITY_FUNCTION)				(int type, struct cl_entity_s* ent, const char* modelname);
typedef void				(*HUD_CREATEENTITIES_FUNCTION)			(void);
typedef void				(*HUD_DRAWNORMALTRIS_FUNCTION)			(void);
typedef void				(*HUD_DRAWTRANSTRIS_FUNCTION)			(void);
typedef void				(*HUD_STUDIOEVENT_FUNCTION)				(const struct mstudioevent_s* event, const struct cl_entity_s* entity);
typedef void				(*HUD_POSTRUNCMD_FUNCTION)				(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
typedef void				(*HUD_SHUTDOWN_FUNCTION)				(void);
typedef void				(*HUD_TXFERLOCALOVERRIDES_FUNCTION)		(struct entity_state_s* state, const struct clientdata_s* client);
typedef void				(*HUD_PROCESSPLAYERSTATE_FUNCTION)		(struct entity_state_s* dst, const struct entity_state_s* src);
typedef void				(*HUD_TXFERPREDICTIONDATA_FUNCTION)		(struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
typedef void				(*HUD_DEMOREAD_FUNCTION)				(int size, unsigned char* buffer);
typedef int					(*HUD_CONNECTIONLESS_FUNCTION)			(const struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
typedef int					(*HUD_GETHULLBOUNDS_FUNCTION)			(int hullnumber, float* mins, float* maxs);
typedef void				(*HUD_FRAME_FUNCTION)					(double);
typedef int					(*HUD_KEY_EVENT_FUNCTION)				(int eventcode, int keynum, const char* pszCurrentBinding);
typedef void				(*HUD_TEMPENTUPDATE_FUNCTION)			(double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int (*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void (*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
typedef struct cl_entity_s* (*HUD_GETUSERENTITY_FUNCTION)			(int index);
typedef void				(*HUD_VOICESTATUS_FUNCTION)				(int entindex, qboolean bTalking);
typedef void				(*HUD_DIRECTORMESSAGE_FUNCTION)			(int iSize, void* pbuf);
typedef int					(*HUD_STUDIO_INTERFACE_FUNCTION)		(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
typedef void				(*HUD_CHATINPUTPOSITION_FUNCTION)		(int* x, int* y);
typedef int					(*HUD_GETPLAYERTEAM_FUNCTION)			(int iplayer);
typedef void				(*CLIENTFACTORY)						();

typedef struct cl_clientfunc_t
{
	INITIALIZE_FUNCTION								Initialize;
	HUD_INIT_FUNCTION								HUD_Init;
	HUD_VIDINIT_FUNCTION							HUD_VidInit;
	HUD_REDRAW_FUNCTION								HUD_Redraw;
	HUD_UPDATECLIENTDATA_FUNCTION					HUD_UpdateClientData;
	HUD_RESET_FUNCTION								HUD_Reset;
	HUD_CLIENTMOVE_FUNCTION							HUD_PlayerMove;
	HUD_CLIENTMOVEINIT_FUNCTION						HUD_PlayerMoveInit;
	HUD_TEXTURETYPE_FUNCTION						HUD_PlayerMoveTexture;
	HUD_IN_ACTIVATEMOUSE_FUNCTION					IN_ActivateMouse;
	HUD_IN_DEACTIVATEMOUSE_FUNCTION					IN_DeactivateMouse;
	HUD_IN_MOUSEEVENT_FUNCTION						IN_MouseEvent;
	HUD_IN_CLEARSTATES_FUNCTION						IN_ClearStates;
	HUD_IN_ACCUMULATE_FUNCTION						IN_Accumulate;
	HUD_CL_CREATEMOVE_FUNCTION						CL_CreateMove;
	HUD_CL_ISTHIRDPERSON_FUNCTION					CL_IsThirdPerson;
	HUD_CL_GETCAMERAOFFSETS_FUNCTION				CL_CameraOffset;
	HUD_KB_FIND_FUNCTION							KB_Find;
	HUD_CAMTHINK_FUNCTION							CAM_Think;
	HUD_V_CALCREFDEF_FUNCTION						V_CalcRefdef;
	HUD_ADDENTITY_FUNCTION							HUD_AddEntity;
	HUD_CREATEENTITIES_FUNCTION						HUD_CreateEntities;
	HUD_DRAWNORMALTRIS_FUNCTION						HUD_DrawNormalTriangles;
	HUD_DRAWTRANSTRIS_FUNCTION						HUD_DrawTransparentTriangles;
	HUD_STUDIOEVENT_FUNCTION						HUD_StudioEvent;
	HUD_POSTRUNCMD_FUNCTION							HUD_PostRunCmd;
	HUD_SHUTDOWN_FUNCTION							HUD_Shutdown;
	HUD_TXFERLOCALOVERRIDES_FUNCTION				HUD_TxferLocalOverrides;
	HUD_PROCESSPLAYERSTATE_FUNCTION					HUD_ProcessPlayerState;
	HUD_TXFERPREDICTIONDATA_FUNCTION				HUD_TxferPredictionData;
	HUD_DEMOREAD_FUNCTION							Demo_ReadBuffer;
	HUD_CONNECTIONLESS_FUNCTION						HUD_ConnectionlessPacket;
	HUD_GETHULLBOUNDS_FUNCTION						HUD_GetHullBounds;
	HUD_FRAME_FUNCTION								HUD_Frame;
	HUD_KEY_EVENT_FUNCTION							HUD_Key_Event;
	HUD_TEMPENTUPDATE_FUNCTION						HUD_TempEntUpdate;
	HUD_GETUSERENTITY_FUNCTION						HUD_GetUserEntity;
	HUD_VOICESTATUS_FUNCTION						HUD_VoiceStatus;
	HUD_DIRECTORMESSAGE_FUNCTION					HUD_DirectorMessage;
	HUD_STUDIO_INTERFACE_FUNCTION					HUD_GetStudioModelInterface;
	HUD_CHATINPUTPOSITION_FUNCTION					HUD_ChatInputPosition;
	HUD_GETPLAYERTEAM_FUNCTION						HUD_GetPlayerTeam;
	CLIENTFACTORY									ClientFactory;
} cl_clientfunc_t;

void HUD_Frame(double time);
void HUD_Redraw ( float x, int y );
void HUD_PlayerMove ( struct playermove_s *ppmove, qboolean server );
void CL_CreateMove ( float frametime, struct usercmd_s *cmd, int active );
void PreV_CalcRefdef ( struct ref_params_s *pparams );
void PostV_CalcRefdef ( struct ref_params_s *pparams );
void HUD_AddEntity ( int type, struct cl_entity_s *ent, const char *modelname );
void HUD_PostRunCmd ( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
int HUD_Key_Event ( int eventcode, int keynum, const char *pszCurrentBinding );
void AtRoundStart(void);
void AdjustSpeed(double x);
int StudioCheckBBox();

extern DWORD HudRedraw;
extern DWORD SpeedPtr;

extern cl_clientfunc_t* g_pClient;
extern cl_enginefuncs_s* g_pEngine;
extern engine_studio_api_s* g_pStudio;
extern StudioModelRenderer_t* g_pStudioModelRenderer;
extern screenfade_t* pScreenFade;

extern cl_clientfunc_t g_Client;
extern cl_enginefuncs_s g_Engine;
extern engine_studio_api_t g_Studio;
extern StudioModelRenderer_t g_StudioModelRenderer;
extern glReadPixels_t gl_ReadPixels;

#endif