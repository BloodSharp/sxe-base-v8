#include "client.h"

glBegin_t			pglBegin;
glPopMatrix_t	 	pglPopMatrix;
glPushMatrix_t	 	pglPushMatrix;
glClear_t			pglClear;
glEnable_t			pglEnable;
glDisable_t			pglDisable;
glEnd_t				pglEnd;
glPolygonOffset_t	pglPolygonOffset;
glTranslatef_t		pglTranslatef;
glVertex2f_t		pglVertex2f;
glVertex3f_t	 	pglVertex3f;
glVertex3fv_t	 	pglVertex3fv;
glViewport_t	 	pglViewport;
wglSwapBuffers_t 	pwglSwapBuffers;
glShadeModel_t	 	pglShadeModel;
glFrustum_t         pglFrustum;
glBlendFunc_t       pglBlendFunc;
glColor4f_t         pglColor4f;
glReadPixels_t      pglReadPixels;

void APIENTRY hooked_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if (chams_render_chicken)
	{
		if (cvar.chams_chicken == 1 || cvar.chams_chicken == 3)
		{
			red = chams_render_chicken_r;
			green = chams_render_chicken_g;
			blue = chams_render_chicken_b;
		}
		if (cvar.chams_chicken == 2)
		{
			red = (chams_render_chicken_r)* red;
			green = (chams_render_chicken_g)* green;
			blue = (chams_render_chicken_b)* blue;
		}
	}
	if (chams_render_weapon)
	{
		if (cvar.chams_weapon == 1 || cvar.chams_weapon == 3)
		{
			red = chams_render_weapon_r;
			green = chams_render_weapon_g;
			blue = chams_render_weapon_b;
		}
		if (cvar.chams_weapon == 2)
		{
			red = (chams_render_weapon_r)* red;
			green = (chams_render_weapon_g)* green;
			blue = (chams_render_weapon_b)* blue;
		}
	}
	if (chams_render_viewmodel)
	{
		if (cvar.chams_view_model == 1 || cvar.chams_view_model == 3)
		{
			red = chams_render_viewmodel_r;
			green = chams_render_viewmodel_g;
			blue = chams_render_viewmodel_b;
		}
		if (cvar.chams_view_model == 2)
		{
			red = (chams_render_viewmodel_r)* red;
			green = (chams_render_viewmodel_g)* green;
			blue = (chams_render_viewmodel_b)* blue;
		}
	}
	if (chams_render_third_person)
	{
		if (cvar.chams_third_person == 1 || cvar.chams_third_person == 3)
		{
			red = chams_render_third_person_r;
			green = chams_render_third_person_g;
			blue = chams_render_third_person_b;
		}
		if (cvar.chams_third_person == 2)
		{
			red = (chams_render_third_person_r)* red;
			green = (chams_render_third_person_g)* green;
			blue = (chams_render_third_person_b)* blue;
		}
	}
	if (chams_render)
	{
		if (cvar.chams_player == 1 || cvar.chams_player == 3)
		{
			red = chams_render_r;
			green = chams_render_g;
			blue = chams_render_b;
		}
		if (cvar.chams_player == 2)
		{
			red = (chams_render_r)* red;
			green = (chams_render_g)* green;
			blue = (chams_render_b)* blue;
		}
	}
	pglColor4f(red, green, blue, alpha);
}

