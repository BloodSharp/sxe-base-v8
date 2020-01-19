#include "client.h"

StudioModelRenderer_d pThis;

float chams_render_viewmodel_r;
float chams_render_viewmodel_g;
float chams_render_viewmodel_b;
float chams_render_viewmodel;

float chams_render_third_person_r;
float chams_render_third_person_g;
float chams_render_third_person_b;
float chams_render_third_person;

float chams_render_r;
float chams_render_g;
float chams_render_b;
float chams_render;

float chams_render_weapon_r;
float chams_render_weapon_g;
float chams_render_weapon_b;
float chams_render_weapon;

float chams_render_chicken_r;
float chams_render_chicken_g;
float chams_render_chicken_b;
float chams_render_chicken;

//=========================
// StudioDrawModel
//=========================
int StudioDrawModel (int flags)
{
	return oStudioDrawModel(flags);
}

//=========================
// StudioDrawPlayer
//=========================
int StudioDrawPlayer(int flags, struct entity_state_s *pplayer)
{
	return oStudioDrawPlayer(flags, pplayer);
}

//=========================
// StudioGetAnim
// Look up animation data for sequence
//=========================
mstudioanim_t StudioGetAnim( model_t *m_pSubModel, mstudioseqdesc_t *pseqdesc )
{
	return oStudioGetAnim(m_pSubModel, pseqdesc);
}

//=========================
// StudioSetUpTransform
// Interpolate model position and angles and set up matrices
//=========================
void StudioSetUpTransform (int trivial_accept)
{
	oStudioSetUpTransform(trivial_accept);
}

//=========================
// StudioSetupBones
// Set up model bone positions
//=========================
void StudioSetupBones ( void )
{
	oStudioSetupBones();
}

//=========================
// StudioCalcAttachments
// Find final attachment points
//=========================
void StudioCalcAttachments ( void )
{
	oStudioCalcAttachments();
}

//=========================
// StudioSaveBones
// Save bone matrices and names
//=========================
void StudioSaveBones( void )
{
	oStudioSaveBones();
}

//=========================
// StudioMergeBones
// Merge cached bones with current bones for model
//=========================
void StudioMergeBones( model_t *m_pSubModel )
{
	oStudioMergeBones(m_pSubModel);
}

//=========================
// StudioEstimateInterpolant
// Determine interpolation fraction
//=========================
float StudioEstimateInterpolant( void )
{
	return oStudioEstimateInterpolant();
}

//=========================
// StudioEstimateFrame
// Determine current frame for rendering
//=========================
float StudioEstimateFrame( mstudioseqdesc_t *pseqdesc )
{
	return oStudioEstimateFrame(pseqdesc);
}

//=========================
// StudioFxTransform
// Apply special effects to transform matrix
//=========================
void StudioFxTransform( cl_entity_t *pEnt, float transform[3][4] )
{
	oStudioFxTransform(pEnt, transform);
}

//=========================
// StudioSlerpBones
// Spherical interpolation of bones
//=========================
void StudioSlerpBones( vec4_t q1[], float pos1[][3], vec4_t q2[], float pos2[][3], float s )
{
	oStudioSlerpBones(q1, pos1, q2, pos2, s);
}

//=========================
// StudioCalcBoneAdj
// Compute bone adjustments ( bone controllers )
//=========================
void StudioCalcBoneAdj ( float dadt, float *adj, const byte *pcontroller1, const byte *pcontroller2, byte mouthopen )
{
	oStudioCalcBoneAdj(dadt, adj, pcontroller1, pcontroller2, mouthopen);
}

//=========================
// StudioCalcBoneQuaterion
// Get bone quaternions
//=========================
void StudioCalcBoneQuaterion( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *q )
{
	oStudioCalcBoneQuaterion(frame, s, pbone, panim, adj, q);
}

//=========================
// StudioCalcBonePosition
// Get bone positions
//=========================
void StudioCalcBonePosition( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *pos )
{
	oStudioCalcBonePosition(frame, s, pbone, panim, adj, pos);
}

//=========================
// StudioCalcRotations
// Compute rotations
//=========================
void StudioCalcRotations( float pos[][3], vec4_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f )
{
	oStudioCalcRotations(pos, q, pseqdesc, panim, f);
}

//=========================
// StudioRenderModel
// Send bones and verts to renderer
//=========================

