#include "client.h"
int TexKnifeIndex[3];
int TexHandsIndex[114];
int TexAk47Index[11];
int TexAwpIndex[13];
int TexM4a1Index[9];
int TexDeagleIndex[9];
int TexScoutIndex[8];
int TexAugIndex[6];
int TexC4Index[11];
int TexSmokeGrenadeIndex[12];
int TexHeGrenadeIndex[12];
int TexFlashBangIndex[12];
int TexShieldIndex[47];
int TexEliteIndex[6];
int TexFamasIndex[4];
int TexFiveSevenIndex[7];
int TexG3SG1Index[11];
int TexGalilIndex[4];
int TexGlockIndex[11];
int TexM3Index[7];
int TexM249Index[12];
int TexMac10Index[5];
int TexMp5Index[10];
int TexP90Index[8];
int TexP228Index[9];
int TexSG550Index[9];
int TexSG552Index[8];
int TexTMPIndex[11];
int TexUMP45Index[6];
int TexUspIndex[10];
int TexXM1014Index[9];
int TexPlayerIndex[8];
int TexChickenIndex[1];
int TexBackpack1Index[5];
int TexBackpack2Index[5];
int TexShellIndex[4];
int TexKevlarIndex[1];

void ClearIndex(struct ref_params_s* pparams)
{
	if (pparams->nextView == 0)
	{
		for (unsigned int i = 0; i < 33; i++)
		{
			g_Player[i].bGotPlayer = false;
		}
		for (unsigned int i = 0; i < 1024; i++)
		{
			g_PlayerEntityInfoList[i].bGotWeapon = false;
			g_PlayerEntityInfoList[i].bGotChick = false;
		}
	}
}

void VectorTransform(Vector in1, float in2[3][4], float* out)
{
	out[0] = in1.Dot(in2[0]) + in2[0][3];
	out[1] = in1.Dot(in2[1]) + in2[1][3];
	out[2] = in1.Dot(in2[2]) + in2[2][3];
}

void GetHitboxBoneBox(int index, struct cl_entity_s* ent, int box, float front, float back, float left, float right, float up, float down, mstudiobbox_t* pHitbox, BoneMatrix_t* pBoneMatrix)
{
	Vector vCubePoints[8], vCubePointsTrans[8];

	vCubePoints[0] = Vector(pHitbox[box].bbmin.x + left, pHitbox[box].bbmin.y + back, pHitbox[box].bbmin.z + down);
	vCubePoints[1] = Vector(pHitbox[box].bbmin.x + left, pHitbox[box].bbmax.y - front, pHitbox[box].bbmin.z + down);
	vCubePoints[2] = Vector(pHitbox[box].bbmax.x - right, pHitbox[box].bbmax.y - front, pHitbox[box].bbmin.z + down);
	vCubePoints[3] = Vector(pHitbox[box].bbmax.x - right, pHitbox[box].bbmin.y + back, pHitbox[box].bbmin.z + down);
	vCubePoints[4] = Vector(pHitbox[box].bbmax.x - right, pHitbox[box].bbmax.y - front, pHitbox[box].bbmax.z - up);
	vCubePoints[5] = Vector(pHitbox[box].bbmin.x + left, pHitbox[box].bbmax.y - front, pHitbox[box].bbmax.z - up);
	vCubePoints[6] = Vector(pHitbox[box].bbmin.x + left, pHitbox[box].bbmin.y + back, pHitbox[box].bbmax.z - up);
	vCubePoints[7] = Vector(pHitbox[box].bbmax.x - right, pHitbox[box].bbmin.y + back, pHitbox[box].bbmax.z - up);

	for (unsigned int x = 0; x < 8; x++)
	{
		VectorTransform(vCubePoints[x], (*pBoneMatrix)[index], vCubePointsTrans[x]);
		g_PlayerExtraInfoList[ent->index].vBoneMulti[index][x] = vCubePointsTrans[x] + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
	}
}

void GetModelIndexByModelName(char* modelname, char* texture, int* array, int index, int type)
{
	if (!array[index])
	{
		char filename[512];
		if (type == 1)sprintf(filename, "models/player/%s/%s.mdl", modelname, modelname);
		if (type == 2)sprintf(filename, "models/shield/%s.mdl", modelname);
		if (type == 3)sprintf(filename, "models/%s.mdl", modelname);

		model_t* pModel = g_Engine.CL_LoadModel(filename, NULL);
		if (pModel)
		{
			studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(pModel);
			mstudiotexture_t* ptexture = (mstudiotexture_t*)((byte*)pStudioHeader + pStudioHeader->textureindex);
			//ConsolePrint("Looking for Model: %s. Texture: %s.", pModel->name, texture);
			for (int i = 0; i < pStudioHeader->numtextures; i++)
			{
				if (strstr(ptexture[i].name, texture) && strstr(pModel->name, modelname))
				{
					array[index] = ptexture[i].index;
					//	ConsolePrint("Found Texture: %s. Index: %d.", ptexture[i].name, ptexture[i].index);
				}
			}
		}
		//else
		//	ConsolePrint("Could not load model: %s. Texture: %s.", modelname, texture);
	}
}

char* Terror[] = { "arctic", "guerilla", "leet", "terror", "w_backpack", };
char* CounterTerror[] = { "gign", "gsg9", "sas", "urban", "w_thighpack", };
char* PlayerModels[] = { "arctic", "gign", "gsg9", "guerilla", "leet", "sas", "terror", "urban", };
char* PlayerSkin[] = { "ARTIC_Working1", "GIGN_DMBASE2", "GSG9_Working1", "GUERILLA_DMBASE", "Arab_dmbase1", "SAS_DMBASE1", "Terrorist_Working1", "SEAL_Working1", };

