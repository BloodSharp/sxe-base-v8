#include "client.h"

HWND hGameWnd;
WNDPROC hGameWndProc;
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool bShowMenu = false;
bool show_app_console = false;
bool show_menu_key = false;
bool loadtexture = true;
bool loadtexturemodel = true;
bool keys[256]; 
bool bOldOpenGL = true;
bool show_demo = false;
bool channelchange = false;

int MenuTab;
int rotation_start_index;

GLuint texture_id[1024];
GLint iMajor, iMinor;

DWORD ModeChangeDelay = 0;
DWORD stream;

bool CalcScreen(float* pflOrigin, float* pflVecScreen)
{
	int iResult = WorldToScreen(pflOrigin, pflVecScreen);
	if (pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult)
	{
		pflVecScreen[0] = pflVecScreen[0] * (ImGui::GetWindowSize().x / 2) + (ImGui::GetWindowSize().x / 2);
		pflVecScreen[1] = -pflVecScreen[1] * (ImGui::GetWindowSize().y / 2) + (ImGui::GetWindowSize().y / 2);
		return true;
	}
	return false;
}

void ImRotateStart()
{
	rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
}

ImVec2 operator-(const ImVec2& l, const ImVec2& r)
{
	return{ l.x - r.x, l.y - r.y };
}

void ImRotateEnd(float rad, ImVec2 center)
{
	float s = sin(rad), c = cos(rad);
	center = ImRotate(center, s, c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

void LoadTextureMemory(const unsigned char* image, int index, int size)
{
	int width, height;

	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &texture_id[index]);
	glBindTexture(GL_TEXTURE_2D, texture_id[index]);
	unsigned char* soilimage = SOIL_load_image_from_memory(image, size ,&width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soilimage);
	SOIL_free_image_data(soilimage);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

void LoadTextureImage(const char* image, int index)
{
	char filename[256];
	int width, height;

	sprintf(filename, "%s%s", hackdir, image);
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &texture_id[index]);
	glBindTexture(GL_TEXTURE_2D, texture_id[index]);
	unsigned char* soilimage = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, soilimage);
	SOIL_free_image_data(soilimage);
	glBindTexture(GL_TEXTURE_2D, last_texture);
}

void GetTexture()
{
	if (loadtexture)
	{
		LoadTextureMemory(menuback, MENUBACK, 924980);
		LoadTextureMemory(target, TARGET, 6080);
		LoadTextureMemory(rage, RAGE, 5494);
		LoadTextureMemory(legit, LEGIT, 3447);
		LoadTextureMemory(knife, KNIFE, 2079);
		LoadTextureMemory(settings, SETTINGS, 5946);
		LoadTextureMemory(model, MODEL, 2407);
		LoadTextureMemory(visual, VISUAL, 2722);
		LoadTextureMemory(bunny, BUNNY, 4749);
		LoadTextureMemory(color, COLOR, 5411);
		LoadTextureMemory(face, FACE, 46742);
		LoadTextureMemory(backdrop, BACKDROP, 3805);

		loadtexture = false;
	}
}