void Glow(cl_entity_s* ent, bool valident, float glow, float chams, float r, float g, float b, int width)
{
	if (valident && glow && CheckDrawEngine())
	{
		glDepthFunc(GL_GREATER);
		glDisable(GL_DEPTH_TEST);
		g_Studio.SetForceFaceFlags(STUDIO_NF_CHROME);
		ent->curstate.renderfx = kRenderFxGlowShell;
		ent->curstate.renderamt = width;
		ent->curstate.rendermode = 0;

		ent->curstate.rendercolor.r = r * 255.0f;
		ent->curstate.rendercolor.g = g * 255.0f;
		ent->curstate.rendercolor.b = b * 255.0f;

		oStudioRenderFinal();
		glEnable(GL_DEPTH_TEST);

		if (!chams)
		{
			glDisable(GL_DEPTH_TEST);
			g_Studio.SetForceFaceFlags(0);
			ent->curstate.renderfx = 0;
			oStudioRenderFinal();
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			oStudioRenderFinal();
		}
	}
}

void Chams(cl_entity_s* ent, bool valident, float chams, float chamswall, float chams_r, float chams_g, float chams_b, float chamswall_r, float chamswall_g, float chamswall_b, int coloring)
{
	if (valident && chams && CheckDrawEngine())
	{
		if (coloring == 1)chams_render_chicken = true;
		if (coloring == 2)chams_render_weapon = true;
		if (coloring == 3)chams_render_viewmodel = true;
		if (coloring == 4)chams_render_third_person = true;
		if (coloring == 5)chams_render = true;

		ent->curstate.rendermode = 0;
		ent->curstate.renderfx = 0;
		ent->curstate.renderamt = 0;
		g_Studio.SetForceFaceFlags(0);

		if (chams > 1) glDisable(GL_TEXTURE_2D);
		if (chams > 1) glBindTexture(GL_TEXTURE_2D, 0);

		ent->curstate.rendermode = 0;
		ent->curstate.renderfx = 0;
		ent->curstate.renderamt = 0;
		g_Studio.SetForceFaceFlags(0);

		if (chamswall)
		{
			glDepthFunc(GL_GREATER);
			glDisable(GL_DEPTH_TEST);
			if (coloring == 1)
			{
				chams_render_chicken_r = chamswall_r;
				chams_render_chicken_g = chamswall_g;
				chams_render_chicken_b = chamswall_b;
			}
			if (coloring == 2)
			{
				chams_render_weapon_r = chamswall_r;
				chams_render_weapon_g = chamswall_g;
				chams_render_weapon_b = chamswall_b;
			}
			if (coloring == 3)
			{
				chams_render_viewmodel_r = chamswall_r;
				chams_render_viewmodel_g = chamswall_g;
				chams_render_viewmodel_b = chamswall_b;
			}
			if (coloring == 4)
			{
				chams_render_third_person_r = chamswall_r;
				chams_render_third_person_g = chamswall_g;
				chams_render_third_person_b = chamswall_b;
			}
			if (coloring == 5)
			{
				chams_render_r = chamswall_r;
				chams_render_g = chamswall_g;
				chams_render_b = chamswall_b;
			}
			oStudioRenderFinal();
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		if (coloring == 1)
		{
			chams_render_chicken_r = chams_r;
			chams_render_chicken_g = chams_g;
			chams_render_chicken_b = chams_b;
		}
		if (coloring == 2)
		{
			chams_render_weapon_r = chams_r;
			chams_render_weapon_g = chams_g;
			chams_render_weapon_b = chams_b;
		}
		if (coloring == 3)
		{
			chams_render_viewmodel_r = chams_r;
			chams_render_viewmodel_g = chams_g;
			chams_render_viewmodel_b = chams_b;
		}
		if (coloring == 4)
		{
			chams_render_third_person_r = chams_r;
			chams_render_third_person_g = chams_g;
			chams_render_third_person_b = chams_b;
		}
		if (coloring == 5)
		{
			chams_render_r = chams_r;
			chams_render_g = chams_g;
			chams_render_b = chams_b;
		}
		oStudioRenderFinal();

		if (chams > 1) glEnable(GL_TEXTURE_2D);
	}
	if (coloring == 1)chams_render_chicken = false;
	if (coloring == 2)chams_render_weapon = false;
	if (coloring == 3)chams_render_viewmodel = false;
	if (coloring == 4)chams_render_third_person = false;
	if (coloring == 5)chams_render = false;
}

void StudioRenderModel(void)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	bool Player = ent && ent->player && ent->index != g_Local.iIndex && g_Player[ent->index].bAlive && (g_Player[ent->index].iTeam != g_Local.iTeam || cvar.esp_visual_team);
	bool ThirdPerson = ent && ent->player && ent->index == g_Local.iIndex && g_Local.bAlive;
	bool ViewModel = ent && ent->model && ent->model->name && strstr(ent->model->name, "v_");
	bool Weapons = ent && ent->model && (strstr(ent->model->name, "w_") || strstr(ent->model->name, "p_")) && g_Local.bAlive;
	bool Chicken = ent && ent->model && strstr(ent->model->name, "chick");

	Glow(ent, Chicken, cvar.chams_chicken_glow, cvar.chams_chicken, cvar.color_green, cvar.color_blue, cvar.color_red, 12);
	Chams(ent, Chicken, cvar.chams_chicken, cvar.chams_chicken_wall, cvar.color_blue, cvar.color_red, cvar.color_green, cvar.color_red, cvar.color_green, cvar.color_blue, 1);
	Glow(ent, Weapons, cvar.chams_weapon_glow, cvar.chams_weapon, cvar.color_green, cvar.color_blue, cvar.color_red, 12);
	Chams(ent, Weapons, cvar.chams_weapon, cvar.chams_weapon_wall, cvar.color_blue, cvar.color_red, cvar.color_green, cvar.color_red, cvar.color_green, cvar.color_blue, 2);
	Glow(ent, ViewModel, cvar.chams_view_model_glow, cvar.chams_view_model, cvar.color_blue, cvar.color_red, cvar.color_green, cvar.esp_view_model_no_hands ? 0 : 1);
	Chams(ent, ViewModel, cvar.chams_view_model, 0, cvar.color_blue, cvar.color_red, cvar.color_green, 0, 0, 0, 3);
	Glow(ent, ThirdPerson, cvar.chams_third_person_glow && cvar.skeleton_models_third_person != 1, cvar.chams_third_person, cvar.color_green, cvar.color_blue, cvar.color_red, 12);
	Chams(ent, ThirdPerson, cvar.chams_third_person, cvar.chams_third_person_wall && cvar.skeleton_models_third_person != 1, cvar.color_blue, cvar.color_red, cvar.color_green, cvar.color_blue, cvar.color_red, cvar.color_green, 4);
	Glow(ent, Player, cvar.chams_player_glow && cvar.skeleton_models != 1, cvar.chams_player, cvar.color_green, cvar.color_blue, cvar.color_red, 12);
	if (ent)Chams(ent, Player, cvar.chams_player, cvar.chams_player_wall, PlayerColor(ent->index)->onebased_r, PlayerColor(ent->index)->onebased_g, PlayerColor(ent->index)->onebased_b, PlayerColorChamsWall(ent->index)->onebased_r, PlayerColorChamsWall(ent->index)->onebased_g, PlayerColorChamsWall(ent->index)->onebased_b, 5);

	oStudioRenderModel();
}

