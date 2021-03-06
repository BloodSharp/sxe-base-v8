#include "client.h"

void V_CalcRefdefRecoil(struct ref_params_s *pparams)
{
	if (IsCurWeaponGun())
	{
		if (cvar.rage_active)
		{
			g_Local.vNoRecoilAngle[0] = pparams->punchangle[0] * cvar.rage[g_Local.weapon.m_iWeaponID].recoil;
			g_Local.vNoRecoilAngle[1] = pparams->punchangle[1] * cvar.rage[g_Local.weapon.m_iWeaponID].recoil;
			g_Local.vNoRecoilAngle[2] = 0;

			pparams->punchangle[0] = 0;
			pparams->punchangle[1] = 0;
			pparams->punchangle[2] = 0;
		}
		else
		{
			g_Local.vNoRecoilAngle[0] = pparams->punchangle[0] * 2;
			g_Local.vNoRecoilAngle[1] = pparams->punchangle[1] * 2;
			g_Local.vNoRecoilAngle[2] = 0;
		}
	}
	
}

void NoRecoil(struct usercmd_s *cmd)
{
	if (cvar.rage_active && IsCurWeaponGun() && g_Local.bAlive && cmd->buttons & IN_ATTACK && CanAttack())
		cmd->viewangles -= g_Local.vNoRecoilAngle;
}