void GetTextureModel()
{
	if (loadtexturemodel)
	{
		LoadTextureImage("texture/weapon/knife/knifeskin.bmp", 31);
		LoadTextureImage("texture/weapon/hands/view_glove.bmp", 32);
		LoadTextureImage("texture/weapon/hands/view_finger.bmp", 33);
		LoadTextureImage("texture/weapon/hands/view_skin.bmp", 34);
		LoadTextureImage("texture/weapon/ak47/barrel.bmp", 35);
		LoadTextureImage("texture/weapon/ak47/forearm.bmp", 36);
		LoadTextureImage("texture/weapon/ak47/handle.bmp", 37);
		LoadTextureImage("texture/weapon/ak47/lower_body.bmp", 38);
		LoadTextureImage("texture/weapon/ak47/magazine.bmp", 39);
		LoadTextureImage("texture/weapon/ak47/reticle.bmp", 40);
		LoadTextureImage("texture/weapon/ak47/upper_body.bmp", 41);
		LoadTextureImage("texture/weapon/ak47/wood.bmp", 42);
		LoadTextureImage("texture/weapon/awp/barrel.bmp", 43);
		LoadTextureImage("texture/weapon/awp/base.bmp", 44);
		LoadTextureImage("texture/weapon/awp/bolt_handle.bmp", 45);
		LoadTextureImage("texture/weapon/awp/buttstock.bmp", 46);
		LoadTextureImage("texture/weapon/awp/ejector_port.bmp", 47);
		LoadTextureImage("texture/weapon/awp/magazine.bmp", 48);
		LoadTextureImage("texture/weapon/awp/newparts.bmp", 49);
		LoadTextureImage("texture/weapon/awp/newparts2.bmp", 50);
		LoadTextureImage("texture/weapon/awp/scope.bmp", 51);
		LoadTextureImage("texture/weapon/awp/scope_clamps.bmp", 52);
		LoadTextureImage("texture/weapon/m4a1/barrel.bmp", 53);
		LoadTextureImage("texture/weapon/m4a1/buttstock.bmp", 54);
		LoadTextureImage("texture/weapon/m4a1/handle.bmp", 55);
		LoadTextureImage("texture/weapon/m4a1/magazine.bmp", 56);
		LoadTextureImage("texture/weapon/m4a1/receiver.bmp", 57);
		LoadTextureImage("texture/weapon/m4a1/silencer.bmp", 58);
		LoadTextureImage("texture/weapon/deagle/DE_handle.bmp", 59);
		LoadTextureImage("texture/weapon/deagle/DE_slide1.bmp", 60);
		LoadTextureImage("texture/weapon/deagle/DE_slide2_eagle.bmp", 61);
		LoadTextureImage("texture/weapon/scout/base.bmp", 62);
		LoadTextureImage("texture/weapon/scout/magazine.bmp", 63);
		LoadTextureImage("texture/weapon/scout/rail.bmp", 64);
		LoadTextureImage("texture/weapon/scout/scope.bmp", 65);
		LoadTextureImage("texture/weapon/scout/scope_clamps.bmp", 66);
		LoadTextureImage("texture/weapon/aug/barrel.bmp", 67);
		LoadTextureImage("texture/weapon/aug/body.bmp", 68);
		LoadTextureImage("texture/weapon/aug/magazine.bmp", 69);
		LoadTextureImage("texture/weapon/c4/c4base.bmp", 70);
		LoadTextureImage("texture/weapon/c4/c4buttons.bmp", 71);
		LoadTextureImage("texture/weapon/c4/c4timer.bmp", 72);
		LoadTextureImage("texture/weapon/c4/c4wires.bmp", 73);
		LoadTextureImage("texture/weapon/c4/cbase_front.bmp", 74);
		LoadTextureImage("texture/weapon/smokegrenade/smoke_body.bmp", 75);
		LoadTextureImage("texture/weapon/smokegrenade/smoke_spoon.bmp", 76);
		LoadTextureImage("texture/weapon/smokegrenade/smoke_top.bmp", 77);
		LoadTextureImage("texture/weapon/hegrenade/he_body.bmp", 78);
		LoadTextureImage("texture/weapon/hegrenade/he_spoon.bmp", 79);
		LoadTextureImage("texture/weapon/hegrenade/he_top.bmp", 80);
		LoadTextureImage("texture/weapon/flashbang/flash_body.bmp", 81);
		LoadTextureImage("texture/weapon/flashbang/flash_spoon.bmp", 82);
		LoadTextureImage("texture/weapon/flashbang/flash_top.bmp", 83);
		LoadTextureImage("texture/weapon/shield/shield.bmp", 84);
		LoadTextureImage("texture/weapon/elite/barrel.bmp", 85);
		LoadTextureImage("texture/weapon/elite/handle.bmp", 86);
		LoadTextureImage("texture/weapon/elite/magazine.bmp", 87);
		LoadTextureImage("texture/weapon/elite/slide.bmp", 88);
		LoadTextureImage("texture/weapon/famas/v_famas.bmp", 89);
		LoadTextureImage("texture/weapon/fiveseven/fs1.bmp", 90);
		LoadTextureImage("texture/weapon/fiveseven/fs2.bmp", 91);
		LoadTextureImage("texture/weapon/g3sg1/barrel.bmp", 92);
		LoadTextureImage("texture/weapon/g3sg1/body.bmp", 93);
		LoadTextureImage("texture/weapon/g3sg1/buttstock.bmp", 94);
		LoadTextureImage("texture/weapon/g3sg1/forearm.bmp", 95);
		LoadTextureImage("texture/weapon/g3sg1/lowerbody.bmp", 96);
		LoadTextureImage("texture/weapon/g3sg1/scope.bmp", 97);
		LoadTextureImage("texture/weapon/g3sg1/scope_hold.bmp", 98);
		LoadTextureImage("texture/weapon/g3sg1/scope_knob.bmp", 99);
		LoadTextureImage("texture/weapon/galil/galil.bmp", 100);
		LoadTextureImage("texture/weapon/glock18/glock_barrel.bmp", 101);
		LoadTextureImage("texture/weapon/glock18/glock_base.bmp", 102);
		LoadTextureImage("texture/weapon/glock18/glock_mag.bmp", 103);
		LoadTextureImage("texture/weapon/glock18/glock_slide.bmp", 104);
		LoadTextureImage("texture/weapon/m3/barrel.bmp", 105);
		LoadTextureImage("texture/weapon/m3/forearm.bmp", 106);
		LoadTextureImage("texture/weapon/m3/handle.bmp", 107);
		LoadTextureImage("texture/weapon/m3/sights.bmp", 108);
		LoadTextureImage("texture/weapon/m249/ammobox.bmp", 109);
		LoadTextureImage("texture/weapon/m249/barrel.bmp", 110);
		LoadTextureImage("texture/weapon/m249/body.bmp", 111);
		LoadTextureImage("texture/weapon/m249/bullet.bmp", 112);
		LoadTextureImage("texture/weapon/m249/buttstock.bmp", 113);
		LoadTextureImage("texture/weapon/m249/cover.bmp", 114);
		LoadTextureImage("texture/weapon/m249/forearm.bmp", 115);
		LoadTextureImage("texture/weapon/m249/handle.bmp", 116);
		LoadTextureImage("texture/weapon/m249/sight.bmp", 117);
		LoadTextureImage("texture/weapon/mac10/body.bmp", 118);
		LoadTextureImage("texture/weapon/mac10/buttstock.bmp", 119);
		LoadTextureImage("texture/weapon/mac10/handle.bmp", 120);
		LoadTextureImage("texture/weapon/mp5/barrel.bmp", 121);
		LoadTextureImage("texture/weapon/mp5/buttstock.bmp", 122);
		LoadTextureImage("texture/weapon/mp5/clip.bmp", 123);
		LoadTextureImage("texture/weapon/mp5/forearm.bmp", 124);
		LoadTextureImage("texture/weapon/mp5/handle.bmp", 125);
		LoadTextureImage("texture/weapon/mp5/lowerrec.bmp", 126);
		LoadTextureImage("texture/weapon/mp5/rearsight.bmp", 127);
		LoadTextureImage("texture/weapon/p90/buttstock.bmp", 128);
		LoadTextureImage("texture/weapon/p90/handle-1.bmp", 129);
		LoadTextureImage("texture/weapon/p90/handles.bmp", 130);
		LoadTextureImage("texture/weapon/p90/magazine.bmp", 131);
		LoadTextureImage("texture/weapon/p90/sights-1.bmp", 132);
		LoadTextureImage("texture/weapon/p228/p228_handle.bmp", 133);
		LoadTextureImage("texture/weapon/p228/p228_mag.bmp", 134);
		LoadTextureImage("texture/weapon/p228/p228_slide.bmp", 135);
		LoadTextureImage("texture/weapon/sg550/buttstock.bmp", 136);
		LoadTextureImage("texture/weapon/sg550/forearm.bmp", 137);
		LoadTextureImage("texture/weapon/sg550/handle.bmp", 138);
		LoadTextureImage("texture/weapon/sg550/magazine_transp.bmp", 139);
		LoadTextureImage("texture/weapon/sg550/receiver.bmp", 140);
		LoadTextureImage("texture/weapon/sg550/scope.bmp", 141);
		LoadTextureImage("texture/weapon/sg552/acog.bmp", 142);
		LoadTextureImage("texture/weapon/sg552/buttstock.bmp", 143);
		LoadTextureImage("texture/weapon/sg552/forearm.bmp", 144);
		LoadTextureImage("texture/weapon/sg552/handle.bmp", 145);
		LoadTextureImage("texture/weapon/sg552/magazine_transp.bmp", 146);
		LoadTextureImage("texture/weapon/tmp/barrel.bmp", 147);
		LoadTextureImage("texture/weapon/tmp/body.bmp", 148);
		LoadTextureImage("texture/weapon/tmp/ejector.bmp", 149);
		LoadTextureImage("texture/weapon/tmp/forearm.bmp", 150);
		LoadTextureImage("texture/weapon/tmp/handle.bmp", 151);
		LoadTextureImage("texture/weapon/tmp/magazine.bmp", 152);
		LoadTextureImage("texture/weapon/tmp/misc.bmp", 153);
		LoadTextureImage("texture/weapon/tmp/silencer.bmp", 154);
		LoadTextureImage("texture/weapon/tmp/top.bmp", 155);
		LoadTextureImage("texture/weapon/ump45/buttstock.bmp", 156);
		LoadTextureImage("texture/weapon/ump45/handle.bmp", 157);
		LoadTextureImage("texture/weapon/ump45/receiver.bmp", 158);
		LoadTextureImage("texture/weapon/usp/handle.bmp", 159);
		LoadTextureImage("texture/weapon/usp/magazine.bmp", 160);
		LoadTextureImage("texture/weapon/usp/silencer.bmp", 161);
		LoadTextureImage("texture/weapon/usp/slide.bmp", 162);
		LoadTextureImage("texture/weapon/xm1014/barrel.bmp", 163);
		LoadTextureImage("texture/weapon/xm1014/body.bmp", 164);
		LoadTextureImage("texture/weapon/xm1014/buttstock.bmp", 165);
		LoadTextureImage("texture/weapon/xm1014/forearm.bmp", 166);
		LoadTextureImage("texture/weapon/xm1014/rear_sight.bmp", 167);
		LoadTextureImage("texture/weapon/xm1014/shell.bmp", 168);
		LoadTextureImage("texture/player/arctic/ARTIC_Working1.bmp", 169);
		LoadTextureImage("texture/player/gign/GIGN_DMBASE2.bmp", 170);
		LoadTextureImage("texture/player/gsg9/GSG9_Working1.bmp", 171);
		LoadTextureImage("texture/player/guerilla/GUERILLA_DMBASE.bmp", 172);
		LoadTextureImage("texture/player/leet/Arab_dmbase1.bmp", 173);
		LoadTextureImage("texture/player/sas/SAS_DMBASE1.bmp", 174);
		LoadTextureImage("texture/player/terror/Terrorist_Working1.bmp", 175);
		LoadTextureImage("texture/player/urban/SEAL_Working1.bmp", 176);
		LoadTextureImage("texture/chicken/skin.bmp", 177);
		LoadTextureImage("texture/player/backpack/Backpack1.bmp", 178);
		LoadTextureImage("texture/player/backpack/Backpack2.bmp", 179);
		LoadTextureImage("texture/weapon/ak47/ak47_skin.bmp", 180);
		LoadTextureImage("texture/weapon/shell/rifle_goldshell.bmp", 181);
		LoadTextureImage("texture/weapon/shell/pistol_goldshell.bmp", 182);
		LoadTextureImage("texture/weapon/shell/shell.bmp", 183);
		LoadTextureImage("texture/weapon/aug/w_aug.bmp", 184);
		LoadTextureImage("texture/weapon/awp/w_awp.bmp", 185);
		LoadTextureImage("texture/weapon/deagle/deserteagle_skin.bmp", 186);
		LoadTextureImage("texture/weapon/elite/w_elite.bmp", 187);
		LoadTextureImage("texture/weapon/famas/p_famas.bmp", 188);
		LoadTextureImage("texture/weapon/fiveseven/57_profile.bmp", 189);
		LoadTextureImage("texture/weapon/flashbang/f_body.bmp", 190);
		LoadTextureImage("texture/weapon/flashbang/f_top.bmp", 191);
		LoadTextureImage("texture/weapon/hegrenade/f_body.bmp", 192);
		LoadTextureImage("texture/weapon/hegrenade/f_top.bmp", 193);
		LoadTextureImage("texture/weapon/smokegrenade/f_body.bmp", 194);
		LoadTextureImage("texture/weapon/smokegrenade/f_top.bmp", 195);
		LoadTextureImage("texture/weapon/g3sg1/w_g3sg1.bmp", 196);
		LoadTextureImage("texture/weapon/galil/p_galil.bmp", 197);
		LoadTextureImage("texture/weapon/glock18/w_glock18.bmp", 198);
		LoadTextureImage("texture/weapon/knife/pknifeskin.bmp", 199);
		LoadTextureImage("texture/weapon/m3/w_m3super90.bmp", 200);
		LoadTextureImage("texture/weapon/m4a1/m4a1_skin.bmp", 201);
		LoadTextureImage("texture/weapon/m249/w_m249.bmp", 202);
		LoadTextureImage("texture/weapon/mac10/w_mac10.bmp", 203);
		LoadTextureImage("texture/weapon/mp5/w_mp5.bmp", 204);
		LoadTextureImage("texture/weapon/p90/w_p90.bmp", 205);
		LoadTextureImage("texture/weapon/p228/w_p228.bmp", 206);
		LoadTextureImage("texture/weapon/scout/w_scout.bmp", 207);
		LoadTextureImage("texture/weapon/sg550/sg550_profile.bmp", 208);
		LoadTextureImage("texture/weapon/sg552/sg552_skin.bmp", 209);
		LoadTextureImage("texture/weapon/shield/shield_back.bmp", 210);
		LoadTextureImage("texture/weapon/shield/shield_front.bmp", 211);
		LoadTextureImage("texture/weapon/shield/shield_glass.bmp", 212);
		LoadTextureImage("texture/weapon/tmp/w_tmp.bmp", 213);
		LoadTextureImage("texture/weapon/ump45/ump_profile.bmp", 214);
		LoadTextureImage("texture/weapon/usp/usp_skin.bmp", 215);
		LoadTextureImage("texture/weapon/xm1014/xm1014.bmp", 216);
		LoadTextureImage("texture/weapon/assault/kevlar_vest.bmp", 217);

		loadtexturemodel = false;
	}
}

