#include "client.h"

DWORD dwReactionTime;
Vector vAimOriginRage;
Vector vAimOriginKnife;
Vector vAimOriginLegit;

int iTargetLegit;
int iTargetRage;
int iTargetKnife;

float m_flCurrentFOV;

bool IsBoxIntersectingRay(const Vector& boxMin, const Vector& boxMax, const Vector& origin, const Vector& delta)
{
	ASSERT(boxMin[0] <= boxMax[0]);
	ASSERT(boxMin[1] <= boxMax[1]);
	ASSERT(boxMin[2] <= boxMax[2]);

	float tmin = -FLT_MAX;
	float tmax = FLT_MAX;

	for (unsigned int i = 0; i < 3; ++i)
	{
		if (fabs(delta[i]) < 1e-8)
		{
			if ((origin[i] < boxMin[i]) || (origin[i] > boxMax[i]))
				return false;

			continue;
		}

		float invDelta = 1.0f / delta[i];
		float t1 = (boxMin[i] - origin[i]) * invDelta;
		float t2 = (boxMax[i] - origin[i]) * invDelta;
		if (t1 > t2)
		{
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}
		if (t1 > tmin)
			tmin = t1;
		if (t2 < tmax)
			tmax = t2;
		if (tmin > tmax)
			return false;
		if (tmax < 0)
			return false;
	}

	return true;
}

void TriggerAimbot(struct usercmd_s* cmd)
{
	if (!cvar.legit[g_Local.weapon.m_iWeaponID].trigger_active || !IsCurWeaponGun() || !CanAttack())
		return;

	if (cvar.legit_trigger_only_zoom && IsCurWeaponSniper() && g_Local.iFOV == 90)
		return;

	float flAccuracy = cvar.legit[g_Local.weapon.m_iWeaponID].trigger_accuracy;

	Vector vecSpreadDir, vecForward, vecRight, vecUp, vecRandom;

	QAngle QAngles;

	g_Engine.GetViewAngles(QAngles);

	if (flAccuracy > 0)//Recoil
	{
		QAngles[0] += g_Local.vPunchangle[0];
		QAngles[1] += g_Local.vPunchangle[1];
		QAngles[2] = NULL;
	}

	QAngles.Normalize();

	QAngles.AngleVectors(&vecForward, &vecRight, &vecUp);

	if (flAccuracy > 1)//Recoil / Spread
	{
		GetSpreadXY(g_Local.weapon.random_seed, 1, vecRandom);
		vecSpreadDir = vecForward + (vecRight * vecRandom[0]) + (vecUp * vecRandom[1]);
		vecSpreadDir.Normalize();
	}
	else
	{//Empty or Recoil
		vecSpreadDir = vecForward;
		vecSpreadDir.Normalize();
	}

	static DWORD delay = 0;

	static int tickcount = 0;

	for (unsigned int id = 0; id < 33; ++id)
	{
		if (id == g_Local.iIndex)
			continue;

		if (!g_Player[id].bAlive)
			continue;

		if (!g_Player[id].bVisible)
			continue;

		if (!cvar.legit_team_attack && g_Player[id].iTeam == g_Local.iTeam)
			continue;

		std::deque<unsigned int> Bones;

		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_head) Bones.push_back(7);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_chest) Bones.push_back(5);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_stomach) Bones.push_back(2);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_all)
		{
			if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
			{
				if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
				else if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
				else
					for (unsigned int i = 0; i < 4; i++) Bones.push_back(TriggerBone[i]);
			}
			else
			{
				if (g_Player[id].iTeam == 1)
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
				if (g_Player[id].iTeam == 2)
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
			}
		}

		if (Bones.empty())
			return;

		if (GetTickCount() - delay > cvar.legit[g_Local.weapon.m_iWeaponID].trigger_delay_shot)
		{
			for (auto&& bones : Bones)
			{
				for (unsigned int i = 0; i < 12; i++)
				{
					if (g_PlayerExtraInfoList[id].bBonePointsVisible[bones][SkeletonHitboxMatrix[i][0]] && g_PlayerExtraInfoList[id].bBonePointsVisible[bones][SkeletonHitboxMatrix[i][1]])
					{
						if (IsBoxIntersectingRay(g_PlayerExtraInfoList[id].vBoneMulti[bones][SkeletonHitboxMatrix[i][0]], g_PlayerExtraInfoList[id].vBoneMulti[bones][SkeletonHitboxMatrix[i][1]], g_Local.vEye, vecSpreadDir))
						{
							cmd->buttons |= IN_ATTACK;
						}
					}
				}
			}
		}
	}
	if (cmd->buttons & IN_ATTACK)tickcount++;
	if (tickcount >= (!cvar.legit[g_Local.weapon.m_iWeaponID].trigger_shot_type ? cvar.legit[g_Local.weapon.m_iWeaponID].trigger_shot_count : rand() % (int)cvar.legit[g_Local.weapon.m_iWeaponID].trigger_random_max + 1))
	{
		delay = GetTickCount();
		tickcount = 0;
	}
}

void TriggerDraw()
{
	if (!cvar.legit[g_Local.weapon.m_iWeaponID].trigger_active || !IsCurWeaponGun() || !g_Local.bAlive)
		return;

	if (cvar.legit_trigger_only_zoom && IsCurWeaponSniper() && g_Local.iFOV == 90)
		return;

	for (unsigned int id = 0; id < 33; ++id)
	{
		if (id == g_Local.iIndex)
			continue;

		if (!g_Player[id].bAlive)
			continue;

		if (!g_Player[id].bVisible)
			continue;

		if (!cvar.legit_team_attack && g_Player[id].iTeam == g_Local.iTeam)
			continue;

		std::deque<unsigned int> Bones;

		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_head) Bones.push_back(7);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_chest) Bones.push_back(5);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_stomach) Bones.push_back(2);
		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_all)
		{
			if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
			{
				if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
				else if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
				else
					for (unsigned int i = 0; i < 4; i++) Bones.push_back(TriggerBone[i]);
			}
			else
			{
				if (g_Player[id].iTeam == 1)
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
				if (g_Player[id].iTeam == 2)
					for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
			}
		}

		if (Bones.empty())
			return;

		for (auto&& bones : Bones)
		{
			for (unsigned int i = 0; i < 12; i++)
			{
				if (g_PlayerExtraInfoList[id].bBonePointsVisible[bones][SkeletonHitboxMatrix[i][0]] && g_PlayerExtraInfoList[id].bBonePointsVisible[bones][SkeletonHitboxMatrix[i][1]])
				{
					float CalcAnglesMin[2], CalcAnglesMax[2];
					if (CalcScreen(g_PlayerExtraInfoList[id].vBoneMulti[bones][SkeletonHitboxMatrix[i][0]], CalcAnglesMin) && CalcScreen(g_PlayerExtraInfoList[id].vBoneMulti[bones][SkeletonHitboxMatrix[i][1]], CalcAnglesMax))
					{
						ImGui::GetCurrentWindow()->DrawList->AddLine({ CalcAnglesMin[0] + 1, CalcAnglesMin[1] + 1 }, { CalcAnglesMax[0], CalcAnglesMax[1] + 1 }, ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha));
						ImGui::GetCurrentWindow()->DrawList->AddLine({ CalcAnglesMin[0], CalcAnglesMin[1] }, { CalcAnglesMax[0], CalcAnglesMax[1] }, ImColor(colorList.get(2)->onebased_r, colorList.get(2)->onebased_g, colorList.get(2)->onebased_b, cvar.esp_alpha));
					}
				}
			}
		}
	}
}