void APIENTRY hooked_glBegin(GLenum mode)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	bool Player = ent && ent->player;
	bool Weapon = ent && ent->model && (strstr(ent->model->name, "p_") || strstr(ent->model->name, "w_"));
	bool View_Model = ent && ent->model && strstr(ent->model->name, "v_");

	if (cvar.esp_wall)
	{
		if ((Player || Weapon))
			glDepthRange(0, 0.5);
		else
			glDepthRange(0.5, 1);
	}

	if (cvar.esp_lambert && (Player && !cvar.chams_player_glow || Weapon && !cvar.chams_weapon_glow || View_Model && !cvar.chams_view_model_glow))
	{
		if (mode == GL_TRIANGLE_STRIP)
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

	if (CheckDrawEngine() && g_pEngine->GetMaxClients())
	{
		GetTextureModel();

		if (cvar.esp_skins)
		{
			for (int i = 0; i < 8; i++)
			{
				if (last_texture == TexPlayerIndex[i])
					glBindTexture(GL_TEXTURE_2D, texture_id[169 + i]);
			}

			for (int i = 0; i < 5; i++)
			{
				if (last_texture == TexBackpack1Index[i])
					glBindTexture(GL_TEXTURE_2D, texture_id[178]);
				if (last_texture == TexBackpack2Index[i])
					glBindTexture(GL_TEXTURE_2D, texture_id[179]);
			}

			for (int i = 0; i < 11; i++)
			{
				if (i < 8)
				{
					if (last_texture == TexAk47Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[35 + i]);
				}
				else
				{
					if (last_texture == TexAk47Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[180]);
				}
			}

			for (int i = 0; i < 1; i++)
			{
				if (last_texture == TexKevlarIndex[i])
					glBindTexture(GL_TEXTURE_2D, texture_id[217]);
			}

			for (int i = 0; i < 6; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexAugIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[67 + i]);
				}
				else
				{
					if (last_texture == TexAugIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[184]);
				}
			}

			for (int i = 0; i < 13; i++)
			{
				if (i < 10)
				{
					if (last_texture == TexAwpIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[43 + i]);
				}
				else
				{
					if (last_texture == TexAwpIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[185]);
				}
			}

			for (int i = 0; i < 11; i++)
			{
				if (i < 5)
				{
					if (last_texture == TexC4Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[70 + i]);
				}
				else if (i < 8)
				{
					if (last_texture == TexC4Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 5 ? 70 : i == 6 ? 72 : 73]);
				}
				else
				{
					if (last_texture == TexC4Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 8 ? 70 : i == 9 ? 72 : 73]);
				}
			}

			for (int i = 0; i < 9; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexDeagleIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[59 + i]);
				}
				else if (i < 6)
				{
					if (last_texture == TexDeagleIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[186]);
				}
				else
				{
					if (last_texture == TexDeagleIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 6 ? 59 : i == 7 ? 60 : 61]);
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (i < 4)
				{
					if (last_texture == TexEliteIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[85 + i]);
				}
				else
				{
					if (last_texture == TexEliteIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[187]);
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (i < 1)
				{
					if (last_texture == TexFamasIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[89]);
				}
				else
				{
					if (last_texture == TexFamasIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[188]);
				}
			}

			for (int i = 0; i < 7; i++)
			{
				if (i < 2)
				{
					if (last_texture == TexFiveSevenIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[90 + i]);
				}
				else if (i < 5)
				{
					if (last_texture == TexFiveSevenIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[189]);
				}
				else
				{
					if (last_texture == TexFiveSevenIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 5 ? 90 : 91]);
				}
			}

			for (int i = 0; i < 12; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexFlashBangIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[81 + 1]);
				}
				else if (i < 5)
				{
					if (last_texture == TexFlashBangIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 3 ? 190 : 191]);
				}
				else if (i < 7)
				{
					if (last_texture == TexFlashBangIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 5 ? 190 : 191]);
				}
				else if (i < 9)
				{
					if (last_texture == TexFlashBangIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 7 ? 190 : 191]);
				}
				else
				{
					if (last_texture == TexFlashBangIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 9 ? 81 : i == 10 ? 82 : 83]);
				}
			}

			for (int i = 0; i < 11; i++)
			{
				if (i < 8)
				{
					if (last_texture == TexG3SG1Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[92 + i]);
				}
				else
				{
					if (last_texture == TexG3SG1Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[196]);
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (i < 1)
				{
					if (last_texture == TexGalilIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[100]);
				}
				else
				{
					if (last_texture == TexGalilIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[197]);
				}
			}

			for (int i = 0; i < 11; i++)
			{
				if (i < 4)
				{
					if (last_texture == TexGlockIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[101 + i]);
				}
				else if (i < 7)
				{
					if (last_texture == TexGlockIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[198]);
				}
				else
				{
					if (last_texture == TexGlockIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 7 ? 101 : i == 8 ? 102 : i == 9 ? 103 : 104]);
				}
			}

			for (int i = 0; i < 12; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexHeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[78 + i]);
				}
				else if (i < 5)
				{
					if (last_texture == TexHeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 3 ? 192 : 193]);
				}
				else if (i < 7)
				{
					if (last_texture == TexHeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 5 ? 192 : 193]);
				}
				else if (i < 9)
				{
					if (last_texture == TexHeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 7 ? 192 : 193]);
				}
				else
				{
					if (last_texture == TexHeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 9 ? 78 : i == 10 ? 79 : 80]);
				}
			}

			for (int i = 0; i < 3; i++)
			{
				if (i < 1)
				{
					if (last_texture == TexKnifeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[31]);
				}
				else if (i < 2)
				{
					if (last_texture == TexKnifeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[199]);
				}
				else if (i < 3)
				{
					if (last_texture == TexKnifeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[31]);
				}
			}

			for (int i = 0; i < 7; i++)
			{
				if (i < 4)
				{
					if (last_texture == TexM3Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[105 + i]);
				}
				else
				{
					if (last_texture == TexM3Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[200]);
				}
			}

			for (int i = 0; i < 9; i++)
			{
				if (i < 6)
				{
					if (last_texture == TexM4a1Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[53 + i]);
				}
				else
				{
					if (last_texture == TexM4a1Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[201]);
				}
			}

			for (int i = 0; i < 12; i++)
			{
				if (i < 9)
				{
					if (last_texture == TexM249Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[109 + i]);
				}
				else
				{
					if (last_texture == TexM249Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[202]);
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexMac10Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[118 + i]);
				}
				else
				{
					if (last_texture == TexMac10Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[203]);
				}
			}

			for (int i = 0; i < 10; i++)
			{
				if (i < 7)
				{
					if (last_texture == TexMp5Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[121 + i]);
				}
				else
				{
					if (last_texture == TexMp5Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[204]);
				}
			}

			for (int i = 0; i < 8; i++)
			{
				if (i < 5)
				{
					if (last_texture == TexP90Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[128 + i]);
				}
				else
				{
					if (last_texture == TexP90Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[205]);
				}
			}

			for (int i = 0; i < 9; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexP228Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[133 + i]);
				}
				else if (i < 6)
				{
					if (last_texture == TexP228Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[206]);
				}
				else
				{
					if (last_texture == TexP228Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 6 ? 133 : i == 7 ? 134 : 135]);
				}
			}

			for (int i = 0; i < 8; i++)
			{
				if (i < 5)
				{
					if (last_texture == TexScoutIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[62 + i]);
				}
				else
				{
					if (last_texture == TexScoutIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[207]);
				}
			}

			for (int i = 0; i < 9; i++)
			{
				if (i < 6)
				{
					if (last_texture == TexSG550Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[136 + i]);
				}
				else
				{
					if (last_texture == TexSG550Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[208]);
				}
			}

			for (int i = 0; i < 8; i++)
			{
				if (i < 5)
				{
					if (last_texture == TexSG552Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[142 + i]);
				}
				else
				{
					if (last_texture == TexSG552Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[209]);
				}
			}

			for (int i = 0; i < 47; i++)
			{
				if (i == 30 || i == 32 || i == 34 || i == 36 || i == 38 || i == 40 || i == 41 || i == 43 || i == 45)
				{
					if (last_texture == TexShieldIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[84]);
				}
				if (i == 31 || i == 33 || i == 35 || i == 37 || i == 39 || i == 42 || i == 44 || i == 46)
				{
					if (last_texture == TexShieldIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[212]);
				}
				if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15 || i == 18 || i == 21 || i == 24 || i == 27)
				{
					if (last_texture == TexShieldIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[210]);
				}
				if (i == 1 || i == 4 || i == 7 || i == 10 || i == 13 || i == 16 || i == 19 || i == 22 || i == 25 || i == 28)
				{
					if (last_texture == TexShieldIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[211]);
				}
				if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14 || i == 17 || i == 20 || i == 23 || i == 26 || i == 29)
				{
					if (last_texture == TexShieldIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[212]);
				}
			}

			for (int i = 0; i < 12; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexSmokeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[75 + 1]);
				}
				else if (i < 5)
				{
					if (last_texture == TexSmokeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 3 ? 194 : 195]);
				}
				else if (i < 7)
				{
					if (last_texture == TexSmokeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 5 ? 194 : 195]);
				}
				else if (i < 9)
				{
					if (last_texture == TexSmokeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 7 ? 194 : 195]);
				}
				else
				{
					if (last_texture == TexSmokeGrenadeIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 9 ? 75 : i == 10 ? 76 : 77]);
				}
			}

			for (int i = 0; i < 11; i++)
			{
				if (i < 9)
				{
					if (last_texture == TexTMPIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[147 + i]);
				}
				else
				{
					if (last_texture == TexTMPIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[213]);
				}
			}

			for (int i = 0; i < 6; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexUMP45Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[156 + i]);
				}
				else
				{
					if (last_texture == TexUMP45Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[214]);
				}
			}

			for (int i = 0; i < 10; i++)
			{
				if (i < 4)
				{
					if (last_texture == TexUspIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[159 + i]);
				}
				else if (i < 7)
				{
					if (last_texture == TexUspIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[215]);
				}
				else
				{
					if (last_texture == TexUspIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[i == 7 ? 159 : i == 8 ? 160 : 162]);
				}
			}

			for (int i = 0; i < 9; i++)
			{
				if (i < 6)
				{
					if (last_texture == TexXM1014Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[163 + i]);
				}
				else
				{
					if (last_texture == TexXM1014Index[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[216]);
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (i < 3)
				{
					if (last_texture == TexShellIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[181 + i]);
				}
				else
				{
					if (last_texture == TexShellIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[181]);
				}
			}

			for (int i = 0; i < 1; i++)
			{
				if (last_texture == TexChickenIndex[i])
					glBindTexture(GL_TEXTURE_2D, texture_id[177]);
			}
		}

		if (cvar.esp_view_model_no_hands && g_Local.bAlive)
		{
			for (int i = 0; i < 114; i++)
			{
				if (last_texture == TexHandsIndex[i])
					return;
			}
		}
		else
		{
			if (cvar.esp_skins && g_Local.bAlive)
			{
				for (int i = 0; i < 38; i++)
				{
					if (last_texture == TexHandsIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[32]);
				}
				for (int i = 38; i < 76; i++)
				{
					if (last_texture == TexHandsIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[33]);
				}
				for (int i = 76; i < 114; i++)
				{
					if (last_texture == TexHandsIndex[i])
						glBindTexture(GL_TEXTURE_2D, texture_id[34]);
				}
			}
		}
	}
	pglBegin(mode);
}