//=========================
// StudioRenderFinal
// Finalize rendering
//=========================
void StudioRenderFinal (void)
{
	oStudioRenderFinal();
}

//=========================
// StudioRenderFinal_Software
// Software renderer finishing function
//=========================
void StudioRenderFinal_Software ( void )
{
	oStudioRenderFinal_Software();
}

//=========================
// StudioRenderFinal_Hardware
// GL&D3D renderer finishing function
//=========================
void StudioRenderFinal_Hardware(void)
{
	cl_entity_s* ent = g_Studio.GetCurrentEntity();
	bool Player = ent && ent->player && ent->index != g_Local.iIndex;
	bool Checkplayer = Player && g_Player[ent->index].bAlive && (g_Player[ent->index].iTeam != g_Local.iTeam || cvar.esp_visual_team);
	bool Thirdperson = ent && ent->player && ent->index == g_Local.iIndex;
	bool Chicken = ent && ent->model && strstr(ent->model->name, "chick");

	g_pStudio->SetupRenderer(0);

	if (((Checkplayer && cvar.skeleton_models == 1) || (Thirdperson && g_Local.bAlive && cvar.skeleton_models_third_person == 1) || (Chicken && cvar.skeleton_models_chicken == 1)) && CheckDrawEngine())
		g_pStudio->StudioDrawBones();

	if (((Checkplayer && cvar.skeleton_models == 2) || (Thirdperson && g_Local.bAlive && cvar.skeleton_models_third_person == 2) || (Chicken && cvar.skeleton_models_chicken == 2)) && CheckDrawEngine())
		g_pStudio->StudioDrawHulls();

	g_pStudio->RestoreRenderer();

	oStudioRenderFinal_Hardware();
}

//=========================
// StudioPlayerBlend
// Determine pitch and blending amounts for players
//=========================
void StudioPlayerBlend ( mstudioseqdesc_t *pseqdesc, int *pBlend, float *pPitch )
{
	oStudioPlayerBlend(pseqdesc, pBlend, pPitch);
}

//=========================
// StudioEstimateGait
// Estimate gait frame for player
//=========================
void StudioEstimateGait ( entity_state_t *pplayer )
{
	oStudioEstimateGait(pplayer);
}

//=========================
// StudioProcessGait
// Process movement of player
//=========================
void StudioProcessGait ( entity_state_t *pplayer )
{
	oStudioProcessGait(pplayer);
}

//===================================================================================
// Callgates & Patchgates
//===================================================================================
void __declspec(naked) oStudioRenderModel(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderModel;
	}
}
void __declspec(naked)StudioRenderModel_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderModel
	}
}