void SmoothAimAngles(QAngle MyViewAngles, QAngle AimAngles, QAngle& OutAngles, float Smoothing, bool bSpiral, float SpiralX, float SpiralY)
{
	if (Smoothing < 1)
	{
		OutAngles = AimAngles;
		return;
	}

	OutAngles = AimAngles - MyViewAngles;

	OutAngles.Normalize();

	Vector vecViewAngleDelta = OutAngles;

	if (bSpiral && SpiralX != 0 && SpiralY != 0)
		vecViewAngleDelta += Vector(vecViewAngleDelta.y / SpiralX, vecViewAngleDelta.x / SpiralY, 0.0f);

	if (!isnan(Smoothing))
		vecViewAngleDelta /= Smoothing;

	OutAngles = MyViewAngles + vecViewAngleDelta;

	OutAngles.Normalize();
}

void LegitAimbot(struct usercmd_s* cmd)
{
	static DWORD dwBlockAttack = 0;

	static float flSpeedSpiralX = 1.3;
	static float flSpeedSpiralY = 3.7;

	m_flCurrentFOV = 0;

	if (!IsCurWeaponGun() || g_Local.weapon.m_iInReload || g_Local.weapon.m_iClip < 1 || g_Local.weapon.m_flNextAttack > 0.0)
		return;

	float flFOV = cvar.legit[g_Local.weapon.m_iWeaponID].fov;

	if (!flFOV)
		return;

	float flSpeed = 0;

	if (cvar.legit[g_Local.weapon.m_iWeaponID].speed_in_attack)
		flSpeed = 101 - cvar.legit[g_Local.weapon.m_iWeaponID].speed_in_attack;

	if (cvar.legit[g_Local.weapon.m_iWeaponID].speed && !(cmd->buttons & IN_ATTACK))
		flSpeed = 101 - cvar.legit[g_Local.weapon.m_iWeaponID].speed;

	if (!flSpeed)
		return;

	float flReactionTime = cvar.legit[g_Local.weapon.m_iWeaponID].reaction_time;

	if (flReactionTime > 0 && GetTickCount() - dwReactionTime < flReactionTime)
		return;

	float flSpeedScaleFov = cvar.legit[g_Local.weapon.m_iWeaponID].speed_scale_fov;

	bool bSpeedSpiral = cvar.legit[g_Local.weapon.m_iWeaponID].humanize;

	if (!g_Local.vPunchangle.IsZero2D())
		bSpeedSpiral = false;

	float flRecoilCompensationPitch = 0.02f * cvar.legit[g_Local.weapon.m_iWeaponID].recoil_compensation_pitch;
	float flRecoilCompensationYaw = 0.02f * cvar.legit[g_Local.weapon.m_iWeaponID].recoil_compensation_yaw;

	unsigned int iRecoilCompensationAfterShotsFired = static_cast<int>(cvar.legit[g_Local.weapon.m_iWeaponID].recoil_compensation_after_shots_fired);

	if (iRecoilCompensationAfterShotsFired > 0 && g_Local.weapon.m_iShotsFired <= iRecoilCompensationAfterShotsFired)
	{
		flRecoilCompensationPitch = 0;
		flRecoilCompensationYaw = 0;
	}
	float flBlockAttackAfterKill = cvar.legit[g_Local.weapon.m_iWeaponID].block_attack_after_kill;

	float flAccuracy = cvar.legit[g_Local.weapon.m_iWeaponID].accuracy;

	float flPSilent = cvar.legit[g_Local.weapon.m_iWeaponID].silent;

	Vector vecFOV = {};
	{
		QAngle QAngles = cmd->viewangles + g_Local.vPunchangle;
		QAngles.Normalize();
		QAngles.AngleVectors(&vecFOV, NULL, NULL);
		vecFOV.Normalize();
	}

	m_flCurrentFOV = flFOV;

	int iBone = -1;

	std::deque<unsigned int> Bones;

	float flBestFOV = flFOV;

	for (unsigned int id = 0; id < 33; ++id)
	{
		if (idhook.FirstKillPlayer[id] == 1 || cvar.aim_id_mode == 0)
		{
			if (id == g_Local.iIndex)
				continue;

			if (!g_Player[id].bAlive)
				continue;

			if (!g_Player[id].bVisible)
				continue;

			if (!cvar.legit_team_attack && g_Player[id].iTeam == g_Local.iTeam)
				continue;

			if (cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone) Bones.push_front(2);
			if (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone)
			{
				Bones.push_back(1);
				for (unsigned int i = 0; i < 4; i++) Bones.push_front(LegitBone[i]);
			}
			if (cvar.legit[g_Local.weapon.m_iWeaponID].head_bone) Bones.push_front(7);
			if (cvar.legit[g_Local.weapon.m_iWeaponID].all_bone)
			{
				if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
				{
					if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
					if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
				}
				else
				{
					if (g_Player[id].iTeam == 1)
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
					if (g_Player[id].iTeam == 2)
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
				}
			}

			if (Bones.empty())
				return;

			for (auto&& bones : Bones)
			{
				if (!g_PlayerExtraInfoList[id].bBoneVisible[bones])
					continue;

				if (cvar.legit[g_Local.weapon.m_iWeaponID].head_bone && (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone || cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone) && bones == 7 && (!(g_Local.weapon.m_iFlags & FL_ONGROUND) || g_Local.flVelocity2D > 140 || g_Local.weapon.m_iShotsFired > 5))
					continue;

				if (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone && cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone && bones != 0 && bones != 7 && (!(g_Local.weapon.m_iFlags & FL_ONGROUND) || g_Local.weapon.m_iShotsFired > 15))
					continue;

				float fov = vecFOV.AngleBetween(g_PlayerExtraInfoList[id].vBone[bones] - g_Local.vEye);

				if (fov < flBestFOV)
				{
					flBestFOV = fov;
					iTargetLegit = id;
					iBone = bones;
				}
			}
		}
	}
	if (cvar.aim_id_mode != 2 && iTargetLegit == 0)
	{
		for (unsigned int id = 0; id < 33; ++id)
		{
			if (idhook.FirstKillPlayer[id] < 2)
			{
				if (id == g_Local.iIndex)
					continue;

				if (!g_Player[id].bAlive)
					continue;

				if (!g_Player[id].bVisible)
					continue;

				if (!cvar.legit_team_attack && g_Player[id].iTeam == g_Local.iTeam)
					continue;

				if (cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone) Bones.push_front(2);
				if (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone)
				{
					Bones.push_back(1);
					for (unsigned int i = 0; i < 4; i++) Bones.push_front(LegitBone[i]);
				}
				if (cvar.legit[g_Local.weapon.m_iWeaponID].head_bone) Bones.push_front(7);
				if (cvar.legit[g_Local.weapon.m_iWeaponID].all_bone)
				{
					if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
					{
						if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
						if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
					}
					else
					{
						if (g_Player[id].iTeam == 1)
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
						if (g_Player[id].iTeam == 2)
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
					}
				}

				if (Bones.empty())
					return;

				for (auto&& bones : Bones)
				{
					if (!g_PlayerExtraInfoList[id].bBoneVisible[bones])
						continue;

					if (cvar.legit[g_Local.weapon.m_iWeaponID].head_bone && (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone || cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone) && bones == 7 && (!(g_Local.weapon.m_iFlags & FL_ONGROUND) || g_Local.flVelocity2D > 140 || g_Local.weapon.m_iShotsFired > 5))
						continue;

					if (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone && cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone && bones != 0 && bones != 7 && (!(g_Local.weapon.m_iFlags & FL_ONGROUND) || g_Local.weapon.m_iShotsFired > 15))
						continue;

					float fov = vecFOV.AngleBetween(g_PlayerExtraInfoList[id].vBone[bones] - g_Local.vEye);

					if (fov < flBestFOV)
					{
						flBestFOV = fov;
						iTargetLegit = id;
						iBone = bones;
					}
				}
			}
		}
	}

	if (iTargetLegit)
	{
		bool bAttack = false;
		bool bBlock = false;//Block IN_ATTACK?

		QAngle QMyAngles, QNewAngles, QSmoothAngles, QAimAngles;

		vAimOriginLegit = g_PlayerExtraInfoList[iTargetLegit].vBone[iBone];

		g_Engine.GetViewAngles(QMyAngles);

		VectorAngles(vAimOriginLegit - g_Local.vEye, QAimAngles);

		if (flPSilent > 0 && flPSilent <= 1 && CanAttack())
		{
			QAngle QAngleSilent = QAimAngles;

			QAngleSilent += g_Local.vPunchangle;

			QAngleSilent.Normalize();

			GetSpreadOffset(g_Local.weapon.random_seed, 1, QAngleSilent, QAngleSilent);

			Vector vecPsilentFOV;
			QAngleSilent.AngleVectors(&vecPsilentFOV, NULL, NULL);
			vecPsilentFOV.Normalize();

			float fov = vecPsilentFOV.AngleBetween(g_PlayerExtraInfoList[iTargetLegit].vBone[iBone] - g_Local.vEye);

			if (fov <= flPSilent && cmd->buttons & IN_ATTACK)
			{
				MakeAngle(QAngleSilent, cmd);

				dwBlockAttack = GetTickCount();
				return;
			}
		}

		QNewAngles[0] = QAimAngles[0] - g_Local.vPunchangle[0] * flRecoilCompensationPitch;
		QNewAngles[1] = QAimAngles[1] - g_Local.vPunchangle[1] * flRecoilCompensationYaw;
		QNewAngles[2] = 0;

		QNewAngles.Normalize();

		if (flSpeedScaleFov > 0 && flSpeedScaleFov <= 100 && g_Local.vPunchangle.IsZero() && !isnan(g_PlayerExtraInfoList[iTargetLegit].fBoneFOV[iBone]))
		{
			flSpeed = flSpeed - (((g_PlayerExtraInfoList[iTargetLegit].fBoneFOV[iBone] * (flSpeed / m_flCurrentFOV)) * flSpeedScaleFov) / 100);
		}

		SmoothAimAngles(QMyAngles, QNewAngles, QSmoothAngles, flSpeed, bSpeedSpiral, flSpeedSpiralX, flSpeedSpiralY);

		if (flAccuracy > 0)
		{
			bBlock = true;

			QAngle QAngleAccuracy = QAimAngles;

			Vector vecSpreadDir;

			if (flAccuracy == 1)//Aiming
			{
				QSmoothAngles.AngleVectors(&vecSpreadDir, NULL, NULL);

				vecSpreadDir.Normalize();
			}
			else if (flAccuracy == 2) //Recoil
			{
				Vector vecRandom, vecForward;

				SmoothAimAngles(QMyAngles, QAimAngles, QAngleAccuracy, flSpeed, bSpeedSpiral, flSpeedSpiralX, flSpeedSpiralY);

				QAngleAccuracy[0] += g_Local.vPunchangle[0];
				QAngleAccuracy[1] += g_Local.vPunchangle[1];
				QAngleAccuracy[2] = NULL;

				QAngleAccuracy.Normalize();

				QAngleAccuracy.AngleVectors(&vecForward, NULL, NULL);

				vecSpreadDir = vecForward;

				vecSpreadDir.Normalize();
			}
			else //Recoil / Spread
			{
				Vector vecRandom, vecRight, vecUp, vecForward;

				SmoothAimAngles(QMyAngles, QAimAngles, QAngleAccuracy, flSpeed, bSpeedSpiral, flSpeedSpiralX, flSpeedSpiralY);

				QAngleAccuracy[0] += g_Local.vPunchangle[0];
				QAngleAccuracy[1] += g_Local.vPunchangle[1];
				QAngleAccuracy[2] = NULL;

				QAngleAccuracy.Normalize();

				QAngleAccuracy.AngleVectors(&vecForward, &vecRight, &vecRight);

				GetSpreadXY(g_Local.weapon.random_seed, 1, vecRandom);

				vecSpreadDir = vecForward + (vecRight * vecRandom[0]) + (vecUp * vecRandom[1]);

				vecSpreadDir.Normalize();
			}

			for (unsigned int x = 0; x < Bones.size(); x++)
			{
				unsigned int bones = Bones[x];

				for (unsigned int i = 0; i < 12; i++)
				{
					if (IsBoxIntersectingRay(g_PlayerExtraInfoList[iTargetLegit].vBoneMulti[bones][SkeletonHitboxMatrix[i][0]], g_PlayerExtraInfoList[iTargetLegit].vBoneMulti[bones][SkeletonHitboxMatrix[i][1]], g_Local.vEye, vecSpreadDir))
					{
						bBlock = false;
						break;
					}
				}
			}
		}

		if (cmd->buttons & IN_ATTACK)
			bAttack = true;
		else if (cvar.legit[g_Local.weapon.m_iWeaponID].speed)//Auto aim
		{
			bAttack = true;
			bBlock = true;
		}

		if (bAttack)
		{
			QSmoothAngles.Normalize();

			MakeAngle(QSmoothAngles, cmd);
			cmd->viewangles = QSmoothAngles;
			g_Engine.SetViewAngles(QSmoothAngles);

			if (!bBlock)
				cmd->buttons |= IN_ATTACK;
			else if (cmd->buttons & IN_ATTACK)
				cmd->buttons &= ~IN_ATTACK;

			if (!g_Local.vPunchangle.IsZero2D())
				dwBlockAttack = GetTickCount();
		}
	}
	else if (flBlockAttackAfterKill > 0 && GetTickCount() - dwBlockAttack < flBlockAttackAfterKill)
	{
		cmd->buttons &= ~IN_ATTACK;
	}
}