void APIENTRY hooked_glClear(GLbitfield mask)
{
	if (cvar.esp_chase_cam && g_pEngine->GetMaxClients() && g_Local.bAlive && GetTickCount() - HudRedraw <= 100)
	{
		if (mask == GL_DEPTH_BUFFER_BIT)
		{
			(*pglClear)(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}
	pglClear(mask);
}

void APIENTRY hooked_glEnable(GLenum mode)
{
	pglEnable(mode);
}

void APIENTRY hooked_glDisable(GLenum mode)
{
	pglDisable(mode);
}

void APIENTRY hooked_glEnd(void)
{
	pglEnd();
}

void APIENTRY hooked_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();

	bool Player = ent && ent->player && ent->index != g_Local.iIndex;
	bool ThirdPerson = ent && ent->player && ent->index == g_Local.iIndex;
	bool Chicken = ent && ent->model && strstr(ent->model->name, "chick");
	bool CheckPlayer = (cvar.skeleton_models || (cvar.skeleton_player_stick_figure_only && cvar.skeleton_player_stick_figure)) && Player && g_Player[ent->index].bAlive && (g_Player[ent->index].iTeam != g_Local.iTeam || cvar.esp_visual_team);
	bool CheckThirdPerson = (cvar.skeleton_models_third_person || (cvar.skeleton_third_person_stick_figure_only && cvar.skeleton_third_person_stick_figure)) && ThirdPerson && g_Local.bAlive;
	bool CheckChicken = cvar.skeleton_models_chicken && Chicken;

	if ((CheckPlayer || CheckThirdPerson || CheckChicken) && CheckDrawEngine())
		return;
	pglVertex3f(x, y, z);
}

void APIENTRY hooked_glVertex2f(GLfloat x, GLfloat y)
{
	pglVertex2f(x, y);
}

void APIENTRY hooked_glVertex3fv(const GLfloat* v)
{
	pglVertex3fv(v);
}

void APIENTRY hooked_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	static bool FirstFrame = true;
	if (FirstFrame)
	{
		g_Engine.pfnClientCmd("toggleconsole");

		g_Engine.Con_Printf("\n\n\t\t\t\tHello, %s ;)\n", g_Engine.pfnGetCvarString("name"));
		g_Engine.Con_Printf("\t\t\t\tYou are injected!\n\n");

		g_Engine.Con_Printf("\t\t\t\tMultihack by:\n");
		g_Engine.Con_Printf("\t\t\t\t            [..         [..    [....     [..      \n");
		g_Engine.Con_Printf("\t\t\t\t             [..       [..   [..    [..  [..      \n");
		g_Engine.Con_Printf("\t\t\t\t     [..      [..     [..  [..        [..[..      \n");
		g_Engine.Con_Printf("\t\t\t\t   [.   [..    [..   [..   [..        [..[..      \n");
		g_Engine.Con_Printf("\t\t\t\t  [..... [..    [.. [..    [..        [..[..      \n");
		g_Engine.Con_Printf("\t\t\t\t  [.             [....       [..     [.. [..      \n");
		g_Engine.Con_Printf("\t\t\t\t    [....         [..          [....     [........\n\n");

		g_Engine.Con_Printf("\t\t\t\tSpecial thanks to my friend BloodSharp and oxiKKK <3\n\n");

		if (cvar.gui_key == -1)
			g_Engine.Con_Printf("\t\t\t\tMenu key is [No key]!\n");
		else if (cvar.gui_key == -2)
			g_Engine.Con_Printf("\t\t\t\tMenu key is [Press key]!\n");
		else
			g_Engine.Con_Printf("\t\t\t\tMenu key is [%s]!\n", VirtualKeyCodeToString((int)cvar.gui_key).c_str());

		FirstFrame = false;
	}
	static float ChangeKey = cvar.gui_key;
	if (ChangeKey != cvar.gui_key)
	{
		if (cvar.gui_key == -1)
			g_Engine.Con_Printf("\t\t\t\tMenu key is [No key]!\n");
		else if (cvar.gui_key == -2)
			g_Engine.Con_Printf("\t\t\t\tMenu key is [Press key]!\n");
		else
			g_Engine.Con_Printf("\t\t\t\tMenu key is [%s]!\n", VirtualKeyCodeToString((int)cvar.gui_key).c_str());

		ChangeKey = cvar.gui_key;
	}
	pglViewport(x, y, width, height);
}