void PopMatrix(struct cl_entity_s* ent)
{
	for (int i = 0; i < 8; i++)
		GetModelIndexByModelName(PlayerModels[i], PlayerSkin[i], TexPlayerIndex, i, 1);
	for (int i = 0; i < 5; i++)
		GetModelIndexByModelName(Terror[i], "Backpack1", TexBackpack1Index, i, i < 4 ? 1 : 3);
	for (int i = 0; i < 5; i++)
		GetModelIndexByModelName(CounterTerror[i], "Backpack2", TexBackpack2Index, i, i < 4 ? 1 : 3);

	GetModelIndexByModelName("v_ak47", "barrel", TexAk47Index, 0, 3);
	GetModelIndexByModelName("v_ak47", "forearm", TexAk47Index, 1, 3);
	GetModelIndexByModelName("v_ak47", "handle", TexAk47Index, 2, 3);
	GetModelIndexByModelName("v_ak47", "lower_body", TexAk47Index, 3, 3);
	GetModelIndexByModelName("v_ak47", "magazine", TexAk47Index, 4, 3);
	GetModelIndexByModelName("v_ak47", "reticle", TexAk47Index, 5, 3);
	GetModelIndexByModelName("v_ak47", "upper_body", TexAk47Index, 6, 3);
	GetModelIndexByModelName("v_ak47", "wood", TexAk47Index, 7, 3);
	GetModelIndexByModelName("w_ak47", "ak47_skin", TexAk47Index, 8, 3);
	GetModelIndexByModelName("p_ak47", "ak47_skin", TexAk47Index, 9, 3);
	GetModelIndexByModelName("backweapons", "ak47_skin", TexAk47Index, 10, 3);

	GetModelIndexByModelName("w_assault", "kevlar_vest", TexKevlarIndex, 0, 3);

	GetModelIndexByModelName("v_aug", "barrel", TexAugIndex, 0, 3);
	GetModelIndexByModelName("v_aug", "body", TexAugIndex, 1, 3);
	GetModelIndexByModelName("v_aug", "magazine", TexAugIndex, 2, 3);
	GetModelIndexByModelName("w_aug", "w_aug", TexAugIndex, 3, 3);
	GetModelIndexByModelName("p_aug", "w_aug", TexAugIndex, 4, 3);
	GetModelIndexByModelName("backweapons", "w_aug", TexAugIndex, 5, 3);

	GetModelIndexByModelName("v_awp", "barrel", TexAwpIndex, 0, 3);
	GetModelIndexByModelName("v_awp", "base", TexAwpIndex, 1, 3);
	GetModelIndexByModelName("v_awp", "bolt_handle", TexAwpIndex, 2, 3);
	GetModelIndexByModelName("v_awp", "buttstock", TexAwpIndex, 3, 3);
	GetModelIndexByModelName("v_awp", "ejector_port", TexAwpIndex, 4, 3);
	GetModelIndexByModelName("v_awp", "magazine", TexAwpIndex, 5, 3);
	GetModelIndexByModelName("v_awp", "newparts.bmp", TexAwpIndex, 6, 3);
	GetModelIndexByModelName("v_awp", "newparts2", TexAwpIndex, 7, 3);
	GetModelIndexByModelName("v_awp", "scope.bmp", TexAwpIndex, 8, 3);
	GetModelIndexByModelName("v_awp", "scope_clamps", TexAwpIndex, 9, 3);
	GetModelIndexByModelName("w_awp", "w_awp", TexAwpIndex, 10, 3);
	GetModelIndexByModelName("p_awp", "w_awp", TexAwpIndex, 11, 3);
	GetModelIndexByModelName("backweapons", "w_awp", TexAwpIndex, 12, 3);

	GetModelIndexByModelName("v_c4", "c4base", TexC4Index, 0, 3);
	GetModelIndexByModelName("v_c4", "c4buttons", TexC4Index, 1, 3);
	GetModelIndexByModelName("v_c4", "c4timer", TexC4Index, 2, 3);
	GetModelIndexByModelName("v_c4", "c4wires", TexC4Index, 3, 3);
	GetModelIndexByModelName("v_c4", "cbase_front", TexC4Index, 4, 3);
	GetModelIndexByModelName("p_c4", "c4base", TexC4Index, 5, 3);
	GetModelIndexByModelName("p_c4", "c4timer", TexC4Index, 6, 3);
	GetModelIndexByModelName("p_c4", "c4wires", TexC4Index, 7, 3);
	GetModelIndexByModelName("w_c4", "c4base", TexC4Index, 8, 3);
	GetModelIndexByModelName("w_c4", "c4timer", TexC4Index, 9, 3);
	GetModelIndexByModelName("w_c4", "c4wires", TexC4Index, 10, 3);

	GetModelIndexByModelName("v_deagle", "DE_handle", TexDeagleIndex, 0, 3);
	GetModelIndexByModelName("v_deagle", "DE_slide1", TexDeagleIndex, 1, 3);
	GetModelIndexByModelName("v_deagle", "DE_slide2_eagle", TexDeagleIndex, 2, 3);
	GetModelIndexByModelName("p_deagle", "deserteagle_skin", TexDeagleIndex, 3, 3);
	GetModelIndexByModelName("w_deagle", "deserteagle_skin", TexDeagleIndex, 4, 3);
	GetModelIndexByModelName("p_shield_deagle", "deserteagle_skin", TexDeagleIndex, 5, 2);
	GetModelIndexByModelName("v_shield_deagle", "DE_handle", TexDeagleIndex, 6, 2);
	GetModelIndexByModelName("v_shield_deagle", "DE_slide1", TexDeagleIndex, 7, 2);
	GetModelIndexByModelName("v_shield_deagle", "DE_slide2_eagle", TexDeagleIndex, 8, 2);

	GetModelIndexByModelName("v_elite", "barrel", TexEliteIndex, 0, 3);
	GetModelIndexByModelName("v_elite", "handle", TexEliteIndex, 1, 3);
	GetModelIndexByModelName("v_elite", "magazine", TexEliteIndex, 2, 3);
	GetModelIndexByModelName("v_elite", "slide", TexEliteIndex, 3, 3);
	GetModelIndexByModelName("p_elite", "w_elite", TexEliteIndex, 4, 3);
	GetModelIndexByModelName("w_elite", "w_elite", TexEliteIndex, 5, 3);

	GetModelIndexByModelName("v_famas", "v_famas", TexFamasIndex, 0, 3);
	GetModelIndexByModelName("p_famas", "p_famas", TexFamasIndex, 1, 3);
	GetModelIndexByModelName("w_famas", "p_famas", TexFamasIndex, 2, 3);
	GetModelIndexByModelName("backweapons", "p_famas", TexFamasIndex, 3, 3);

	GetModelIndexByModelName("v_fiveseven", "fs1", TexFiveSevenIndex, 0, 3);
	GetModelIndexByModelName("v_fiveseven", "fs2", TexFiveSevenIndex, 1, 3);
	GetModelIndexByModelName("p_fiveseven", "57_profile", TexFiveSevenIndex, 2, 3);
	GetModelIndexByModelName("w_fiveseven", "57_profile", TexFiveSevenIndex, 3, 3);
	GetModelIndexByModelName("p_shield_fiveseven", "57_profile", TexFiveSevenIndex, 4, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "fs1", TexFiveSevenIndex, 5, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "fs2", TexFiveSevenIndex, 6, 2);

	GetModelIndexByModelName("v_flashbang", "flash_body", TexFlashBangIndex, 0, 3);
	GetModelIndexByModelName("v_flashbang", "flash_spoon", TexFlashBangIndex, 1, 3);
	GetModelIndexByModelName("v_flashbang", "flash_top", TexFlashBangIndex, 2, 3);
	GetModelIndexByModelName("w_flashbang", "f_body", TexFlashBangIndex, 3, 3);
	GetModelIndexByModelName("w_flashbang", "f_top", TexFlashBangIndex, 4, 3);
	GetModelIndexByModelName("p_flashbang", "f_body", TexFlashBangIndex, 5, 3);
	GetModelIndexByModelName("p_flashbang", "f_top", TexFlashBangIndex, 6, 3);
	GetModelIndexByModelName("p_shield_flashbang", "f_body", TexFlashBangIndex, 7, 2);
	GetModelIndexByModelName("p_shield_flashbang", "f_top", TexFlashBangIndex, 8, 2);
	GetModelIndexByModelName("v_shield_flashbang", "flash_body", TexFlashBangIndex, 9, 2);
	GetModelIndexByModelName("v_shield_flashbang", "flash_spoon", TexFlashBangIndex, 10, 2);
	GetModelIndexByModelName("v_shield_flashbang", "flash_top", TexFlashBangIndex, 11, 2);

	GetModelIndexByModelName("v_g3sg1", "barrel", TexG3SG1Index, 0, 3);
	GetModelIndexByModelName("v_g3sg1", "body", TexG3SG1Index, 1, 3);
	GetModelIndexByModelName("v_g3sg1", "buttstock", TexG3SG1Index, 2, 3);
	GetModelIndexByModelName("v_g3sg1", "forearm", TexG3SG1Index, 3, 3);
	GetModelIndexByModelName("v_g3sg1", "lowerbody", TexG3SG1Index, 4, 3);
	GetModelIndexByModelName("v_g3sg1", "scope.bmp", TexG3SG1Index, 5, 3);
	GetModelIndexByModelName("v_g3sg1", "scope_hold", TexG3SG1Index, 6, 3);
	GetModelIndexByModelName("v_g3sg1", "scope_knob", TexG3SG1Index, 7, 3);
	GetModelIndexByModelName("w_g3sg1", "w_g3sg1", TexG3SG1Index, 8, 3);
	GetModelIndexByModelName("p_g3sg1", "w_g3sg1", TexG3SG1Index, 9, 3);
	GetModelIndexByModelName("backweapons", "w_g3sg1", TexG3SG1Index, 10, 3);

	GetModelIndexByModelName("v_galil", "galil", TexGalilIndex, 0, 3);
	GetModelIndexByModelName("w_galil", "p_galil", TexGalilIndex, 1, 3);
	GetModelIndexByModelName("p_galil", "p_galil", TexGalilIndex, 2, 3);
	GetModelIndexByModelName("backweapons", "p_galil", TexGalilIndex, 3, 3);

	GetModelIndexByModelName("v_glock18", "glock_barrel", TexGlockIndex, 0, 3);
	GetModelIndexByModelName("v_glock18", "glock_base", TexGlockIndex, 1, 3);
	GetModelIndexByModelName("v_glock18", "glock_mag", TexGlockIndex, 2, 3);
	GetModelIndexByModelName("v_glock18", "glock_slide", TexGlockIndex, 3, 3);
	GetModelIndexByModelName("w_glock18", "w_glock", TexGlockIndex, 4, 3);
	GetModelIndexByModelName("p_glock18", "w_glock", TexGlockIndex, 5, 3);
	GetModelIndexByModelName("p_shield_glock18", "w_glock", TexGlockIndex, 6, 2);
	GetModelIndexByModelName("v_shield_glock18", "glock_barrel", TexGlockIndex, 7, 2);
	GetModelIndexByModelName("v_shield_glock18", "glock_base", TexGlockIndex, 8, 2);
	GetModelIndexByModelName("v_shield_glock18", "glock_mag", TexGlockIndex, 9, 2);
	GetModelIndexByModelName("v_shield_glock18", "glock_slide", TexGlockIndex, 10, 2);

	GetModelIndexByModelName("v_hegrenade", "he_body", TexHeGrenadeIndex, 0, 3);
	GetModelIndexByModelName("v_hegrenade", "he_spoon", TexHeGrenadeIndex, 1, 3);
	GetModelIndexByModelName("v_hegrenade", "he_top", TexHeGrenadeIndex, 2, 3);
	GetModelIndexByModelName("w_hegrenade", "f_body", TexHeGrenadeIndex, 3, 3);
	GetModelIndexByModelName("w_hegrenade", "f_top", TexHeGrenadeIndex, 4, 3);
	GetModelIndexByModelName("p_hegrenade", "f_body", TexHeGrenadeIndex, 5, 3);
	GetModelIndexByModelName("p_hegrenade", "f_top", TexHeGrenadeIndex, 6, 3);
	GetModelIndexByModelName("p_shield_hegrenade", "f_body", TexHeGrenadeIndex, 7, 2);
	GetModelIndexByModelName("p_shield_hegrenade", "f_top", TexHeGrenadeIndex, 8, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "he_body", TexHeGrenadeIndex, 9, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "he_spoon", TexHeGrenadeIndex, 10, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "he_top", TexHeGrenadeIndex, 11, 2);

	GetModelIndexByModelName("v_knife", "knifeskin", TexKnifeIndex, 0, 3);
	GetModelIndexByModelName("p_knife", "pknifeskin", TexKnifeIndex, 1, 3);
	GetModelIndexByModelName("v_shield_knife", "knifeskin", TexKnifeIndex, 2, 2);

	GetModelIndexByModelName("v_m3", "barrel", TexM3Index, 0, 3);
	GetModelIndexByModelName("v_m3", "forearm", TexM3Index, 1, 3);
	GetModelIndexByModelName("v_m3", "handle", TexM3Index, 2, 3);
	GetModelIndexByModelName("v_m3", "sights", TexM3Index, 3, 3);
	GetModelIndexByModelName("w_m3", "w_m3super90", TexM3Index, 4, 3);
	GetModelIndexByModelName("p_m3", "w_m3super90", TexM3Index, 5, 3);
	GetModelIndexByModelName("backweapons", "w_m3super90", TexM3Index, 6, 3);

	GetModelIndexByModelName("v_m4a1", "barrel", TexM4a1Index, 0, 3);
	GetModelIndexByModelName("v_m4a1", "buttstock", TexM4a1Index, 1, 3);
	GetModelIndexByModelName("v_m4a1", "handle", TexM4a1Index, 2, 3);
	GetModelIndexByModelName("v_m4a1", "magazine", TexM4a1Index, 3, 3);
	GetModelIndexByModelName("v_m4a1", "receiver", TexM4a1Index, 4, 3);
	GetModelIndexByModelName("v_m4a1", "silencer", TexM4a1Index, 5, 3);
	GetModelIndexByModelName("w_m4a1", "m4a1_skin", TexM4a1Index, 6, 3);
	GetModelIndexByModelName("p_m4a1", "m4a1_skin", TexM4a1Index, 7, 3);
	GetModelIndexByModelName("backweapons", "m4a1_skin", TexM4a1Index, 8, 3);

	GetModelIndexByModelName("v_m249", "ammobox", TexM249Index, 0, 3);
	GetModelIndexByModelName("v_m249", "barrel", TexM249Index, 1, 3);
	GetModelIndexByModelName("v_m249", "body", TexM249Index, 2, 3);
	GetModelIndexByModelName("v_m249", "bullet", TexM249Index, 3, 3);
	GetModelIndexByModelName("v_m249", "buttstock", TexM249Index, 4, 3);
	GetModelIndexByModelName("v_m249", "cover", TexM249Index, 5, 3);
	GetModelIndexByModelName("v_m249", "forearm", TexM249Index, 6, 3);
	GetModelIndexByModelName("v_m249", "handle", TexM249Index, 7, 3);
	GetModelIndexByModelName("v_m249", "sight", TexM249Index, 8, 3);
	GetModelIndexByModelName("w_m249", "w_m249", TexM249Index, 9, 3);
	GetModelIndexByModelName("p_m249", "w_m249", TexM249Index, 10, 3);
	GetModelIndexByModelName("backweapons", "w_m249", TexM249Index, 11, 3);

	GetModelIndexByModelName("v_mac10", "body", TexMac10Index, 0, 3);
	GetModelIndexByModelName("v_mac10", "buttstock", TexMac10Index, 1, 3);
	GetModelIndexByModelName("v_mac10", "handle", TexMac10Index, 2, 3);
	GetModelIndexByModelName("w_mac10", "w_mac10", TexMac10Index, 3, 3);
	GetModelIndexByModelName("p_mac10", "w_mac10", TexMac10Index, 4, 3);

	GetModelIndexByModelName("v_mp5", "barrel", TexMp5Index, 0, 3);
	GetModelIndexByModelName("v_mp5", "buttstock", TexMp5Index, 1, 3);
	GetModelIndexByModelName("v_mp5", "clip", TexMp5Index, 2, 3);
	GetModelIndexByModelName("v_mp5", "forearm", TexMp5Index, 3, 3);
	GetModelIndexByModelName("v_mp5", "handle", TexMp5Index, 4, 3);
	GetModelIndexByModelName("v_mp5", "lowerrec", TexMp5Index, 5, 3);
	GetModelIndexByModelName("v_mp5", "rearsight", TexMp5Index, 6, 3);
	GetModelIndexByModelName("w_mp5", "w_mp5", TexMp5Index, 7, 3);
	GetModelIndexByModelName("p_mp5", "w_mp5", TexMp5Index, 8, 3);
	GetModelIndexByModelName("backweapons", "w_mp5", TexMp5Index, 9, 3);

	GetModelIndexByModelName("v_p90", "buttstock", TexP90Index, 0, 3);
	GetModelIndexByModelName("v_p90", "handle-1", TexP90Index, 1, 3);
	GetModelIndexByModelName("v_p90", "handles", TexP90Index, 2, 3);
	GetModelIndexByModelName("v_p90", "magazine", TexP90Index, 3, 3);
	GetModelIndexByModelName("v_p90", "sights-1", TexP90Index, 4, 3);
	GetModelIndexByModelName("w_p90", "w_p90", TexP90Index, 5, 3);
	GetModelIndexByModelName("p_p90", "w_p90", TexP90Index, 6, 3);
	GetModelIndexByModelName("backweapons", "w_p90", TexP90Index, 7, 3);

	GetModelIndexByModelName("v_p228", "p228_handle", TexP228Index, 0, 3);
	GetModelIndexByModelName("v_p228", "p228_mag", TexP228Index, 1, 3);
	GetModelIndexByModelName("v_p228", "p228_slide", TexP228Index, 2, 3);
	GetModelIndexByModelName("w_p228", "w_p228", TexP228Index, 3, 3);
	GetModelIndexByModelName("p_p228", "w_p228", TexP228Index, 4, 3);
	GetModelIndexByModelName("p_shield_p228", "w_p228", TexP228Index, 5, 2);
	GetModelIndexByModelName("v_shield_p228", "p228_handle", TexP228Index, 6, 2);
	GetModelIndexByModelName("v_shield_p228", "p228_mag", TexP228Index, 7, 2);
	GetModelIndexByModelName("v_shield_p228", "p228_slide", TexP228Index, 8, 2);

	GetModelIndexByModelName("v_scout", "base", TexScoutIndex, 0, 3);
	GetModelIndexByModelName("v_scout", "magazine", TexScoutIndex, 1, 3);
	GetModelIndexByModelName("v_scout", "rail", TexScoutIndex, 2, 3);
	GetModelIndexByModelName("v_scout", "scope.bmp", TexScoutIndex, 3, 3);
	GetModelIndexByModelName("v_scout", "scope_clamps", TexScoutIndex, 4, 3);
	GetModelIndexByModelName("w_scout", "w_scout", TexScoutIndex, 5, 3);
	GetModelIndexByModelName("p_scout", "w_scout", TexScoutIndex, 6, 3);
	GetModelIndexByModelName("backweapons", "w_scout", TexScoutIndex, 7, 3);

	GetModelIndexByModelName("v_sg550", "buttstock", TexSG550Index, 0, 3);
	GetModelIndexByModelName("v_sg550", "forearm", TexSG550Index, 1, 3);
	GetModelIndexByModelName("v_sg550", "handle", TexSG550Index, 2, 3);
	GetModelIndexByModelName("v_sg550", "magazine_transp", TexSG550Index, 3, 3);
	GetModelIndexByModelName("v_sg550", "receiver", TexSG550Index, 4, 3);
	GetModelIndexByModelName("v_sg550", "scope", TexSG550Index, 5, 3);
	GetModelIndexByModelName("w_sg550", "sg550_profile", TexSG550Index, 6, 3);
	GetModelIndexByModelName("p_sg550", "sg550_profile", TexSG550Index, 7, 3);
	GetModelIndexByModelName("backweapons", "sg550_profile", TexSG550Index, 8, 3);

	GetModelIndexByModelName("v_sg552", "acog", TexSG552Index, 0, 3);
	GetModelIndexByModelName("v_sg552", "buttstock", TexSG552Index, 1, 3);
	GetModelIndexByModelName("v_sg552", "forearm", TexSG552Index, 2, 3);
	GetModelIndexByModelName("v_sg552", "handle", TexSG552Index, 3, 3);
	GetModelIndexByModelName("v_sg552", "magazine_transp", TexSG552Index, 4, 3);
	GetModelIndexByModelName("w_sg552", "sg552_skin", TexSG552Index, 5, 3);
	GetModelIndexByModelName("p_sg552", "sg552_skin", TexSG552Index, 6, 3);
	GetModelIndexByModelName("backweapons", "sg552_skin", TexSG552Index, 7, 3);

	GetModelIndexByModelName("w_shield", "shield_back", TexShieldIndex, 0, 3);
	GetModelIndexByModelName("w_shield", "shield_front", TexShieldIndex, 1, 3);
	GetModelIndexByModelName("w_shield", "shield_glass", TexShieldIndex, 2, 3);
	GetModelIndexByModelName("p_shield_deagle", "shield_back", TexShieldIndex, 3, 2);
	GetModelIndexByModelName("p_shield_deagle", "shield_front", TexShieldIndex, 4, 2);
	GetModelIndexByModelName("p_shield_deagle", "shield_glass", TexShieldIndex, 5, 2);
	GetModelIndexByModelName("p_shield_fiveseven", "shield_back", TexShieldIndex, 6, 2);
	GetModelIndexByModelName("p_shield_fiveseven", "shield_front", TexShieldIndex, 7, 2);
	GetModelIndexByModelName("p_shield_fiveseven", "shield_glass", TexShieldIndex, 8, 2);
	GetModelIndexByModelName("p_shield_flashbang", "shield_back", TexShieldIndex, 9, 2);
	GetModelIndexByModelName("p_shield_flashbang", "shield_front", TexShieldIndex, 10, 2);
	GetModelIndexByModelName("p_shield_flashbang", "shield_glass", TexShieldIndex, 11, 2);
	GetModelIndexByModelName("p_shield_glock18", "shield_back", TexShieldIndex, 12, 2);
	GetModelIndexByModelName("p_shield_glock18", "shield_front", TexShieldIndex, 13, 2);
	GetModelIndexByModelName("p_shield_glock18", "shield_glass", TexShieldIndex, 14, 2);
	GetModelIndexByModelName("p_shield_hegrenade", "shield_back", TexShieldIndex, 15, 2);
	GetModelIndexByModelName("p_shield_hegrenade", "shield_front", TexShieldIndex, 16, 2);
	GetModelIndexByModelName("p_shield_hegrenade", "shield_glass", TexShieldIndex, 17, 2);
	GetModelIndexByModelName("p_shield_knife", "shield_back", TexShieldIndex, 18, 2);
	GetModelIndexByModelName("p_shield_knife", "shield_front", TexShieldIndex, 19, 2);
	GetModelIndexByModelName("p_shield_knife", "shield_glass", TexShieldIndex, 20, 2);
	GetModelIndexByModelName("p_shield_p228", "shield_back", TexShieldIndex, 21, 2);
	GetModelIndexByModelName("p_shield_p228", "shield_front", TexShieldIndex, 22, 2);
	GetModelIndexByModelName("p_shield_p228", "shield_glass", TexShieldIndex, 23, 2);
	GetModelIndexByModelName("p_shield_smokegrenade", "shield_back", TexShieldIndex, 24, 2);
	GetModelIndexByModelName("p_shield_smokegrenade", "shield_front", TexShieldIndex, 25, 2);
	GetModelIndexByModelName("p_shield_smokegrenade", "shield_glass", TexShieldIndex, 26, 2);
	GetModelIndexByModelName("p_shield_usp", "shield_back", TexShieldIndex, 27, 2);
	GetModelIndexByModelName("p_shield_usp", "shield_front", TexShieldIndex, 28, 2);
	GetModelIndexByModelName("p_shield_usp", "shield_glass", TexShieldIndex, 29, 2);

	GetModelIndexByModelName("v_shield_deagle", "shield.bmp", TexShieldIndex, 30, 2);
	GetModelIndexByModelName("v_shield_deagle", "shield_glass", TexShieldIndex, 31, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "shield.bmp", TexShieldIndex, 32, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "shield_glass", TexShieldIndex, 33, 2);
	GetModelIndexByModelName("v_shield_flashbang", "shield.bmp", TexShieldIndex, 34, 2);
	GetModelIndexByModelName("v_shield_flashbang", "shield_glass", TexShieldIndex, 35, 2);
	GetModelIndexByModelName("v_shield_glock18", "shield.bmp", TexShieldIndex, 36, 2);
	GetModelIndexByModelName("v_shield_glock18", "shield_glass", TexShieldIndex, 37, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "shield.bmp", TexShieldIndex, 38, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "shield_glass", TexShieldIndex, 39, 2);
	GetModelIndexByModelName("v_shield_knife", "shield.bmp", TexShieldIndex, 40, 2);
	GetModelIndexByModelName("v_shield_p228", "shield.bmp", TexShieldIndex, 41, 2);
	GetModelIndexByModelName("v_shield_p228", "shield_glass", TexShieldIndex, 42, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "shield.bmp", TexShieldIndex, 43, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "shield_glass", TexShieldIndex, 44, 2);
	GetModelIndexByModelName("v_shield_usp", "shield.bmp", TexShieldIndex, 45, 2);
	GetModelIndexByModelName("v_shield_usp", "shield_glass", TexShieldIndex, 46, 2);

	GetModelIndexByModelName("v_smokegrenade", "smoke_body", TexSmokeGrenadeIndex, 0, 3);
	GetModelIndexByModelName("v_smokegrenade", "smoke_spoon", TexSmokeGrenadeIndex, 1, 3);
	GetModelIndexByModelName("v_smokegrenade", "smoke_top", TexSmokeGrenadeIndex, 2, 3);
	GetModelIndexByModelName("p_smokegrenade", "f_body", TexSmokeGrenadeIndex, 3, 3);
	GetModelIndexByModelName("p_smokegrenade", "f_top", TexSmokeGrenadeIndex, 4, 3);
	GetModelIndexByModelName("w_smokegrenade", "f_body", TexSmokeGrenadeIndex, 5, 3);
	GetModelIndexByModelName("w_smokegrenade", "f_top", TexSmokeGrenadeIndex, 6, 3);
	GetModelIndexByModelName("p_shield_smokegrenade", "f_body", TexSmokeGrenadeIndex, 7, 2);
	GetModelIndexByModelName("p_shield_smokegrenade", "f_top", TexSmokeGrenadeIndex, 8, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "smoke_body", TexSmokeGrenadeIndex, 9, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "smoke_spoon", TexSmokeGrenadeIndex, 10, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "smoke_top", TexSmokeGrenadeIndex, 11, 2);

	GetModelIndexByModelName("v_tmp", "barrel", TexTMPIndex, 0, 3);
	GetModelIndexByModelName("v_tmp", "body", TexTMPIndex, 1, 3);
	GetModelIndexByModelName("v_tmp", "ejector", TexTMPIndex, 2, 3);
	GetModelIndexByModelName("v_tmp", "forearm", TexTMPIndex, 3, 3);
	GetModelIndexByModelName("v_tmp", "handle", TexTMPIndex, 4, 3);
	GetModelIndexByModelName("v_tmp", "magazine", TexTMPIndex, 5, 3);
	GetModelIndexByModelName("v_tmp", "misc", TexTMPIndex, 6, 3);
	GetModelIndexByModelName("v_tmp", "silencer", TexTMPIndex, 7, 3);
	GetModelIndexByModelName("v_tmp", "top.bmp", TexTMPIndex, 8, 3);
	GetModelIndexByModelName("w_tmp", "w_tmp", TexTMPIndex, 9, 3);
	GetModelIndexByModelName("p_tmp", "w_tmp", TexTMPIndex, 10, 3);

	GetModelIndexByModelName("v_ump45", "buttstock", TexUMP45Index, 0, 3);
	GetModelIndexByModelName("v_ump45", "handle", TexUMP45Index, 1, 3);
	GetModelIndexByModelName("v_ump45", "receiver", TexUMP45Index, 2, 3);
	GetModelIndexByModelName("w_ump45", "ump_profile", TexUMP45Index, 3, 3);
	GetModelIndexByModelName("p_ump45", "ump_profile", TexUMP45Index, 4, 3);
	GetModelIndexByModelName("backweapons", "ump_profile", TexUMP45Index, 5, 3);

	GetModelIndexByModelName("v_usp", "handle", TexUspIndex, 0, 3);
	GetModelIndexByModelName("v_usp", "mag", TexUspIndex, 1, 3);
	GetModelIndexByModelName("v_usp", "silencer", TexUspIndex, 2, 3);
	GetModelIndexByModelName("v_usp", "slide", TexUspIndex, 3, 3);
	GetModelIndexByModelName("w_usp", "usp_skin", TexUspIndex, 4, 3);
	GetModelIndexByModelName("p_usp", "usp_skin", TexUspIndex, 5, 3);
	GetModelIndexByModelName("p_shield_usp", "usp_skin", TexUspIndex, 6, 2);
	GetModelIndexByModelName("v_shield_usp", "handle", TexUspIndex, 7, 2);
	GetModelIndexByModelName("v_shield_usp", "mag", TexUspIndex, 8, 2);
	GetModelIndexByModelName("v_shield_usp", "slide", TexUspIndex, 9, 2);

	GetModelIndexByModelName("v_xm1014", "barrel", TexXM1014Index, 0, 3);
	GetModelIndexByModelName("v_xm1014", "body", TexXM1014Index, 1, 3);
	GetModelIndexByModelName("v_xm1014", "buttstock", TexXM1014Index, 2, 3);
	GetModelIndexByModelName("v_xm1014", "forearm", TexXM1014Index, 3, 3);
	GetModelIndexByModelName("v_xm1014", "rear_sight", TexXM1014Index, 4, 3);
	GetModelIndexByModelName("v_xm1014", "shell", TexXM1014Index, 5, 3);
	GetModelIndexByModelName("w_xm1014", "xm1014", TexXM1014Index, 6, 3);
	GetModelIndexByModelName("p_xm1014", "xm1014", TexXM1014Index, 7, 3);
	GetModelIndexByModelName("backweapons", "xm1014", TexXM1014Index, 8, 3);

	GetModelIndexByModelName("v_ak47", "view_glove", TexHandsIndex, 0, 3);
	GetModelIndexByModelName("v_aug", "view_glove", TexHandsIndex, 1, 3);
	GetModelIndexByModelName("v_awp", "view_glove", TexHandsIndex, 2, 3);
	GetModelIndexByModelName("v_c4", "view_glove", TexHandsIndex, 3, 3);
	GetModelIndexByModelName("v_deagle", "view_glove", TexHandsIndex, 4, 3);
	GetModelIndexByModelName("v_elite", "view_glove", TexHandsIndex, 5, 3);
	GetModelIndexByModelName("v_famas", "view_glove", TexHandsIndex, 6, 3);
	GetModelIndexByModelName("v_fiveseven", "view_glove", TexHandsIndex, 7, 3);
	GetModelIndexByModelName("v_flashbang", "view_glove", TexHandsIndex, 8, 3);
	GetModelIndexByModelName("v_g3sg1", "view_glove", TexHandsIndex, 9, 3);
	GetModelIndexByModelName("v_galil", "view_glove", TexHandsIndex, 10, 3);
	GetModelIndexByModelName("v_glock18", "view_glove", TexHandsIndex, 11, 3);
	GetModelIndexByModelName("v_hegrenade", "view_glove", TexHandsIndex, 12, 3);
	GetModelIndexByModelName("v_knife", "view_glove", TexHandsIndex, 13, 3);
	GetModelIndexByModelName("v_m3", "view_glove", TexHandsIndex, 14, 3);
	GetModelIndexByModelName("v_m4a1", "view_glove", TexHandsIndex, 15, 3);
	GetModelIndexByModelName("v_m249", "view_glove", TexHandsIndex, 16, 3);
	GetModelIndexByModelName("v_mac10", "view_glove", TexHandsIndex, 17, 3);
	GetModelIndexByModelName("v_mp5", "view_glove", TexHandsIndex, 18, 3);
	GetModelIndexByModelName("v_p90", "view_glove", TexHandsIndex, 19, 3);
	GetModelIndexByModelName("v_p228", "view_glove", TexHandsIndex, 20, 3);
	GetModelIndexByModelName("v_scout", "view_glove", TexHandsIndex, 21, 3);
	GetModelIndexByModelName("v_sg550", "view_glove", TexHandsIndex, 22, 3);
	GetModelIndexByModelName("v_sg552", "view_glove", TexHandsIndex, 23, 3);
	GetModelIndexByModelName("v_smokegrenade", "view_glove", TexHandsIndex, 24, 3);
	GetModelIndexByModelName("v_tmp", "view_glove", TexHandsIndex, 25, 3);
	GetModelIndexByModelName("v_ump45", "view_glove", TexHandsIndex, 26, 3);
	GetModelIndexByModelName("v_usp", "view_glove", TexHandsIndex, 27, 3);
	GetModelIndexByModelName("v_xm1014", "view_glove", TexHandsIndex, 28, 3);
	GetModelIndexByModelName("v_shield_deagle", "view_glove", TexHandsIndex, 29, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "view_glove", TexHandsIndex, 30, 2);
	GetModelIndexByModelName("v_shield_flashbang", "view_glove", TexHandsIndex, 31, 2);
	GetModelIndexByModelName("v_shield_glock18", "view_glove", TexHandsIndex, 32, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "view_glove", TexHandsIndex, 33, 2);
	GetModelIndexByModelName("v_shield_knife", "view_glove", TexHandsIndex, 34, 2);
	GetModelIndexByModelName("v_shield_p228", "view_glove", TexHandsIndex, 35, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "view_glove", TexHandsIndex, 36, 2);
	GetModelIndexByModelName("v_shield_usp", "view_glove", TexHandsIndex, 37, 2);
	GetModelIndexByModelName("v_ak47", "view_finger", TexHandsIndex, 38, 3);
	GetModelIndexByModelName("v_aug", "view_finger", TexHandsIndex, 39, 3);
	GetModelIndexByModelName("v_awp", "view_finger", TexHandsIndex, 40, 3);
	GetModelIndexByModelName("v_c4", "view_finger", TexHandsIndex, 41, 3);
	GetModelIndexByModelName("v_deagle", "view_finger", TexHandsIndex, 42, 3);
	GetModelIndexByModelName("v_elite", "view_finger", TexHandsIndex, 43, 3);
	GetModelIndexByModelName("v_famas", "view_finger", TexHandsIndex, 44, 3);
	GetModelIndexByModelName("v_fiveseven", "view_finger", TexHandsIndex, 45, 3);
	GetModelIndexByModelName("v_flashbang", "view_finger", TexHandsIndex, 46, 3);
	GetModelIndexByModelName("v_g3sg1", "view_finger", TexHandsIndex, 47, 3);
	GetModelIndexByModelName("v_galil", "view_finger", TexHandsIndex, 48, 3);
	GetModelIndexByModelName("v_glock18", "view_finger", TexHandsIndex, 49, 3);
	GetModelIndexByModelName("v_hegrenade", "view_finger", TexHandsIndex, 50, 3);
	GetModelIndexByModelName("v_knife", "view_finger", TexHandsIndex, 51, 3);
	GetModelIndexByModelName("v_m3", "view_finger", TexHandsIndex, 52, 3);
	GetModelIndexByModelName("v_m4a1", "view_finger", TexHandsIndex, 53, 3);
	GetModelIndexByModelName("v_m249", "view_finger", TexHandsIndex, 54, 3);
	GetModelIndexByModelName("v_mac10", "view_finger", TexHandsIndex, 55, 3);
	GetModelIndexByModelName("v_mp5", "view_finger", TexHandsIndex, 56, 3);
	GetModelIndexByModelName("v_p90", "view_finger", TexHandsIndex, 57, 3);
	GetModelIndexByModelName("v_p228", "view_finger", TexHandsIndex, 58, 3);
	GetModelIndexByModelName("v_scout", "view_finger", TexHandsIndex, 59, 3);
	GetModelIndexByModelName("v_sg550", "view_finger", TexHandsIndex, 60, 3);
	GetModelIndexByModelName("v_sg552", "view_finger", TexHandsIndex, 61, 3);
	GetModelIndexByModelName("v_smokegrenade", "view_finger", TexHandsIndex, 62, 3);
	GetModelIndexByModelName("v_tmp", "view_finger", TexHandsIndex, 63, 3);
	GetModelIndexByModelName("v_ump45", "view_finger", TexHandsIndex, 64, 3);
	GetModelIndexByModelName("v_usp", "view_finger", TexHandsIndex, 65, 3);
	GetModelIndexByModelName("v_xm1014", "view_finger", TexHandsIndex, 66, 3);
	GetModelIndexByModelName("v_shield_deagle", "view_finger", TexHandsIndex, 67, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "view_finger", TexHandsIndex, 68, 2);
	GetModelIndexByModelName("v_shield_flashbang", "view_finger", TexHandsIndex, 69, 2);
	GetModelIndexByModelName("v_shield_glock18", "view_finger", TexHandsIndex, 70, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "view_finger", TexHandsIndex, 71, 2);
	GetModelIndexByModelName("v_shield_knife", "view_finger", TexHandsIndex, 72, 2);
	GetModelIndexByModelName("v_shield_p228", "view_finger", TexHandsIndex, 73, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "view_finger", TexHandsIndex, 74, 2);
	GetModelIndexByModelName("v_shield_usp", "view_finger", TexHandsIndex, 75, 2);
	GetModelIndexByModelName("v_ak47", "view_skin", TexHandsIndex, 76, 3);
	GetModelIndexByModelName("v_aug", "view_skin", TexHandsIndex, 77, 3);
	GetModelIndexByModelName("v_awp", "view_skin", TexHandsIndex, 78, 3);
	GetModelIndexByModelName("v_c4", "view_skin", TexHandsIndex, 79, 3);
	GetModelIndexByModelName("v_deagle", "view_skin", TexHandsIndex, 80, 3);
	GetModelIndexByModelName("v_elite", "view_skin", TexHandsIndex, 81, 3);
	GetModelIndexByModelName("v_famas", "view_skin", TexHandsIndex, 82, 3);
	GetModelIndexByModelName("v_fiveseven", "view_skin", TexHandsIndex, 83, 3);
	GetModelIndexByModelName("v_flashbang", "view_skin", TexHandsIndex, 84, 3);
	GetModelIndexByModelName("v_g3sg1", "view_skin", TexHandsIndex, 85, 3);
	GetModelIndexByModelName("v_galil", "view_skin", TexHandsIndex, 86, 3);
	GetModelIndexByModelName("v_glock18", "view_skin", TexHandsIndex, 87, 3);
	GetModelIndexByModelName("v_hegrenade", "view_skin", TexHandsIndex, 88, 3);
	GetModelIndexByModelName("v_knife", "view_skin", TexHandsIndex, 89, 3);
	GetModelIndexByModelName("v_m3", "view_skin", TexHandsIndex, 90, 3);
	GetModelIndexByModelName("v_m4a1", "view_skin", TexHandsIndex, 91, 3);
	GetModelIndexByModelName("v_m249", "view_skin", TexHandsIndex, 92, 3);
	GetModelIndexByModelName("v_mac10", "view_skin", TexHandsIndex, 93, 3);
	GetModelIndexByModelName("v_mp5", "view_skin", TexHandsIndex, 94, 3);
	GetModelIndexByModelName("v_p90", "view_skin", TexHandsIndex, 95, 3);
	GetModelIndexByModelName("v_p228", "view_skin", TexHandsIndex, 96, 3);
	GetModelIndexByModelName("v_scout", "view_skin", TexHandsIndex, 97, 3);
	GetModelIndexByModelName("v_sg550", "view_skin", TexHandsIndex, 98, 3);
	GetModelIndexByModelName("v_sg552", "view_skin", TexHandsIndex, 99, 3);
	GetModelIndexByModelName("v_smokegrenade", "view_skin", TexHandsIndex, 100, 3);
	GetModelIndexByModelName("v_tmp", "view_skin", TexHandsIndex, 101, 3);
	GetModelIndexByModelName("v_ump45", "view_skin", TexHandsIndex, 102, 3);
	GetModelIndexByModelName("v_usp", "view_skin", TexHandsIndex, 103, 3);
	GetModelIndexByModelName("v_xm1014", "view_skin", TexHandsIndex, 104, 3);
	GetModelIndexByModelName("v_shield_deagle", "view_skin", TexHandsIndex, 105, 2);
	GetModelIndexByModelName("v_shield_fiveseven", "view_skin", TexHandsIndex, 106, 2);
	GetModelIndexByModelName("v_shield_flashbang", "view_skin", TexHandsIndex, 107, 2);
	GetModelIndexByModelName("v_shield_glock18", "view_skin", TexHandsIndex, 108, 2);
	GetModelIndexByModelName("v_shield_hegrenade", "view_skin", TexHandsIndex, 109, 2);
	GetModelIndexByModelName("v_shield_knife", "view_skin", TexHandsIndex, 110, 2);
	GetModelIndexByModelName("v_shield_p228", "view_skin", TexHandsIndex, 111, 2);
	GetModelIndexByModelName("v_shield_smokegrenade", "view_skin", TexHandsIndex, 112, 2);
	GetModelIndexByModelName("v_shield_usp", "view_skin", TexHandsIndex, 113, 2);

	GetModelIndexByModelName("rshell", "rifle_goldshell", TexShellIndex, 0, 3);
	GetModelIndexByModelName("pshell", "pistol_goldshell", TexShellIndex, 1, 3);
	GetModelIndexByModelName("shotgunshell", "shell", TexShellIndex, 2, 3);
	GetModelIndexByModelName("rshell_big", "rifle_goldshell", TexShellIndex, 3, 3);

	GetModelIndexByModelName("chick", "skin", TexChickenIndex, 0, 3);

	if (ent && ent->model && ent->model->name && strstr(ent->model->name, "w_"))
	{
		//w_fiveseven has no hitbox
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(ent->model);
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();

		if (!g_PlayerEntityInfoList[ent->index].bGotWeapon)
		{
			Vector vCubePointsTrans[8], vCubePoints[8];

			vCubePoints[0] = Vector(pHitbox[0].bbmin.x, pHitbox[0].bbmin.y, pHitbox[0].bbmin.z);
			vCubePoints[1] = Vector(pHitbox[0].bbmin.x, pHitbox[0].bbmax.y, pHitbox[0].bbmin.z);
			vCubePoints[2] = Vector(pHitbox[0].bbmax.x, pHitbox[0].bbmax.y, pHitbox[0].bbmin.z);
			vCubePoints[3] = Vector(pHitbox[0].bbmax.x, pHitbox[0].bbmin.y, pHitbox[0].bbmin.z);
			vCubePoints[4] = Vector(pHitbox[0].bbmax.x, pHitbox[0].bbmax.y, pHitbox[0].bbmax.z);
			vCubePoints[5] = Vector(pHitbox[0].bbmin.x, pHitbox[0].bbmax.y, pHitbox[0].bbmax.z);
			vCubePoints[6] = Vector(pHitbox[0].bbmin.x, pHitbox[0].bbmin.y, pHitbox[0].bbmax.z);
			vCubePoints[7] = Vector(pHitbox[0].bbmax.x, pHitbox[0].bbmin.y, pHitbox[0].bbmax.z);

			for (unsigned int i = 0; i < 8; i++)
			{
				VectorTransform(vCubePoints[i], (*pBoneMatrix)[pHitbox[0].bone], vCubePointsTrans[i]);
				g_PlayerEntityInfoList[ent->index].vHitboxWeaponBox[i] = vCubePointsTrans[i];
			}

			g_PlayerEntityInfoList[ent->index].bGotWeapon = true;
		}
	}

	if (ent && ent->model && ent->model->name && strstr(ent->model->name, "chick"))
	{
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(ent->model);
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();

		if (!g_PlayerEntityInfoList[ent->index].bGotChick)
		{
			Vector pos, vBBMin, vBBMax, vCubePointsTrans[8], vCubePoints[8];
			
			for (unsigned int i = 0; i < 9; i++)
			{
				VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
				VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);

				g_PlayerEntityInfoList[ent->index].vHitboxChick[i] = (vBBMax + vBBMin) * 0.5f;

				vCubePoints[0] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[1] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[2] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[3] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[4] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[5] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[6] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
				vCubePoints[7] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);

				for (unsigned int x = 0; x < 8; x++)
				{
					VectorTransform(vCubePoints[x], (*pBoneMatrix)[pHitbox[i].bone], vCubePointsTrans[x]);
					g_PlayerEntityInfoList[ent->index].vHitboxChickBox[i][x] = vCubePointsTrans[x];
				}

				pos[0] = (*pBoneMatrix)[i][0][3];
				pos[1] = (*pBoneMatrix)[i][1][3];
				pos[2] = (*pBoneMatrix)[i][2][3];
				g_PlayerEntityInfoList[ent->index].vBoneChick[i] = pos;
			}

			g_PlayerEntityInfoList[ent->index].bGotChick = true;
		}
	}

	if (ent && ent->player)
	{
		studiohdr_t* pStudioHeader = (studiohdr_t*)g_Studio.Mod_Extradata(ent->model);
		mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);
		BoneMatrix_t* pBoneMatrix = (BoneMatrix_t*)g_Studio.StudioGetBoneTransform();

		if (!g_Player[ent->index].bGotPlayer)
		{
			Vector pos, vBBMin, vBBMax, vCubePointsTrans[8], vCubePoints[8];
			
			vCubePoints[0] = Vector(pHitbox[11].bbmin.x, pHitbox[11].bbmin.y, pHitbox[11].bbmin.z);
			vCubePoints[1] = Vector(pHitbox[11].bbmin.x, pHitbox[11].bbmax.y, pHitbox[11].bbmin.z);
			vCubePoints[2] = Vector(pHitbox[11].bbmax.x, pHitbox[11].bbmax.y, pHitbox[11].bbmin.z);
			vCubePoints[3] = Vector(pHitbox[11].bbmax.x, pHitbox[11].bbmin.y, pHitbox[11].bbmin.z);
			vCubePoints[4] = Vector(pHitbox[11].bbmax.x, pHitbox[11].bbmax.y, pHitbox[11].bbmax.z);
			vCubePoints[5] = Vector(pHitbox[11].bbmin.x, pHitbox[11].bbmax.y, pHitbox[11].bbmax.z);
			vCubePoints[6] = Vector(pHitbox[11].bbmin.x, pHitbox[11].bbmin.y, pHitbox[11].bbmax.z);
			vCubePoints[7] = Vector(pHitbox[11].bbmax.x, pHitbox[11].bbmin.y, pHitbox[11].bbmax.z);
			
			for (unsigned int i = 0; i < 8; i++)
			{
				VectorTransform(vCubePoints[i], (*pBoneMatrix)[pHitbox[11].bone], vCubePointsTrans[i]);
				g_PlayerExtraInfoList[ent->index].vHitboxFace[i] = vCubePointsTrans[i] + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
			}
			for (unsigned int i = 0; i < 8; i++)
			{
				VectorTransform(vCubePoints[i], (*pBoneMatrix)[pHitbox[11].bone], vCubePointsTrans[i]);
				g_PlayerExtraInfoList[ent->index].vHitboxSkeletonHead[i] = vCubePointsTrans[i] + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
			}
			for (unsigned int i = 0; i < 53; i++)
			{
				pos[0] = (*pBoneMatrix)[i][0][3];
				pos[1] = (*pBoneMatrix)[i][1][3];
				pos[2] = (*pBoneMatrix)[i][2][3];
				g_PlayerExtraInfoList[ent->index].vBoneSkeleton[i] = pos + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
			}
			
			for (unsigned int i = 0; i < 21; i++)
			{
				VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
				VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);
				g_PlayerExtraInfoList[ent->index].vHitbox[i] = (vBBMax + vBBMin) * 0.5f + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
				g_PlayerExtraInfoList[ent->index].fHitboxFOV[i] = g_Local.vForward.AngleBetween(g_PlayerExtraInfoList[ent->index].vHitbox[i] - g_Local.vEye);

				vCubePoints[0] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[1] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[2] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmin.z);
				vCubePoints[3] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmin.z);
				vCubePoints[4] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[5] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmax.y, pHitbox[i].bbmax.z);
				vCubePoints[6] = Vector(pHitbox[i].bbmin.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);
				vCubePoints[7] = Vector(pHitbox[i].bbmax.x, pHitbox[i].bbmin.y, pHitbox[i].bbmax.z);

				for (unsigned int x = 0; x < 8; x++)
				{
					VectorTransform(vCubePoints[x], (*pBoneMatrix)[pHitbox[i].bone], vCubePointsTrans[x]);
					g_PlayerExtraInfoList[ent->index].vHitboxMulti[i][x] = vCubePointsTrans[x] + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
				}
			}

			for (unsigned int i = 0; i < 53; i++)
			{
				if (i == 7)
				{
					VectorTransform(pHitbox[11].bbmin, (*pBoneMatrix)[pHitbox[11].bone], vBBMin);
					VectorTransform(pHitbox[11].bbmax, (*pBoneMatrix)[pHitbox[11].bone], vBBMax);
					g_PlayerExtraInfoList[ent->index].vBone[i] = (vBBMax + vBBMin) * 0.5f + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
				}
				else
				{
					pos[0] = (*pBoneMatrix)[i][0][3];
					pos[1] = (*pBoneMatrix)[i][1][3];
					pos[2] = (*pBoneMatrix)[i][2][3];
					g_PlayerExtraInfoList[ent->index].vBone[i] = pos + g_Player[ent->index].vVelocity * g_Player[ent->index].flFrametime;
				}

				g_PlayerExtraInfoList[ent->index].fBoneFOV[i] = g_Local.vForward.AngleBetween(g_PlayerExtraInfoList[ent->index].vBone[i] - g_Local.vEye);

				if (i > 0 && i < 7) GetHitboxBoneBox(i, ent, 7, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
				if (i == 7) GetHitboxBoneBox(i, ent, 11, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);

				if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
				{
					if (ModelArctic(ent->index) || ModelLeet(ent->index) || ModelSas(ent->index))
					{
						if (i == 10) GetHitboxBoneBox(i, ent, 13, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 11) GetHitboxBoneBox(i, ent, 14, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 12) GetHitboxBoneBox(i, ent, 15, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 25) GetHitboxBoneBox(i, ent, 17, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 26) GetHitboxBoneBox(i, ent, 18, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 27)GetHitboxBoneBox(i, ent, 19, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 44 || i == 50) GetHitboxBoneBox(i, ent, 3, 4, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 43 || i == 49) GetHitboxBoneBox(i, ent, 5, 1, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 45 || i == 51) GetHitboxBoneBox(i, ent, 15, 0, 2, 0, 2, 0, 0, pHitbox, pBoneMatrix);
						if (i == 48 || i == 42) GetHitboxBoneBox(i, ent, 5, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
					}
					if (ModelGuerilla(ent->index) || ModelUrban(ent->index) || ModelTerror(ent->index) || ModelGign(ent->index) || ModelGsg9(ent->index))
					{
						if (i == 10) GetHitboxBoneBox(i, ent, 13, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 11) GetHitboxBoneBox(i, ent, 14, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 12) GetHitboxBoneBox(i, ent, 15, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 24) GetHitboxBoneBox(i, ent, 17, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 25) GetHitboxBoneBox(i, ent, 18, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 26) GetHitboxBoneBox(i, ent, 19, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 42 || i == 48) GetHitboxBoneBox(i, ent, 3, 4, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 41 || i == 47) GetHitboxBoneBox(i, ent, 5, 1, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 43 || i == 49) GetHitboxBoneBox(i, ent, 15, 0, 2, 0, 2, 0, 0, pHitbox, pBoneMatrix);
						if (i == 46 || i == 40) GetHitboxBoneBox(i, ent, 5, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
					}
				}
				else
				{
					if (g_Player[ent->index].iTeam == 1)
					{
						if (i == 10) GetHitboxBoneBox(i, ent, 13, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 11) GetHitboxBoneBox(i, ent, 14, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 12) GetHitboxBoneBox(i, ent, 15, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 25) GetHitboxBoneBox(i, ent, 17, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 26) GetHitboxBoneBox(i, ent, 18, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 27)GetHitboxBoneBox(i, ent, 19, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 44 || i == 50) GetHitboxBoneBox(i, ent, 3, 4, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 43 || i == 49) GetHitboxBoneBox(i, ent, 5, 1, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 45 || i == 51) GetHitboxBoneBox(i, ent, 15, 0, 2, 0, 2, 0, 0, pHitbox, pBoneMatrix);
						if (i == 48 || i == 42) GetHitboxBoneBox(i, ent, 5, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
					}
					if (g_Player[ent->index].iTeam == 2)
					{
						if (i == 10) GetHitboxBoneBox(i, ent, 13, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 11) GetHitboxBoneBox(i, ent, 14, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 12) GetHitboxBoneBox(i, ent, 15, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 24) GetHitboxBoneBox(i, ent, 17, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 25) GetHitboxBoneBox(i, ent, 18, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 26) GetHitboxBoneBox(i, ent, 19, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 42 || i == 48) GetHitboxBoneBox(i, ent, 3, 4, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 41 || i == 47) GetHitboxBoneBox(i, ent, 5, 1, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
						if (i == 43 || i == 49) GetHitboxBoneBox(i, ent, 15, 0, 2, 0, 2, 0, 0, pHitbox, pBoneMatrix);
						if (i == 46 || i == 40) GetHitboxBoneBox(i, ent, 5, 0, 0, 0, 0, 0, 0, pHitbox, pBoneMatrix);
					}
				}
			}

			g_Player[ent->index].bGotPlayer = true;
		}
	}
}

void SkeletonBody0(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 28; i++)
	{
		if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrix[0][i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrix[0][i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void SkeletonBody1(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 28; i++)
	{
		if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrix[1][i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrix[1][i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void SkeletonBodyNoModel(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 8; i++)
	{
		if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrixNoModel[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneSkeleton[SkeletonBoneMatrixNoModel[i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void SkeletonHead(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 12; i++)
	{
		if (CalcScreen(g_PlayerExtraInfoList[ax].vHitboxSkeletonHead[SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vHitboxSkeletonHead[SkeletonHitboxMatrix[i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void Bone0(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int x = 0; x < 21; x++)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrix0[x]][SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrix0[x]][SkeletonHitboxMatrix[i][1]], vecScreen2))
			{
				ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
			}
		}
	}
}

void BoneNum0(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int x = 0; x < 21; x++)
	{
		char str[10];
		sprintf(str, "%i", BoneMatrix0[x]);

		if (CalcScreen(g_PlayerExtraInfoList[ax].vBone[BoneMatrix0[x]], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void Bone1(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int x = 0; x < 21; x++)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrix1[x]][SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrix1[x]][SkeletonHitboxMatrix[i][1]], vecScreen2))
			{
				ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
			}
		}
	}
}

void BoneNum1(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int x = 0; x < 21; x++)
	{
		char str[10];
		sprintf(str, "%i", BoneMatrix1[x]);

		if (CalcScreen(g_PlayerExtraInfoList[ax].vBone[BoneMatrix1[x]], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void BoneNoModel(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int x = 0; x < 7; x++)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			if (CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrixNoModel[x]][SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vBoneMulti[BoneMatrixNoModel[x]][SkeletonHitboxMatrix[i][1]], vecScreen2))
			{
				ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
			}
		}
	}
}

void BoneNoModelNum(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int x = 0; x < 7; x++)
	{
		char str[10];
		sprintf(str, "%i", BoneMatrixNoModel[x]);

		if (CalcScreen(g_PlayerExtraInfoList[ax].vBone[BoneMatrixNoModel[x]], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void BoneNoModelNums(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int x = 0; x < cvar.rage_active; x++)
	{
		char str[10];
		sprintf(str, "%i", x);

		if (CalcScreen(g_PlayerExtraInfoList[ax].vBone[x], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void Hitbox(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int x = 0; x < 21; x++)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			if (CalcScreen(g_PlayerExtraInfoList[ax].vHitboxMulti[x][SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerExtraInfoList[ax].vHitboxMulti[x][SkeletonHitboxMatrix[i][1]], vecScreen2))
			{
				ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
			}
		}
	}
}

void HitboxNum(int ax, ImU32 white)
{
	for (unsigned int x = 0; x < 21; x++)
	{
		float vecScreen[2];
		char str[10];
		sprintf(str, "%i", x);
		if (CalcScreen(g_PlayerExtraInfoList[ax].vHitbox[x], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void HitboxWeapon(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 12; i++)
	{
		if (CalcScreen(g_PlayerEntityInfoList[ax].vHitboxWeaponBox[SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerEntityInfoList[ax].vHitboxWeaponBox[SkeletonHitboxMatrix[i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void HitboxChick(int ax, ImU32 color, int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int x = 0; x < 9; x++)
	{
		for (unsigned int i = 0; i < 12; i++)
		{
			if (CalcScreen(g_PlayerEntityInfoList[ax].vHitboxChickBox[x][SkeletonHitboxMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerEntityInfoList[ax].vHitboxChickBox[x][SkeletonHitboxMatrix[i][1]], vecScreen2))
			{
				ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
			}
		}
	}
}

void HitboxChickenNumber(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int i = 0; i < 9; i++)
	{
		char str[10];
		sprintf(str, "%i", i);

		if (CalcScreen(g_PlayerEntityInfoList[ax].vHitboxChick[i], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void BoneChickenNumber(int ax, ImU32 white)
{
	float vecScreen[2];
	for (unsigned int i = 0; i < 9; i++)
	{
		char str[10];
		sprintf(str, "%i", i);

		if (CalcScreen(g_PlayerEntityInfoList[ax].vBoneChick[i], vecScreen))
		{
			ImGui::GetCurrentWindow()->DrawList->AddText({ vecScreen[0], vecScreen[1] }, white, str);
		}
	}
}

void BoneChicken(int ax, ImU32 color,int offside)
{
	float vecScreen[2], vecScreen2[2];
	for (unsigned int i = 0; i < 8; i++)
	{
		if (CalcScreen(g_PlayerEntityInfoList[ax].vBoneChick[SkeletonChickenMatrix[i][0]], vecScreen) && CalcScreen(g_PlayerEntityInfoList[ax].vBoneChick[SkeletonChickenMatrix[i][1]], vecScreen2))
		{
			ImGui::GetCurrentWindow()->DrawList->AddLine({ vecScreen[0] + offside, vecScreen[1] + offside }, { vecScreen2[0] + offside, vecScreen2[1] + offside }, color);
		}
	}
}

void DrawSkeletonPlayer()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonPlayer", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax != g_Local.iIndex && g_Player[ax].bAlive && g_Player[ax].bGotPlayer)
				{
					if (g_Player[ax].iTeam != g_Local.iTeam || cvar.esp_visual_team)
					{
						ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
						ImU32 team = ImColor(PlayerColor(ax)->onebased_r, PlayerColor(ax)->onebased_g, PlayerColor(ax)->onebased_b, cvar.esp_alpha);

						if (cvar.skeleton_player_body || cvar.skeleton_player_stick_figure)
						{
							if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
							{
								if (ModelArctic(ax) || ModelLeet(ax) || ModelSas(ax))
								{
									SkeletonBody0(ax, black, 1);
									SkeletonBody0(ax, team, 0);
								}
								else if (ModelGuerilla(ax) || ModelUrban(ax) || ModelTerror(ax) || ModelGign(ax) || ModelGsg9(ax))
								{
									SkeletonBody1(ax, black, 1);
									SkeletonBody1(ax, team, 0);
								}
								else
								{
									SkeletonBodyNoModel(ax, black, 1);
									SkeletonBodyNoModel(ax, team, 0);
								}
							}
							else
							{
								if (g_Player[ax].iTeam == 1)
								{
									SkeletonBody0(ax, black, 1);
									SkeletonBody0(ax, team, 0);
								}
								if (g_Player[ax].iTeam == 2)
								{
									SkeletonBody1(ax, black, 1);
									SkeletonBody1(ax, team, 0);
								}
							}
						}
						if (cvar.skeleton_player_stick_figure)
						{
							SkeletonHead(ax, black, 1);
							SkeletonHead(ax, team, 0);
						}
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
	WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonPlayerBoneBox", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax != g_Local.iIndex && g_Player[ax].bAlive && g_Player[ax].bGotPlayer)
				{
					if (g_Player[ax].iTeam != g_Local.iTeam || cvar.esp_visual_team)
					{
						ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
						ImU32 team = ImColor(PlayerColor(ax)->onebased_r, PlayerColor(ax)->onebased_g, PlayerColor(ax)->onebased_b, cvar.esp_alpha);
						ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);

						if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
						{
							if (ModelArctic(ax) || ModelLeet(ax) || ModelSas(ax))
							{
								if (cvar.skeleton_player_bonebox)
								{
									Bone0(ax, black, 1);
									Bone0(ax, team, 0);
								}
								if (cvar.skeleton_player_bonebox_num)BoneNum0(ax, white);
							}
							else if (ModelGuerilla(ax) || ModelUrban(ax) || ModelTerror(ax) || ModelGign(ax) || ModelGsg9(ax))
							{
								if (cvar.skeleton_player_bonebox)
								{
									Bone1(ax, black, 1);
									Bone1(ax, team, 0);
								}
								if (cvar.skeleton_player_bonebox_num)BoneNum1(ax, white);
							}
							else
							{
								if (cvar.skeleton_player_bonebox)
								{
									BoneNoModel(ax, black, 1);
									BoneNoModel(ax, team, 0);
								}
								if (cvar.skeleton_player_bonebox_num)BoneNoModelNum(ax, white);
							}
						}
						else
						{
							if (g_Player[ax].iTeam == 1)
							{
								if (cvar.skeleton_player_bonebox)
								{
									Bone0(ax, black, 0);
									Bone0(ax, team, 0);
								}
								if (cvar.skeleton_player_bonebox_num)BoneNum0(ax, white);
							}
							if (g_Player[ax].iTeam == 2)
							{
								if (cvar.skeleton_player_bonebox)
								{
									Bone1(ax, black, 0);
									Bone1(ax, team, 0);
								}
								if (cvar.skeleton_player_bonebox_num)BoneNum1(ax, white);
							}
						}
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
	WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonPlayerHitBox", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax != g_Local.iIndex && g_Player[ax].bAlive && g_Player[ax].bGotPlayer)
				{
					if (g_Player[ax].iTeam != g_Local.iTeam || cvar.esp_visual_team)
					{
						ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
						ImU32 team = ImColor(PlayerColor(ax)->onebased_r, PlayerColor(ax)->onebased_g, PlayerColor(ax)->onebased_b, cvar.esp_alpha);
						ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);

						if (cvar.skeleton_player_hitbox)
						{
							Hitbox(ax, black, 1);
							Hitbox(ax, team, 0);
						}
						if (cvar.skeleton_player_hitbox_num)HitboxNum(ax, white);
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}

void DrawSkeletonThirdPerson()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonThirdPerson", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax == g_Local.iIndex && g_Local.bAlive && cvar.esp_chase_cam && g_Player[ax].bGotPlayer)
				{
					ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
					ImU32 colorwheel = ImColor(cvar.color_green, cvar.color_blue, cvar.color_red, cvar.esp_alpha);

					if (cvar.skeleton_third_person_body || cvar.skeleton_third_person_stick_figure)
					{
						if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
						{
							if (ModelArctic(ax) || ModelLeet(ax) || ModelSas(ax))
							{
								SkeletonBody0(ax, black, 1);
								SkeletonBody0(ax, colorwheel, 0);
							}
							else if (ModelGuerilla(ax) || ModelUrban(ax) || ModelTerror(ax) || ModelGign(ax) || ModelGsg9(ax))
							{
								SkeletonBody1(ax, black, 1);
								SkeletonBody1(ax, colorwheel, 0);
							}
							else
							{
								SkeletonBodyNoModel(ax, black, 1);
								SkeletonBodyNoModel(ax, colorwheel, 0);
							}
						}
						else
						{
							if (g_Local.iTeam == 1)
							{
								SkeletonBody0(ax, black, 1);
								SkeletonBody0(ax, colorwheel, 0);
							}
							if (g_Local.iTeam == 2)
							{
								SkeletonBody1(ax, black, 1);
								SkeletonBody1(ax, colorwheel, 0);
							}
						}
					}
					if (cvar.skeleton_third_person_stick_figure)
					{
						SkeletonHead(ax, colorwheel, 0);
						SkeletonHead(ax, black, 1);
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
	WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonThirdPersonBoneBox", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax == g_Local.iIndex && g_Local.bAlive && cvar.esp_chase_cam && g_Player[ax].bGotPlayer)
				{
					ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
					ImU32 colorwheel = ImColor(cvar.color_green, cvar.color_blue, cvar.color_red, cvar.esp_alpha);
					ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);

					if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
					{
						if (ModelArctic(ax) || ModelLeet(ax) || ModelSas(ax))
						{
							if (cvar.skeleton_third_person_bonebox)
							{
								Bone0(ax, black, 1);
								Bone0(ax, colorwheel, 0);
							}
							if (cvar.skeleton_third_person_bonebox_num)BoneNum0(ax, white);
						}
						else if (ModelGuerilla(ax) || ModelUrban(ax) || ModelTerror(ax) || ModelGign(ax) || ModelGsg9(ax))
						{
							if (cvar.skeleton_third_person_bonebox)
							{
								Bone1(ax, black, 1);
								Bone1(ax, colorwheel, 0);
							}
							if (cvar.skeleton_third_person_bonebox_num)BoneNum1(ax, white);
						}
						else
						{
							if (cvar.skeleton_third_person_bonebox)
							{
								BoneNoModel(ax, black, 1);
								BoneNoModel(ax, colorwheel, 0);
							}
							if (cvar.skeleton_third_person_bonebox_num)BoneNoModelNum(ax, white);
						}
					}
					else
					{
						if (g_Local.iTeam == 1)
						{
							if (cvar.skeleton_third_person_bonebox)
							{
								Bone0(ax, black, 0);
								Bone0(ax, colorwheel, 0);
							}
							if (cvar.skeleton_third_person_bonebox_num)BoneNum0(ax, white);
						}
						if (g_Local.iTeam == 2)
						{
							if (cvar.skeleton_third_person_bonebox)
							{
								Bone1(ax, black, 0);
								Bone1(ax, colorwheel, 0);
							}
							if (cvar.skeleton_third_person_bonebox_num)BoneNum1(ax, white);
						}
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
	WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("SkeletonThirdPersonHitBox", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 33; ax++)
			{
				if (ax == g_Local.iIndex && g_Local.bAlive && cvar.esp_chase_cam && g_Player[ax].bGotPlayer)
				{
					ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
					ImU32 colorwheel = ImColor(cvar.color_green, cvar.color_blue, cvar.color_red, cvar.esp_alpha);
					ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);

					if (cvar.skeleton_third_person_hitbox)
					{
						Hitbox(ax, black, 1);
						Hitbox(ax, colorwheel, 0);
					}
					if (cvar.skeleton_third_person_hitbox_num)HitboxNum(ax, white);
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}

void DrawEntityHitbox()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("HitboxWeapon", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 1024; ax++)
			{
				if (g_PlayerEntityInfoList[ax].bGotWeapon)
				{
					ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
					ImU32 colorwheel = ImColor(cvar.color_green, cvar.color_blue, cvar.color_red, cvar.esp_alpha);
					ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);
					
					if (cvar.skeleton_weapon_world_box)
					{
						HitboxWeapon(ax, black, 1);
						HitboxWeapon(ax, colorwheel, 0);
					}
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
	WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("HitboxChick", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			for (unsigned int ax = 0; ax < 1024; ax++)
			{
				if (g_PlayerEntityInfoList[ax].bGotChick)
				{
					ImU32 black = ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha);
					ImU32 colorwheel = ImColor(cvar.color_green, cvar.color_blue, cvar.color_red, cvar.esp_alpha);
					ImU32 white = ImColor(colorList.get(4)->onebased_r, colorList.get(4)->onebased_g, colorList.get(4)->onebased_b, cvar.esp_alpha);

					if (cvar.skeleton_chicken_bone)
					{
						BoneChicken(ax, black, 1);
						BoneChicken(ax, colorwheel, 0);
					}

					if (cvar.skeleton_chicken_box)
					{
						HitboxChick(ax, black, 1);
						HitboxChick(ax, colorwheel, 0);
					}

					if (cvar.skeleton_chicken_bone_num)BoneChickenNumber(ax, white);
					if (cvar.skeleton_chicken_box_num)HitboxChickenNumber(ax, white);
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}