void LegitDraw()
{
	if (!IsCurWeaponGun() || !g_Local.bAlive)
		return;

	if (iTargetLegit > 0)
	{
		float vecScreen[2];
		if (CalcScreen(vAimOriginLegit, vecScreen))
		{
			float Dist;
			float Distance = g_Player[iTargetLegit].pEnt->origin.Distance(g_Local.vOrigin) / 220.0f;
			if (Distance < 7.f)
				Dist = Distance;
			else
				Dist = 7.f;
			ImGui::GetWindowDrawList()->AddImage((GLuint*)texture_id[TARGET], ImVec2(vecScreen[0] - 7.5f + Dist, vecScreen[1] - 7.5f + Dist), ImVec2(vecScreen[0] + 7.5f - Dist, vecScreen[1] + 7.5f - Dist));
		}
	}
}

void KnifeBot(struct usercmd_s* cmd)
{
	if (IsCurWeaponKnife() && cvar.knifebot_active)
	{
		float flDist = 8192.f;

		for (unsigned int i = 0; i < 33; ++i)
		{
			if (idhook.FirstKillPlayer[i] == 1 || cvar.aim_id_mode == 0)
			{
				if (i == g_Local.iIndex)
					continue;

				if (!g_Player[i].bAlive)
					continue;

				if (!g_Player[i].bVisible)
					continue;

				if (!cvar.legit_team_attack && g_Player[i].iTeam == g_Local.iTeam)
					continue;

				if (!g_PlayerExtraInfoList[i].bBoneVisible[7])
					continue;

				if (g_Player[i].flDist < flDist || iTargetKnife == NULL)
				{
					flDist = g_Player[i].flDist;
					iTargetKnife = i;
				}
			}
		}

		if (cvar.aim_id_mode != 2 && iTargetKnife == 0)
		{
			for (unsigned int i = 0; i < 33; ++i)
			{
				if (idhook.FirstKillPlayer[i] < 2)
				{
					if (i == g_Local.iIndex)
						continue;

					if (!g_Player[i].bAlive)
						continue;

					if (!g_Player[i].bVisible)
						continue;

					if (!cvar.legit_team_attack && g_Player[i].iTeam == g_Local.iTeam)
						continue;

					if (!g_PlayerExtraInfoList[i].bBoneVisible[7])
						continue;

					if (g_Player[i].flDist < flDist || iTargetKnife == NULL)
					{
						flDist = g_Player[i].flDist;
						iTargetKnife = i;
					}
				}
			}
		}

		if (iTargetKnife)
		{
			int dist;
			if (cvar.knifebot_attack == 0)
				dist = cvar.knifebot_attack_distance;
			else if (cvar.knifebot_attack == 1)
				dist = cvar.knifebot_attack2_distance;

			vAimOriginKnife = g_PlayerExtraInfoList[iTargetKnife].vBone[7];

			if (vAimOriginKnife.Distance(g_Local.vOrigin) < dist)
			{
				if (CanAttack())
				{
					QAngle QAimAngles;

					VectorAngles(vAimOriginKnife - g_Local.vEye, QAimAngles);

					QAimAngles.Normalize();

					if (cvar.knifebot_silent)
					{
						MakeAngle(QAimAngles, cmd);
					}
					else
					{
						cmd->viewangles = QAimAngles;
						g_Engine.SetViewAngles(QAimAngles);
					}

					if (cvar.knifebot_attack == 0)
						cmd->buttons |= IN_ATTACK;
					else if (cvar.knifebot_attack == 1)
						cmd->buttons |= IN_ATTACK2;
				}
				else
				{
					if (cvar.knifebot_attack == 0)
						cmd->buttons &= ~IN_ATTACK;
					else if (cvar.knifebot_attack == 1)
						cmd->buttons &= ~IN_ATTACK2;
				}
			}
		}
	}
}

