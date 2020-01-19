#ifndef _AIMBONE_
#define _AIMBONE_

static BYTE SkeletonHitboxMatrix[12][2] =
{
	{0,1},{1,2},{2,3},{2,4},{4,5},{5,6},{6,7},{1,5},{0,6},{3,7},{7,4},{0,3},//Box
};

static BYTE SkeletonChickenMatrix[8][2] =
{
	{5,4},{4,3},{3,6},//right leg
    {2,1},{1,0},{0,6},//left leg
    {6,7},{7,8},//Neck
};

static BYTE SkeletonBoneMatrix[2][28][2] =
{
	{
		{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8},//Back Bone
		{6,10},{10,11},{11,12},{12,13},{13,14},{13,15},{15,16},//Left Arm
		{6,25},{25,26},{26,27},{27,28},{28,29},{28,30},{30,31},//Right Arm
		{48,42},//Waist
		{42,43},{43,44},{44,45},//Left Foot
		{48,49},{49,50},{50,51},//Right Foot
	},

	{
		{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8},//Back Bone
		{6,10},{10,11},{11,12},{12,13},{13,14},{13,15},{15,16},//Left Arm
		{6,24},{24,25},{25,26},{26,27},{27,28},{27,29},{29,30},//Right Arm
		{46,40},//Waist
		{40,41},{41,42},{42,43},//Left Foot
		{46,47},{47,48},{48,49},//Right Foot
	}
};

static BYTE SkeletonBoneMatrixNoModel[7][2] =
{
	{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},
};

static BYTE BoneMatrix0[21] =
{
	{7},{5},{2},{1},{3},{4},{6},{10},{11},{12},{25},{26},{27},{48},{42},{43},{44},{45},{49},{50},{51},
};

static BYTE BoneMatrix1[21] =
{
	{7},{5},{2},{1},{3},{4},{6},{10},{11},{12},{24},{25},{26},{46},{40},{41},{42},{43},{47},{48},{49},
};

static BYTE BoneMatrixNoModel[7] =
{
	{7},{5},{2},{1},{3},{4},{6},//back bone
};

static BYTE Face[4] =
{
	{2},{4},{1},{5},
};

void PopMatrix(struct cl_entity_s* ent);
void DrawSkeletonPlayer();
void DrawSkeletonThirdPerson();
void DrawEntityHitbox();
void ClearIndex(struct ref_params_s* pparams); 

extern int TexKnifeIndex[3];
extern int TexHandsIndex[114];
extern int TexAk47Index[11];
extern int TexAwpIndex[13];
extern int TexM4a1Index[9];
extern int TexDeagleIndex[9];
extern int TexScoutIndex[8];
extern int TexAugIndex[6];
extern int TexC4Index[11];
extern int TexSmokeGrenadeIndex[12];
extern int TexHeGrenadeIndex[12];
extern int TexFlashBangIndex[12];
extern int TexShieldIndex[47];
extern int TexEliteIndex[6];
extern int TexFamasIndex[4];
extern int TexFiveSevenIndex[7];
extern int TexG3SG1Index[11];
extern int TexGalilIndex[4];
extern int TexGlockIndex[11];
extern int TexM3Index[7];
extern int TexM249Index[12];
extern int TexMac10Index[5];
extern int TexMp5Index[10];
extern int TexP90Index[8];
extern int TexP228Index[9];
extern int TexSG550Index[9];
extern int TexSG552Index[8];
extern int TexTMPIndex[11];
extern int TexUMP45Index[6];
extern int TexUspIndex[10];
extern int TexXM1014Index[9];
extern int TexPlayerIndex[8];
extern int TexChickenIndex[1];
extern int TexBackpack1Index[5];
extern int TexBackpack2Index[5];
extern int TexShellIndex[4];
extern int TexKevlarIndex[1];

#endif