void KeyBinds()
{
	if (!bShowMenu)
	{
		static bool strafekey = keys[(int)cvar.kz_strafe_key];
		if (strafekey != keys[(int)cvar.kz_strafe_key])
		{
			if (cvar.kz_strafe)
			{
				if (keys[(int)cvar.kz_strafe_key])
					Strafe = 1;
				else
					Strafe = 0;
			}
			strafekey = keys[(int)cvar.kz_strafe_key];
		}
		static bool fastrunkey = keys[(int)cvar.kz_fastrun_key];
		if (fastrunkey != keys[(int)cvar.kz_fastrun_key])
		{
			if (cvar.kz_fast_run)
			{
				if (keys[(int)cvar.kz_fastrun_key])
					Fastrun = 1;
				else
					Fastrun = 0;
			}
			fastrunkey = keys[(int)cvar.kz_fastrun_key];
		}
		static bool gstrafekey = keys[(int)cvar.kz_gstrafe_key];
		if (gstrafekey != keys[(int)cvar.kz_gstrafe_key])
		{
			if (cvar.kz_gstrafe)
			{
				if (keys[(int)cvar.kz_gstrafe_key])
					Gstrafe = 1;
				else
					Gstrafe = 0;
			}
			gstrafekey = keys[(int)cvar.kz_gstrafe_key];
		}
		static bool bhopkey = keys[(int)cvar.kz_bhop_key];
		if (bhopkey != keys[(int)cvar.kz_bhop_key])
		{
			if (cvar.kz_bhop)
			{
				if (keys[(int)cvar.kz_bhop_key])
					Bhop = 1;
				else
					Bhop = 0;
			}
			bhopkey = keys[(int)cvar.kz_bhop_key];
		}
		static bool jumpbugkey = keys[(int)cvar.kz_jumpbug_key];
		if (jumpbugkey != keys[(int)cvar.kz_jumpbug_key])
		{
			if (cvar.kz_jump_bug)
			{
				if (keys[(int)cvar.kz_jumpbug_key])
					Jumpbug = 1;
				else
					Jumpbug = 0;
			}
			jumpbugkey = keys[(int)cvar.kz_jumpbug_key];
		}
		static bool quickchangekey = keys[(int)cvar.misc_quick_change_key];
		if (quickchangekey != keys[(int)cvar.misc_quick_change_key])
		{
			if (cvar.misc_quick_change && keys[(int)cvar.misc_quick_change_key])
			{
				cvar.rage_active = !cvar.rage_active;
				SaveCvar();
				ModeChangeDelay = GetTickCount();
			}
			quickchangekey = keys[(int)cvar.misc_quick_change_key];
		}
	}
}