void KnifeDraw()
{
	if (!cvar.knifebot_active || !IsCurWeaponKnife() || !g_Local.bAlive)
		return;

	if (iTargetKnife > 0)
	{
		float vecScreen[2];
		if (CalcScreen(vAimOriginKnife, vecScreen))
		{
			float Dist;
			float Distance = g_Player[iTargetKnife].pEnt->origin.Distance(g_Local.vOrigin) / 220.0f;
			if (Distance < 7.f)
				Dist = Distance;
			else
				Dist = 7.f;
			ImGui::GetWindowDrawList()->AddImage((GLuint*)texture_id[TARGET], ImVec2(vecScreen[0] - 7.5f + Dist, vecScreen[1] - 7.5f + Dist), ImVec2(vecScreen[0] + 7.5f - Dist, vecScreen[1] + 7.5f - Dist));
		}
	}
}

void RageAimbot(struct usercmd_s* cmd)
{
	if (IsCurWeaponNonAttack())
		return;

	int m_iBone = -1;
	int m_iPoint = -1;

	float m_flBestFOV = 180;
	float m_flBestDist = 8192;

	for (unsigned int id = 0; id < 33; ++id)
	{
		if (idhook.FirstKillPlayer[id] == 1 || cvar.aim_id_mode == 0)
		{
			if (id == g_Local.iIndex)
				continue;

			if (!g_Player[id].bAlive)
				continue;

			if (!g_Player[id].bVisible)
				continue;

			if (!cvar.rage_team_attack && g_Player[id].iTeam == g_Local.iTeam)
				continue;

			if (!cvar.rage_shield_attack)
			{
				if (g_Player[id].pEnt->curstate.sequence == 97 || g_Player[id].pEnt->curstate.sequence == 98)
					continue;
			}

			std::deque<unsigned int> Bones;

			if (cvar.rage_bone == 1) Bones.push_front(7);
			if (cvar.rage_bone == 2) Bones.push_front(5);
			if (cvar.rage_bone == 3) Bones.push_front(2);
			if (cvar.rage_bone == 4)
			{
				if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
				{
					if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
					}
					else if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
					}
					else
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
				}
				else
				{
					if (g_Player[id].iTeam == 1)
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
					}
					if (g_Player[id].iTeam == 2)
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
					}
				}
			}

			if (Bones.empty())
				return;

			for (auto&& bones : Bones)
			{
				if (g_PlayerExtraInfoList[id].bBoneVisible[bones])
				{
					m_iBone = bones;
					break;
				}
			}

			if (m_iBone == -1)
			{
				for (auto&& bones : Bones)
				{
					if (cvar.rage_multipoint_bone > 0)//"None", "Only head", "Below head", "All"
					{
						if (cvar.rage_multipoint_bone == 1)
						{
							if (bones != 7)
								continue;
						}

						if (cvar.rage_multipoint_bone == 2)
						{
							if (bones == 7)
								continue;
						}

						for (unsigned int point = 0; point < 8; ++point)
						{
							if (g_PlayerExtraInfoList[id].bBonePointsVisible[bones][point] && !g_PlayerExtraInfoList[id].bBoneVisible[bones])
							{
								m_iPoint = point;
								m_iBone = bones;
								break;
							}
						}
					}
				}
			}

			if ((m_iBone < 0 || m_iBone > 51))
				continue;

			//"Field of view", "Distance", "Cycle"
			if (IsCurWeaponKnife())
			{
				if (g_Player[id].flDist < m_flBestDist)
				{
					m_flBestDist = g_Player[id].flDist;
					iTargetRage = id;
				}
			}
			else if (cvar.rage_target_selection == 0)
			{
				if (g_PlayerExtraInfoList[id].fBoneFOV[m_iBone] < m_flBestFOV)
				{
					m_flBestFOV = g_PlayerExtraInfoList[id].fBoneFOV[m_iBone];
					iTargetRage = id;
				}
			}
			else if (cvar.rage_target_selection == 1)
			{
				if (g_Player[id].flDist < m_flBestDist)
				{
					m_flBestDist = g_Player[id].flDist;
					iTargetRage = id;
				}
			}
			else if (cvar.rage_target_selection == 2)
			{
				if (g_PlayerExtraInfoList[id].fBoneFOV[m_iBone] < m_flBestFOV)
				{
					if (g_Player[id].flDist < m_flBestDist)
					{
						m_flBestFOV = g_PlayerExtraInfoList[id].fBoneFOV[m_iBone];
						m_flBestDist = g_Player[id].flDist;
						iTargetRage = id;
					}
				}
			}
		}
	}

	if (cvar.aim_id_mode != 2 && iTargetRage == 0)
	{
		for (unsigned int id = 0; id < 33; ++id)
		{
			if (idhook.FirstKillPlayer[id] < 2)
			{
				if (id == g_Local.iIndex)
					continue;

				if (!g_Player[id].bAlive)
					continue;

				if (!g_Player[id].bVisible)
					continue;

				if (!cvar.rage_team_attack && g_Player[id].iTeam == g_Local.iTeam)
					continue;

				if (!cvar.rage_shield_attack)
				{
					if (g_Player[id].pEnt->curstate.sequence == 97 || g_Player[id].pEnt->curstate.sequence == 98)
						continue;
				}
				std::deque<unsigned int> Bones;

				if (cvar.rage_bone == 1) Bones.push_front(7);
				if (cvar.rage_bone == 2) Bones.push_front(5);
				if (cvar.rage_bone == 3) Bones.push_front(2);
				if (cvar.rage_bone == 4)
				{
					if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
					{
						if (ModelArctic(id) || ModelLeet(id) || ModelSas(id))
						{
							for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
						}
						else if (ModelGuerilla(id) || ModelUrban(id) || ModelTerror(id) || ModelGign(id) || ModelGsg9(id))
						{
							for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
						}
						else
							for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
					}
					else
					{
						if (g_Player[id].iTeam == 1)
						{
							for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
						}
						if (g_Player[id].iTeam == 2)
						{
							for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
							for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
						}
					}
				}

				if (Bones.empty())
					return;

				for (auto&& bones : Bones)
				{
					if (g_PlayerExtraInfoList[id].bBoneVisible[bones])
					{
						m_iBone = bones;
						break;
					}
				}

				if (m_iBone == -1)
				{
					for (auto&& bones : Bones)
					{
						if (cvar.rage_multipoint_bone > 0)//"None", "Only head", "Below head", "All"
						{
							if (cvar.rage_multipoint_bone == 1)
							{
								if (bones != 7)
									continue;
							}

							if (cvar.rage_multipoint_bone == 2)
							{
								if (bones == 7)
									continue;
							}

							for (unsigned int point = 0; point < 8; ++point)
							{
								if (g_PlayerExtraInfoList[id].bBonePointsVisible[bones][point] && !g_PlayerExtraInfoList[id].bBoneVisible[bones])
								{
									m_iPoint = point;
									m_iBone = bones;
									break;
								}
							}
						}
					}
				}

				if (m_iBone < 0 || m_iBone > 51)
					continue;

				//"Field of view", "Distance", "Cycle"
				if (IsCurWeaponKnife())
				{
					if (g_Player[id].flDist < m_flBestDist)
					{
						m_flBestDist = g_Player[id].flDist;
						iTargetRage = id;
					}
				}
				else if (cvar.rage_target_selection == 0)
				{
					if (g_PlayerExtraInfoList[id].fBoneFOV[m_iBone] < m_flBestFOV)
					{
						m_flBestFOV = g_PlayerExtraInfoList[id].fBoneFOV[m_iBone];
						iTargetRage = id;
					}
				}
				else if (cvar.rage_target_selection == 1)
				{
					if (g_Player[id].flDist < m_flBestDist)
					{
						m_flBestDist = g_Player[id].flDist;
						iTargetRage = id;
					}
				}
				else if (cvar.rage_target_selection == 2)
				{
					if (g_PlayerExtraInfoList[id].fBoneFOV[m_iBone] < m_flBestFOV)
					{
						if (g_Player[id].flDist < m_flBestDist)
						{
							m_flBestFOV = g_PlayerExtraInfoList[id].fBoneFOV[m_iBone];
							m_flBestDist = g_Player[id].flDist;
							iTargetRage = id;
						}
					}
				}
			}
		}
	}

	if (iTargetRage)
	{
		QAngle QMyAngles, QAimAngles, QSmoothAngles;

		if (m_iPoint >= 0 && m_iPoint < 8)
			vAimOriginRage = g_PlayerExtraInfoList[iTargetRage].vBoneMulti[m_iBone][m_iPoint];
		else
			vAimOriginRage = g_PlayerExtraInfoList[iTargetRage].vBone[m_iBone];

		g_Engine.GetViewAngles(QMyAngles);

		VectorAngles(vAimOriginRage - g_Local.vEye, QAimAngles);

		static DWORD delay = 0;

		static int tickcount = 0;

		if (CanAttack())
		{
			if (GetTickCount() - delay > cvar.rage[g_Local.weapon.m_iWeaponID].rage_delay_shot)
			{
				if (cvar.rage_auto_fire)
				{
					if (IsCurWeaponKnife())
					{
						if (cvar.rage_knife_attack == 0)
							cmd->buttons |= IN_ATTACK;
						else if (cvar.rage_knife_attack == 1)
							cmd->buttons |= IN_ATTACK2;
					}
					else
					{
						cmd->buttons |= IN_ATTACK;
						if (cmd->buttons & IN_ATTACK)tickcount++;
						if (tickcount >= (!cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_type ? cvar.rage[g_Local.weapon.m_iWeaponID].rage_shot_count : rand() % (int)cvar.rage[g_Local.weapon.m_iWeaponID].rage_random_max + 1))
						{
							delay = GetTickCount();
							tickcount = 0;
						}
					}
				}
				if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2 && IsCurWeaponKnife())
				{
					if (cvar.rage_silent)
					{
						MakeAngle(QAimAngles, cmd);
					}
					else
					{
						cmd->viewangles = QAimAngles;
						g_Engine.SetViewAngles(QAimAngles);
					}
				}
			}
		}
		else
		{
			if (IsCurWeaponKnife())
			{
				if (cvar.rage_knife_attack == 0)
					cmd->buttons &= ~IN_ATTACK;
				else if (cvar.rage_knife_attack == 1)
					cmd->buttons &= ~IN_ATTACK2;
			}
			else
				cmd->buttons &= ~IN_ATTACK;
		}
	}
}

