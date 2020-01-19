#ifndef _PLAYERS_
#define _PLAYERS_

class PlayerInfoLocal
{
public:
	cl_entity_s* pEnt;
	bool bJumped;
	bool bAlive; 
	int iIndex;
	int iFOV;
	int iMovetype;
	int iFlags;
	int iUsehull;
	int iTeam;
	int iHealthStrafe;
	int iHealth;

	Vector vForward;
	Vector vStrafeForward;
	Vector vNoSpreadAngle;
	Vector vNoRecoilAngle;
	Vector vVelocity;
	Vector vT1, vT2;
	Vector vStartjumppos;
	Vector vOrigin;
	Vector vPunchangle;
	Vector vEye;
	Vector vView_ofs;
	Vector vViewAngles;
	float flVelocity;
	float flHeightorigin;
	float flFallVelocity;
	float flVelocityspeed;
	float flJumpdist;
	float flJumpmesstime;
	float flGroundangle;
	float flWaterlevel;
	float flEdgeDistance;
	float flDamage;
	float flVelocity2D;
	float flFrametime;
	float flGravity;
	CBasePlayerWeapon weapon;
};
extern PlayerInfoLocal g_Local;

class PlayerInfo
{
public:
	void Init() 
	{
		entinfo.name = "\\missing-name\\";
		entinfo.model = "missing-model";
	}

	hud_player_info_t entinfo;
	cl_entity_s* pEnt;
	Vector vVelocity;
	Vector vOrigin;
	player_sound_t sound;
	bool bAlive;
	bool bDucked;
	bool bVisible;
	bool bGotPlayer;
	bool bAliveInScoreTab;
	bool vip;
	int iHealth;
	int iTeam;
	float flFrametime;
	float flDist;

	PlayerInfo()
	{
		Init();
	}
};
extern PlayerInfo g_Player[33];

struct player_extra_info_t
{
	Vector vHitboxSkeletonHead[8];
	Vector vHitboxFace[8];
	Vector vHitboxMulti[21][8];
	Vector vBoneMulti[54][8];
	Vector vHitbox[21];
	Vector vBoneSkeleton[54];
	Vector vBone[54];

	bool bHitboxVisible[21];
	bool bHitboxFaceVisible[8];
	bool bBoneVisible[54];
	bool bBonePointsVisible[54][8];
	bool bHitboxPointsVisible[21][8];

	float fHitboxFOV[21];
	float fBoneFOV[54];
};
extern player_extra_info_t g_PlayerExtraInfoList[33];

struct player_entity_info_t
{
	Vector vHitboxChick[9];
	Vector vBoneChick[9];
	Vector vHitboxWeaponBox[8];
	Vector vHitboxChickBox[9][8];
	bool bGotWeapon;
	bool bGotChick;
};
extern player_entity_info_t g_PlayerEntityInfoList[1024];

void UpdateLocalPlayer();
void UpdatePlayer();
void ThirdPerson();
void RunHLCommands();
char* GetWeaponName(int weaponmodel);
unsigned int Cstrike_SequenceInfo[];
bool ModelGuerilla(int i);
bool ModelArctic(int i);
bool ModelLeet(int i);
bool ModelSas(int i);
bool ModelUrban(int i);
bool ModelTerror(int i);
bool ModelGign(int i);
bool ModelGsg9(int i);
bool bPathFree(float* pflFrom, float* pflTo);
bool CheckDrawEngine();
bool CheckDraw();

#endif