void DrawModeChange()
{
	float WindowBorderSize = ImGui::GetStyle().WindowBorderSize;
	ImGui::GetStyle().WindowBorderSize = 0.0f;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::Begin("DrawModeChange", reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	{
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiSetCond_Always);

		if (CheckDraw() && g_pEngine->GetMaxClients())
		{
			int y = (ImGui::GetWindowSize().y / 2) + ImGui::GetWindowSize().y / 20 * 5;
			if (GetTickCount() - ModeChangeDelay < 5000)
			{
				ImVec2 label_size = ImGui::CalcTextSize(cvar.rage_active ? "Rage Mode":"Legit Mode", NULL, true);
				ImGui::GetCurrentWindow()->DrawList->AddRect({ 20 - 2, (float)y + 37 }, { label_size.x + 24, (float)y + 51 }, ImColor(colorList.get(5)->onebased_r, colorList.get(5)->onebased_g, colorList.get(5)->onebased_b, cvar.esp_alpha), cvar.esp_rounding);
				ImGui::GetCurrentWindow()->DrawList->AddRect({ 20 - 3, (float)y + 36 }, { label_size.x + 23, (float)y + 50 }, ImColor(cvar.color_red, cvar.color_green, cvar.color_blue, cvar.esp_alpha), cvar.esp_rounding);
				ImGui::GetCurrentWindow()->DrawList->AddText({ 20, (float)y + 35 }, ImColor(cvar.color_red, cvar.color_green, cvar.color_blue, cvar.esp_alpha), cvar.rage_active ? "Rage Mode" : "Legit Mode");
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::GetStyle().WindowBorderSize = WindowBorderSize;
	}
}

void ColorChange()
{
	if (cvar.color_random)
	{
		static float Color[3];
		static DWORD Tickcount = 0;
		static DWORD Tickcheck = 0;
		ImGui::ColorConvertRGBtoHSV(cvar.color_red, cvar.color_green, cvar.color_blue, Color[0], Color[1], Color[2]);
		if (GetTickCount() - Tickcount >= 1)
		{
			if (Tickcheck != Tickcount)
			{
				Color[0] += 0.001f * cvar.color_speed;
				Tickcheck = Tickcount;
			}
			Tickcount = GetTickCount();
		}
		if (Color[0] < 0.0f) Color[0] += 1.0f;
		ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], cvar.color_red, cvar.color_green, cvar.color_blue);
	}
}