void RageDraw()
{
	if (IsCurWeaponNonAttack() || !g_Local.bAlive || !cvar.rage_active)
		return;

	if (iTargetRage)
	{
		float vecScreen[2];
		if (CalcScreen(vAimOriginRage, vecScreen))
		{
			float Dist;
			float Distance = g_Player[iTargetRage].pEnt->origin.Distance(g_Local.vOrigin) / 220.0f;
			if (Distance < 7.f)
				Dist = Distance;
			else
				Dist = 7.f;
			ImGui::GetWindowDrawList()->AddImage((GLuint*)texture_id[TARGET], ImVec2(vecScreen[0] - 7.5f + Dist, vecScreen[1] - 7.5f + Dist), ImVec2(vecScreen[0] + 7.5f - Dist, vecScreen[1] + 7.5f - Dist));
		}
	}
}

void DrawAimbot()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("AimbotDraw", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			if (cvar.rage_active)
				RageDraw();
			else
			{
				if (cvar.legit[g_Local.weapon.m_iWeaponID].active)
					LegitDraw();
				else
					TriggerDraw();

				KnifeDraw();
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}

void DrawAimBotFOV()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("DrawAimFov", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			if (g_Local.bAlive && IsCurWeaponGun() && !cvar.rage_active && cvar.legit[g_Local.weapon.m_iWeaponID].active && m_flCurrentFOV > 0 && m_flCurrentFOV <= 45 && g_Local.iFOV)
			{
				float x = ImGui::GetWindowSize().x * 0.5f;
				float y = ImGui::GetWindowSize().y * 0.5f;
				float dx = ImGui::GetWindowSize().x / g_Local.iFOV;
				float dy = ImGui::GetWindowSize().y / g_Local.iFOV;

				float radius = tanf(DEG2RAD(m_flCurrentFOV) * 0.5f) / tanf(DEG2RAD(g_Local.iFOV) * 0.5f) * ImGui::GetWindowSize().x;

				float positions[2];
				positions[0] = (x - (dx * g_Local.vNoRecoilAngle[1]));
				positions[1] = (y + (dy * g_Local.vNoRecoilAngle[0]));

				float ColorHSV[3];
				float ColorRGB[4];

				ImGui::ColorConvertRGBtoHSV(cvar.color_red, cvar.color_green, cvar.color_blue, ColorHSV[0], ColorHSV[1], ColorHSV[2]);
				for (int i = 1; i < (int)radius; i++)
				{
					ColorRGB[3] = 0.2f/radius * i;
					ColorHSV[0] += 1.0f / radius;
					if (ColorHSV[0] > 1.0f)
						ColorHSV[0] -= 1.0f;
					ImGui::ColorConvertHSVtoRGB(ColorHSV[0], ColorHSV[1], ColorHSV[2], ColorRGB[0], ColorRGB[1], ColorRGB[2]);
					ImGui::GetCurrentWindow()->DrawList->AddCircle(ImVec2(positions[0] + 0.5, positions[1]), i, ImColor(ColorRGB[0], ColorRGB[1], ColorRGB[2], ColorRGB[3]), 32);
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}

void AimBot(float frametime, struct usercmd_s* cmd)
{
	iTargetLegit = 0;
	iTargetKnife = 0;
	iTargetRage = 0;
	if (g_Local.bAlive)
	{
		if (cvar.rage_active)
			RageAimbot(cmd);
		else
		{
			if (cvar.legit[g_Local.weapon.m_iWeaponID].active)
				LegitAimbot(cmd);
			else
				TriggerAimbot(cmd);

			KnifeBot(cmd);
		}
	}
}

void UTIL_TextureHit(Vector vecSrc, Vector vecEnd, Vector vecDir, int ignore, pmtrace_t& tr)
{
	if (vecSrc[0] == vecEnd[0] && vecSrc[1] == vecEnd[1] && vecSrc[2] == vecEnd[2])
	{
		memset(&tr, NULL, sizeof(pmtrace_t));

		tr.endpos = vecEnd;
		tr.fraction = 1.0f;
		return;
	}

	EV_SetTraceHull(2);
	EV_PlayerTrace(vecSrc, vecEnd, PM_GLASS_IGNORE, ignore, &tr);

	if (tr.fraction == 0 && tr.startsolid && !tr.allsolid)
	{
		Vector vecTmp = vecSrc;

		while (!tr.allsolid && tr.fraction == 0.f)
		{
			vecTmp = vecTmp + vecDir;

			EV_SetTraceHull(2);
			EV_PlayerTrace(vecTmp, vecEnd, PM_WORLD_ONLY, ignore, &tr);
		}

		if (!tr.allsolid && tr.fraction != 1.0f)
		{
			vecTmp = vecEnd - vecSrc;

			float Length1 = vecTmp.Length();

			vecTmp = tr.endpos - vecSrc;

			float Length2 = vecTmp.Length();

			tr.fraction = Length1 / Length2;
			tr.startsolid = 1.0f;
		}
	}

	if (tr.allsolid)
		tr.fraction = 1.0f;
}

int FireBullets(Vector start, Vector end, float flDistance, int iOriginalPenetration, int iBulletType, int iDamage, float flRangeModifier)
{
	Vector vecSrc, vecEnd, vecDir, vecTmp;
	int iPenetration = iOriginalPenetration + 1;
	int iPenetrationPower;
	int iCurrentDamage = iDamage;
	float flPenetrationDistance;
	float flCurrentDistance;

	pmtrace_t tr;
	vecSrc = start;
	vecEnd = end;

	switch (iBulletType)
	{
	case BULLET_PLAYER_9MM:
		iPenetrationPower = 21;
		flPenetrationDistance = 800;
		break;
	case BULLET_PLAYER_45ACP:
		iPenetrationPower = 15;
		flPenetrationDistance = 500;
		break;
	case BULLET_PLAYER_50AE:
		iPenetrationPower = 30;
		flPenetrationDistance = 1000;
		break;
	case BULLET_PLAYER_762MM:
		iPenetrationPower = 39;
		flPenetrationDistance = 5000;
		break;
	case BULLET_PLAYER_556MM:
		iPenetrationPower = 35;
		flPenetrationDistance = 4000;
		break;
	case BULLET_PLAYER_338MAG:
		iPenetrationPower = 45;
		flPenetrationDistance = 8000;
		break;
	case BULLET_PLAYER_57MM:
		iPenetrationPower = 30;
		flPenetrationDistance = 2000;
		break;
	case BULLET_PLAYER_357SIG:
		iPenetrationPower = 25;
		flPenetrationDistance = 800;
		break;
	default:
		iPenetrationPower = 0;
		flPenetrationDistance = 0;
		break;
	}

	vecDir = vecEnd - vecSrc;

	float Length = vecDir.Length();

	vecDir /= Length;

	vecEnd = vecDir * flDistance + vecSrc;

	float flDamageModifier = 0.5;

	while (iPenetration)
	{
		UTIL_TextureHit(vecSrc, vecEnd, vecDir, -1, tr);

		char cTextureType = GetTextureType(&tr, vecSrc, vecEnd);

		switch (cTextureType)
		{
		case CHAR_TEX_CONCRETE:
			iPenetrationPower *= 0.25;
			break;
		case CHAR_TEX_GRATE:
			iPenetrationPower *= 0.5;
			flDamageModifier = 0.4;
			break;
		case CHAR_TEX_METAL:
			iPenetrationPower *= 0.15;
			flDamageModifier = 0.2;
			break;
		case CHAR_TEX_COMPUTER:
			iPenetrationPower *= 0.4;
			flDamageModifier = 0.45;
			break;
		case CHAR_TEX_TILE:
			iPenetrationPower *= 0.65;
			flDamageModifier = 0.3;
			break;
		case CHAR_TEX_VENT:
			iPenetrationPower *= 0.5;
			flDamageModifier = 0.45;
			break;
		case CHAR_TEX_WOOD:
			flDamageModifier = 0.6;
			break;
		default:
			break;
		}

		if (tr.fraction != 1.0)
		{
			iPenetration--;

			vecTmp = tr.endpos - start;

			float tmplen = vecTmp.Length();

			if (tmplen >= Length)
			{
				vecTmp = end - vecSrc;

				float Length1 = vecTmp.Length();

				vecTmp = vecEnd - vecSrc;

				float Length2 = vecTmp.Length();

				tr.fraction = Length1 / Length2;

				flCurrentDistance = flDistance * tr.fraction;

				iCurrentDamage *= pow(flRangeModifier, flCurrentDistance / 500);

				return iCurrentDamage;
			}

			flCurrentDistance = flDistance * tr.fraction;

			iCurrentDamage *= pow(flRangeModifier, flCurrentDistance / 500);

			if (flCurrentDistance > flPenetrationDistance)
				iPenetration = 0;

			if (iPenetration)
			{
				vecSrc = iPenetrationPower * vecDir + tr.endpos;

				flDistance = (flDistance - flCurrentDistance) * 0.5;

				vecEnd = vecDir * flDistance + vecSrc;

				flCurrentDistance = iCurrentDamage;

				iCurrentDamage = flCurrentDistance * flDamageModifier;
			}
			else
			{
				vecSrc = 42 * vecDir + tr.endpos;

				flDistance = (flDistance - flCurrentDistance) * 0.75;

				vecEnd = vecDir * flDistance + vecSrc;

				iCurrentDamage *= 0.75;
			}
		}
		else iPenetration = 0;
	}
	return 0;
}

bool FakeEdge(float& angle)
{
	float edge_dist = 128;
	Vector position = g_Local.vEye;
	float closest_distance = 100.0f;
	float radius = edge_dist + 0.1f;
	float step = M_PI * 2.0 / 8;

	for (float a = 0; a < (M_PI * 2.0); a += step)
	{
		Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		pmtrace_t tr;

		EV_SetTraceHull(2);
		EV_PlayerTrace(position, location, PM_GLASS_IGNORE, -1, &tr);

		float distance = position.Distance(tr.endpos);

		if (distance < closest_distance)
		{
			closest_distance = distance;
			angle = RAD2DEG(a);
		}
	}
	return closest_distance < edge_dist;
}

void AntiAim(struct usercmd_s* cmd)
{
	if (cvar.rage_active && g_Local.bAlive)
	{
		int m_Use = (cmd->buttons & IN_USE);
		int m_InAttack = (cmd->buttons & IN_ATTACK);

		if (!m_Use && !(m_InAttack && CanAttack()))
		{
			FixMoveStart(cmd);

			int id = NULL;
			float flDist = 8192.f;

			for (unsigned int i = 0; i < 33; i++)
			{
				if (i == g_Local.iIndex)
					continue;

				if (!g_Player[id].bAlive)
					continue;

				if (!cvar.rage_team_attack && g_Player[i].iTeam == g_Local.iTeam)
					continue;

				if (g_Player[i].flDist < flDist || id == NULL)
				{
					flDist = g_Player[i].flDist;
					id = i;
				}
			}

			Vector vAngles = cmd->viewangles;

			if (id > 0)
				VectorAngles(g_Player[id].pEnt->origin - g_Local.vEye, vAngles);

			//Yaw
			if (g_Local.flVelocity2D > 0)
			{
				if (cvar.aa_yaw_while_running > 0)
				{
					if (cvar.aa_yaw_while_running == 1)
					{//180
						cmd->viewangles.y = vAngles[1] + 180;
					}
					else if (cvar.aa_yaw_while_running == 2)
					{//180 Jitter
						static bool jitter = false;

						if (jitter)
							cmd->viewangles.y = vAngles[1] + 180;
						else
							cmd->viewangles.y = vAngles[1];

						jitter = !jitter;
					}
					else if (cvar.aa_yaw_while_running == 3)
					{//Spin
						int spin = 30;
						cmd->viewangles.y = fmod(g_Engine.GetClientTime() * spin * 360.0f, 360.0f);
					}
					else if (cvar.aa_yaw_while_running == 4)
					{//Jitter
						static unsigned int m_side = 0;

						if (m_side == 0)
							cmd->viewangles.y = vAngles[1] + 0;
						else if (m_side == 1)
							cmd->viewangles.y = vAngles[1] + 90;
						else if (m_side == 2)
							cmd->viewangles.y = vAngles[1] + 180;
						else if (m_side >= 3) {
							cmd->viewangles.y = vAngles[1] + -90;
							m_side = 0;
						}

						m_side++;
					}
					else if (cvar.aa_yaw_while_running == 5) {//Sideway
						static bool jitter = false;

						if (jitter)
							cmd->viewangles.y = vAngles[1] + 90;
						else
							cmd->viewangles.y = vAngles[1] - 90;

						jitter = !jitter;
					}
					else if (cvar.aa_yaw_while_running == 6) {//Random
						cmd->viewangles.y = rand() % 361;
						if (cmd->viewangles.y > 180)
							cmd->viewangles.y -= 360;
					}
					else if (cvar.aa_yaw_while_running == 7) {//Static
						cmd->viewangles.y = vAngles[1] + cvar.aa_yaw_static;
					}
				}

				float angle = cmd->viewangles.y;

				if ((cvar.aa_edge == 2 || cvar.aa_edge == 3) && FakeEdge(angle))
					cmd->viewangles.y = angle;
			}
			else
			{
				if (cvar.aa_yaw > 0)
				{
					if (cvar.aa_yaw == 1)
					{//180
						cmd->viewangles.y = vAngles[1] + 180;
					}
					else if (cvar.aa_yaw == 2)
					{//180 Jitter
						static bool jitter = false;

						if (jitter)
							cmd->viewangles.y = vAngles[1] + 180;
						else
							cmd->viewangles.y = vAngles[1];

						jitter = !jitter;
					}
					else if (cvar.aa_yaw == 3)
					{//Spin
						int spin = 30;
						cmd->viewangles.y = fmod(g_Engine.GetClientTime() * spin * 360.0f, 360.0f);
					}
					else if (cvar.aa_yaw == 4)
					{//Jitter
						static unsigned int m_side = 0;

						if (m_side == 0)
							cmd->viewangles.y = vAngles[1] + 0;
						else if (m_side == 1)
							cmd->viewangles.y = vAngles[1] + 90;
						else if (m_side == 2)
							cmd->viewangles.y = vAngles[1] + 180;
						else if (m_side >= 3) {
							cmd->viewangles.y = vAngles[1] + -90;
							m_side = 0;
						}

						m_side++;
					}
					else if (cvar.aa_yaw == 5)
					{//Sideway
						static bool jitter = false;

						if (jitter)
							cmd->viewangles.y = vAngles[1] + 90;
						else
							cmd->viewangles.y = vAngles[1] - 90;

						jitter = !jitter;
					}
					else if (cvar.aa_yaw == 6)
					{//Random
						cmd->viewangles.y = rand() % 361;
						if (cmd->viewangles.y > 180)
							cmd->viewangles.y -= 360;
					}
					else if (cvar.aa_yaw == 7)
					{//Static
						cmd->viewangles.y = vAngles[1] + cvar.aa_yaw_static;
					}
				}

				float angle = cmd->viewangles.y;

				if ((cvar.aa_edge == 1 || cvar.aa_edge == 3 || cvar.aa_edge == 4) && FakeEdge(angle))
				{
					static float timer = g_Local.weapon.curtime;

					if (g_Local.weapon.curtime - timer > 3 && cvar.aa_edge == 4)
					{
						timer = g_Local.weapon.curtime;

						cmd->viewangles.y = angle + 180;
					}
					else {
						cmd->viewangles.y = angle;
					}
				}
			}

			//Pitch
			if (cvar.aa_pitch > 0)
			{
				if (cvar.aa_pitch == 1)
				{//Fakedown
					cmd->viewangles.x = 180;
				}
				if (cvar.aa_pitch == 2)
				{//Random
					cmd->viewangles.x = rand() % 361;
					if (cmd->viewangles.x > 180)
						cmd->viewangles.x -= 360;
				}
			}

			FixMoveEnd(cmd);
		}
	}
}

void ContinueFire(struct usercmd_s* cmd)
{
	if (cvar.rage_active && cmd->buttons & IN_ATTACK && IsCurWeaponPistol() && !g_Local.weapon.m_iInReload && g_Local.bAlive)
	{
		static bool bFire = false;

		if (CanAttack() && bFire)
		{
			cmd->buttons |= IN_ATTACK;
			bFire = false;
		}
		else if (!bFire)
		{
			cmd->buttons &= ~IN_ATTACK;
			bFire = true;
		}
	}
}

void FastZoom(struct usercmd_s* cmd)
{
	if (!cvar.rage_active && cvar.legit_fast_zoom && IsCurWeaponSniper() && g_Local.iFOV == 90 && cmd->buttons & IN_ATTACK && g_Local.bAlive)
	{
		cmd->buttons &= ~IN_ATTACK;
		cmd->buttons |= IN_ATTACK2;
	}
}

void UpdateVisibility(int i)
{
	g_Player[i].bVisible = false;

	bool teammates = false;
	bool walls = false;

	if (cvar.rage_team_attack || cvar.legit_team_attack)
		teammates = true;

	if (((cvar.rage_active && cvar.rage_penetration) || (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_active && cvar.legit[g_Local.weapon.m_iWeaponID].trigger_penetration)) && IsCurWeaponGun())
		walls = true;

	if (!teammates && g_Player[i].iTeam == g_Local.iTeam)
		return;

	if (!g_Player[i].bGotPlayer)
		return;

	std::deque<unsigned int> Bones;

	if (!IsCurWeaponNonAttack())
	{
		if (cvar.rage_active)
		{
			if (cvar.rage_bone == 1) Bones.push_front(7);
			if (cvar.rage_bone == 2) Bones.push_front(5);
			if (cvar.rage_bone == 3) Bones.push_front(2);
			if (cvar.rage_bone == 4)
			{
				if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
				{
					if (ModelArctic(i) || ModelLeet(i) || ModelSas(i))
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
					}
					else if (ModelGuerilla(i) || ModelUrban(i) || ModelTerror(i) || ModelGign(i) || ModelGsg9(i))
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
					}
					else
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
				}
				else
				{
					if (g_Player[i].iTeam == 1)
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack1[i]);
					}
					if (g_Player[i].iTeam == 2)
					{
						for (unsigned int i = 0; i < 7; i++) Bones.push_front(RageBoneFront[i]);
						for (unsigned int i = 0; i < 14; i++) Bones.push_back(RageBoneBack2[i]);
					}
				}
			}
		}
		else
		{
			if (!IsCurWeaponKnife())
			{
				if (cvar.legit[g_Local.weapon.m_iWeaponID].active)
				{
					if (cvar.legit[g_Local.weapon.m_iWeaponID].stomach_bone) Bones.push_front(2);
					if (cvar.legit[g_Local.weapon.m_iWeaponID].chest_bone)
					{
						Bones.push_back(1);
						for (unsigned int id = 0; id < 4; id++) Bones.push_front(LegitBone[id]);
					}
					if (cvar.legit[g_Local.weapon.m_iWeaponID].head_bone) Bones.push_front(7);
					if (cvar.legit[g_Local.weapon.m_iWeaponID].all_bone)
					{
						if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
						{
							if (ModelArctic(i) || ModelLeet(i) || ModelSas(i))
								for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
							if (ModelGuerilla(i) || ModelUrban(i) || ModelTerror(i) || ModelGign(i) || ModelGsg9(i))
								for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
						}
						else
						{
							if (g_Player[i].iTeam == 1)
								for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone1[i]);
							if (g_Player[i].iTeam == 2)
								for (unsigned int i = 0; i < 14; i++) Bones.push_back(LegitBone2[i]);
						}
					}
				}
				else
				{
					if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_active)
					{
						if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_head) Bones.push_back(7);
						if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_chest) Bones.push_back(5);
						if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_stomach) Bones.push_back(2);
						if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_bone_all)
						{
							if (g_pEngine->pfnGetCvarPointer("cl_minmodels")->value == 0)
							{
								if (ModelArctic(i) || ModelLeet(i) || ModelSas(i))
									for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
								else if (ModelGuerilla(i) || ModelUrban(i) || ModelTerror(i) || ModelGign(i) || ModelGsg9(i))
									for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
								else
									for (unsigned int i = 0; i < 4; i++) Bones.push_back(TriggerBone[i]);
							}
							else
							{
								if (g_Player[i].iTeam == 1)
									for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone1[i]);
								if (g_Player[i].iTeam == 2)
									for (unsigned int i = 0; i < 18; i++) Bones.push_back(TriggerBone2[i]);
							}
						}
					}
				}
			}
			else if (cvar.knifebot_active) Bones.push_front(7);
		}
	}

	if (Bones.empty())
		return;

	pmtrace_t tr;
	int detect = 0;

	for (auto&& bones : Bones)
	{
		EV_SetTraceHull(2);

		if (cvar.bypass_trace_blockers)
			EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBone[bones], PM_WORLD_ONLY, -1, &tr);
		else
			EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBone[bones], PM_GLASS_IGNORE, -1, &tr);

		detect = g_Engine.pEventAPI->EV_IndexFromTrace(&tr);

		if ((cvar.bypass_trace_blockers && tr.fraction == 1 && !detect) || (!cvar.bypass_trace_blockers && detect == i))
		{
			g_Player[i].bVisible = true;
			g_PlayerExtraInfoList[i].bBoneVisible[bones] = true;
		}
		else
		{
			g_PlayerExtraInfoList[i].bBoneVisible[bones] = false;

			int iOriginalPenetration = CurPenetration();

			if (iOriginalPenetration && walls)
			{
				int iDamage = CurDamage();
				int iBulletType = CurBulletType();
				float flDistance = CurDistance();
				float flRangeModifier = CurWallPierce();

				int iCurrentDamage = FireBullets(g_Local.vEye, g_PlayerExtraInfoList[i].vBone[bones], flDistance, iOriginalPenetration, iBulletType, iDamage, flRangeModifier);

				if (iCurrentDamage > 0)
				{
					g_Player[i].bVisible = true;
					g_PlayerExtraInfoList[i].bBoneVisible[bones] = true;
				}
			}
		}

		if (cvar.rage_active && cvar.rage_multipoint_bone > 0 && IsCurWeaponGun())
		{
			if (cvar.rage_multipoint_bone == 1)
			{
				if (bones != 7)
					continue;
			}

			if (cvar.rage_multipoint_bone == 2)
			{
				if (bones == 7)
					continue;
			}

			for (unsigned int point = 0; point < 8; ++point)
			{
				EV_SetTraceHull(2);

				if (cvar.bypass_trace_blockers)
					EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], PM_WORLD_ONLY, -1, &tr);
				else
					EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], PM_GLASS_IGNORE, -1, &tr);

				detect = g_Engine.pEventAPI->EV_IndexFromTrace(&tr);

				if ((cvar.bypass_trace_blockers && tr.fraction == 1 && !detect) || (!cvar.bypass_trace_blockers && detect == i))
				{
					g_Player[i].bVisible = true;
					g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = true;
				}
				else
				{
					g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = false;

					int iOriginalPenetration = CurPenetration();

					if (iOriginalPenetration && walls)
					{
						int iDamage = CurDamage();
						int iBulletType = CurBulletType();
						float flDistance = CurDistance();
						float flRangeModifier = CurWallPierce();

						int iCurrentDamage = FireBullets(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], flDistance, iOriginalPenetration, iBulletType, iDamage, flRangeModifier);

						if (iCurrentDamage > 0)
						{
							g_Player[i].bVisible = true;
							g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = true;
						}
					}
				}
			}
		}

		if (cvar.legit[g_Local.weapon.m_iWeaponID].trigger_active && !cvar.rage_active && !cvar.legit[g_Local.weapon.m_iWeaponID].active)
		{
			for (unsigned int point = 0; point < 8; ++point)
			{
				EV_SetTraceHull(2);

				if (cvar.bypass_trace_blockers)
					EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], PM_WORLD_ONLY, -1, &tr);
				else
					EV_PlayerTrace(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], PM_GLASS_IGNORE, -1, &tr);

				detect = g_Engine.pEventAPI->EV_IndexFromTrace(&tr);

				if ((cvar.bypass_trace_blockers && tr.fraction == 1 && !detect) || (!cvar.bypass_trace_blockers && detect == i))
				{
					g_Player[i].bVisible = true;
					g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = true;
				}
				else
				{
					g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = false;

					int iOriginalPenetration = CurPenetration();

					if (iOriginalPenetration && walls)
					{
						int iDamage = CurDamage();
						int iBulletType = CurBulletType();
						float flDistance = CurDistance();
						float flRangeModifier = CurWallPierce();

						int iCurrentDamage = FireBullets(g_Local.vEye, g_PlayerExtraInfoList[i].vBoneMulti[bones][point], flDistance, iOriginalPenetration, iBulletType, iDamage, flRangeModifier);

						if (iCurrentDamage > 0)
						{
							g_Player[i].bVisible = true;
							g_PlayerExtraInfoList[i].bBonePointsVisible[bones][point] = true;
						}
					}
				}
			}
		}
	}
}

void UpdateAimbot(float frametime)
{
	g_Local.flFrametime = frametime;
	for (unsigned int i = 0; i < 33; ++i)
	{
		if (g_Player[i].bAlive && i != g_Local.iIndex && g_Local.bAlive)
			UpdateVisibility(i);
	}
}