int __declspec(naked) oStudioDrawModel(int flags)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioDrawModel;
	}
}
void __declspec(naked) StudioDrawModel_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioDrawModel
	}
}

int __declspec(naked) oStudioDrawPlayer(int flags, struct entity_state_s *pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioDrawPlayer;
	}
}
void __declspec(naked) StudioDrawPlayer_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioDrawPlayer
	}
}

mstudioanim_t __declspec(naked) oStudioGetAnim(model_t *m_pSubModel, mstudioseqdesc_t *pseqdesc)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioGetAnim;
	}
}
void __declspec(naked) StudioGetAnim_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioGetAnim
	}
}

void __declspec(naked) oStudioSetUpTransform(int trivial_accept)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSetUpTransform;
	}
}
void __declspec(naked) StudioSetUpTransform_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSetUpTransform
	}
}

void __declspec(naked) oStudioSetupBones(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSetupBones;
	}
}
void __declspec(naked) StudioSetupBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSetupBones
	}
}

void __declspec(naked) oStudioCalcAttachments(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcAttachments;
	}
}
void __declspec(naked) StudioCalcAttachments_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcAttachments
	}
}

void __declspec(naked) oStudioSaveBones(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSaveBones;
	}
}
void __declspec(naked) StudioSaveBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSaveBones
	}
}

void __declspec(naked) oStudioMergeBones(model_t *m_pSubModel)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioMergeBones;
	}
}
void __declspec(naked) StudioMergeBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioMergeBones
	}
}

float __declspec(naked) oStudioEstimateInterpolant(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateInterpolant;
	}
}
void __declspec(naked) StudioEstimateInterpolant_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateInterpolant
	}
}

float __declspec(naked) oStudioEstimateFrame(mstudioseqdesc_t *pseqdesc)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateFrame;
	}
}
void __declspec(naked) StudioEstimateFrame_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateFrame
	}
}

void __declspec(naked) oStudioFxTransform(cl_entity_t *pEnt, float transform[3][4])
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioFxTransform;
	}
}
void __declspec(naked) StudioFxTransforme_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioFxTransform
	}
}

void __declspec(naked) oStudioSlerpBones(vec4_t q1[], float pos1[][3], vec4_t q2[], float pos2[][3], float s)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioSlerpBones;
	}
}
void __declspec(naked) StudioSlerpBones_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioSlerpBones
	}
}

void __declspec(naked) oStudioCalcBoneAdj(float dadt, float *adj, const byte *pcontroller1, const byte *pcontroller2, byte mouthopen)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBoneAdj;
	}
}
void __declspec(naked) StudioCalcBoneAdj_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBoneAdj
	}
}

void __declspec(naked) oStudioCalcBoneQuaterion(int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *q)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBoneQuaterion;
	}
}
void __declspec(naked) StudioCalcBoneQuaterion_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBoneQuaterion
	}
}

void __declspec(naked) oStudioCalcBonePosition(int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *pos)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcBonePosition;
	}
}
void __declspec(naked) StudioCalcBonePosition_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcBonePosition
	}
}

void __declspec(naked) oStudioCalcRotations(float pos[][3], vec4_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioCalcRotations;
	}
}
void __declspec(naked) StudioCalcRotations_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioCalcRotations
	}
}

void __declspec(naked) oStudioRenderFinal(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal;
	}
}
void __declspec(naked) StudioRenderFinal_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal
	}
}

void __declspec(naked) oStudioRenderFinal_Software(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal_Software;
	}
}
void __declspec(naked) StudioRenderFinal_Software_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal_Software
	}
}

void __declspec(naked) oStudioRenderFinal_Hardware(void)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioRenderFinal_Hardware;
	}
}
void __declspec(naked) StudioRenderFinal_Hardware_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioRenderFinal_Hardware
	}
}

void __declspec(naked) oStudioPlayerBlend(mstudioseqdesc_t *pseqdesc, int *pBlend, float *pPitch)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioPlayerBlend;
	}
}
void __declspec(naked) StudioPlayerBlend_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioPlayerBlend
	}
}

void __declspec(naked) oStudioEstimateGait(entity_state_t *pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioEstimateGait;
	}
}
void __declspec(naked) StudioEstimateGait_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioEstimateGait
	}
}

void __declspec(naked) oStudioProcessGait(entity_state_t *pplayer)
{
	_asm
	{
		mov ecx, pThis
		jmp g_StudioModelRenderer.StudioProcessGait;
	}
}
void __declspec(naked) StudioProcessGait_Gate(void)
{
	_asm
	{
		mov pThis, ecx
		jmp StudioProcessGait
	}
}