LRESULT CALLBACK HOOK_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
		keys[wParam] = true;

	if (uMsg == WM_KEYUP)
		keys[wParam] = false;

	if (uMsg == WM_KEYDOWN && wParam == cvar.gui_key)
	{
		SaveCvar();
		bShowMenu = !bShowMenu;
		return false;
	}
	if (bShowMenu)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			if ((uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP) && (wParam < 256))
				return CallWindowProc(hGameWndProc, hWnd, uMsg, wParam, lParam);
			return true;
		}
	}
	return CallWindowProc(hGameWndProc, hWnd, uMsg, wParam, lParam);
}

void SetCursor()
{
	static bool setcursor = bShowMenu;
	if (setcursor != bShowMenu)
	{
		if (!bShowMenu && g_pEngine->GetMaxClients())
			SetCursorPos(g_pEngine->GetWindowCenterX(), g_pEngine->GetWindowCenterY());
		setcursor = bShowMenu;
	}
}

void InistalizeImgui(HDC hdc)
{
	static bool bInitializeImGui = false;
	if (!bInitializeImGui)
	{
		hGameWnd = WindowFromDC(hdc);
		hGameWndProc = (WNDPROC)SetWindowLong(hGameWnd, GWL_WNDPROC, (LONG)HOOK_WndProc);
		glGetIntegerv(GL_MAJOR_VERSION, &iMajor);
		glGetIntegerv(GL_MINOR_VERSION, &iMinor);
		if ((iMajor * 10 + iMinor) >= 32)
			bOldOpenGL = false;
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hGameWnd);
		if (!bOldOpenGL)
		{
			ImplementGl3();
			ImGui_ImplOpenGL3_Init();
		}
		else
			ImGui_ImplOpenGL2_Init();
		ImGui::StyleColorsDark();
		ImGui::GetStyle().AntiAliasedFill = !bOldOpenGL ? true : false;
		ImGui::GetStyle().AntiAliasedLines = !bOldOpenGL ? true : false;
		ImGui::GetStyle().FrameRounding = 0.0f;
		ImGui::GetStyle().WindowRounding = 0.0f;
		ImGui::GetStyle().ChildRounding = 0.0f;
		ImGui::GetStyle().ScrollbarRounding = 0.0f;
		ImGui::GetStyle().GrabRounding = 0.0f;
		ImGui::GetStyle().FrameBorderSize = 1.0f;
		ImGui::GetStyle().WindowPadding = ImVec2(6, 8);
		ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
		ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);
		ImGui::GetStyle().WindowBorderSize = 0.0f;
		ImGui::GetStyle().FrameBorderSize = 0.0f;
		ImGui::GetStyle().PopupBorderSize = 0.0f;
		ImGui::GetIO().IniFilename = NULL;
		ImGui::GetIO().LogFilename = NULL;
		if (cvar.gui_key < 1 || cvar.gui_key > 254)
			cvar.gui_key = VK_INSERT;

		bInitializeImGui = true;
	}
}