BOOL APIENTRY hooked_wglSwapBuffers(HDC hdc)
{
	InitImGui(hdc);
	return(*pwglSwapBuffers)(hdc);
}

void APIENTRY hooked_glPopMatrix(void)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	PopMatrix(ent);
	pglPopMatrix();
}

void APIENTRY hooked_glPushMatrix(void)
{
	pglPushMatrix();
}

void APIENTRY hooked_glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	pglFrustum(left, right, bottom, top, zNear, zFar);
}

void APIENTRY hooked_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	pglBlendFunc(sfactor, dfactor);
}

void APIENTRY hooked_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
	pglReadPixels(x, y, width, height, format, type, pixels);
}

void CodeWalk(DWORD dwStartAddress, DWORD dwEndAddress)
{
	for (DWORD dwCurrentAddress = dwStartAddress; dwCurrentAddress <= dwEndAddress - 0x6; dwCurrentAddress += 0x6)
	{
		PDWORD pdwTempAddress = (PDWORD)dwCurrentAddress + 0x2;

		PDWORD pdwTableAddress = (PDWORD)* pdwTempAddress;

		HMODULE hmOpenGL = LoadLibrary("opengl32.dll");

		if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glBegin"))
		{
			pglBegin = (glBegin_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glBegin;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glPopMatrix"))
		{
			pglPopMatrix = (glPopMatrix_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glPopMatrix;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glPushMatrix"))
		{
			pglPushMatrix = (glPushMatrix_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glPushMatrix;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glShadeModel"))
		{
			pglShadeModel = (glShadeModel_t)* pdwTableAddress;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glClear"))
		{
			pglClear = (glClear_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glClear;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glEnable"))
		{
			pglEnable = (glEnable_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glEnable;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glEnd"))
		{
			pglEnd = (glEnd_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glEnd;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glDisable"))
		{
			pglDisable = (glDisable_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glDisable;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glVertex2f"))
		{
			pglVertex2f = (glVertex2f_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glVertex2f;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glVertex3f"))
		{
			pglVertex3f = (glVertex3f_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glVertex3f;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glVertex3fv"))
		{
			pglVertex3fv = (glVertex3fv_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glVertex3fv;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glViewport"))
		{
			pglViewport = (glViewport_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glViewport;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "wglSwapBuffers"))
		{
			pwglSwapBuffers = (wglSwapBuffers_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_wglSwapBuffers;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glFrustum"))
		{
			pglFrustum = (glFrustum_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glFrustum;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glBlendFunc"))
		{
			pglBlendFunc = (glBlendFunc_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glBlendFunc;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glColor4f"))
		{
			pglColor4f = (glColor4f_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glColor4f;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glVertex3f"))
		{
			pglVertex3f = (glVertex3f_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glVertex3f;
		}
		else if (*pdwTableAddress == (DWORD)GetProcAddress(hmOpenGL, "glReadPixels"))
		{
			pglReadPixels = (glReadPixels_t)* pdwTableAddress;
			*pdwTableAddress = (DWORD)& hooked_glReadPixels;
		}
	}
}

void OpenGL(void)
{
	DWORD dwImportCode = pMemoryTools->dwFindPattern(0x01D7AC00, 0x000FF000, (BYTE*)"\xA1\xFF\xFF\xFF\xFF\x56\x33\xF6\x3B\xC6\x74\x07\x50", "x????xxxxxxxx");

	if (dwImportCode == NULL) return;
	else
	{
		dwImportCode += 0x13;
		CodeWalk(dwImportCode, dwImportCode + 0x870);
	}
}

void InitOpenGL()
{
	if (GetModuleHandle("opengl32.dll"))
		OpenGL();
}