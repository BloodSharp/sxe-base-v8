#ifndef _USERMSG_
#define _USERMSG_

typedef struct _UserMgsList 
{
	DWORD unknown1, unknown2;
	CHAR msgname[16];
	struct _UserMgsList* next;
	DWORD address;
} UserMgsList, * PUserMgsList;

typedef struct
{
	Vector to;
	DWORD timestamp;
} DeathInfos;
extern deque<DeathInfos> myFrags;

extern pfnUserMsgHook TeamInfoOrg;
extern pfnUserMsgHook ScoreAttribOrg;
extern pfnUserMsgHook SetFOVOrg;
extern pfnUserMsgHook ResetHUDOrg;
extern pfnUserMsgHook DeathMsgOrg;
extern pfnUserMsgHook HealthOrg;

void InitMsg();

#endif