void GLNewFrame()
{
	if (!bOldOpenGL)
		ImGui_ImplOpenGL3_NewFrame();
	else
		ImGui_ImplOpenGL2_NewFrame();
}

void RenderData()
{
	if (!bOldOpenGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	else
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void MouseEvent()
{
	if (bShowMenu)
		g_pClient->IN_DeactivateMouse();
	else
		g_pClient->IN_ActivateMouse();
}

void InitImGui(HDC hdc)
{
	//Credit to my kind friend BloodSharp for helping a noob <3
	SetCursor();
	InistalizeImgui(hdc);
	GLNewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	GetTexture();

	UpdateLocalPlayer();
	UpdatePlayer();

	ColorChange();

	DrawEntityHitbox();
	DrawHitMark();
	DrawLongJump();
	DrawSkeletonPlayer();
	DrawFacePlayer();
	DrawSoundEsp();
	DrawPlayerEsp();
	DrawAimbot();
	DrawTraceGrenade();
	DrawSkeletonThirdPerson();
	DrawFaceThirdPerson(); 
	DrawCrossHair();
	DrawWeaponName();
	DrawModeChange();
	DrawAimBotFOV();
	DrawKzWindow();
	DrawKzKeys();
	DrawChatWindow();
	DrawBackDrop();
	DrawIDHookWindow();
	DrawConsoleWindow();
	DrawMenuWindow();

	if (show_demo && bShowMenu) ImGui::ShowDemoWindow();

	ImGui::Render();
	RenderData();
	ImGui::GetIO().MouseDrawCursor = bShowMenu;
	MouseEvent();
}
