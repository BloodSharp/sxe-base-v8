#include "client.h"

void Spacing()
{
	ImVec2 LastSapcing = ImGui::GetStyle().ItemSpacing;
	ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);
	ImGui::SameLine();
	ImGui::GetStyle().ItemSpacing = LastSapcing;
}

void Menutabs(int tab, int texture, bool sameline)
{
	static float RotateImage = 0.0f;
	static float RotateButton = 0.0f;
	static float RotateReset = 0.0f;
	static DWORD Tickcount = 0;
	static DWORD Tickcheck = 0;
	if (cvar.gui_menu_button_spin)
	{
		if (GetTickCount() - Tickcount >= 1)
		{
			if (Tickcheck != Tickcount)
			{
				RotateImage += 0.001f * cvar.gui_menu_button_spin;
				RotateButton += (0.001f * (M_PI * 2)) * cvar.gui_menu_button_spin;
				RotateReset += (0.001f * (M_PI * 2)) * cvar.gui_menu_button_spin;
				Tickcheck = Tickcount;
			}
			Tickcount = GetTickCount();
		}

		if (RotateReset / (M_PI * 2) > 1.0f || RotateReset / (M_PI * 2) < -1.0f)
		{
			RotateImage = 0.0f;
			RotateButton = M_PI * 2 / 4;
			RotateReset = 0.0f;
		}
	}
	else
	{
		RotateImage = 0.0f;
		RotateButton = M_PI * 2 / 4;
		RotateReset = 0.0f;
	}

	if (MenuTab == tab)
	{
		ImRotateStart();
		if (sameline) ImGui::SameLine();
		if (ImGui::ImageButton((GLuint*)texture_id[texture], ImVec2(42, 44), ImVec2(0, 0.0f + RotateImage), ImVec2(1, 1.0f + RotateImage)))MenuTab = 0;
		ImRotateEnd(RotateButton);
	}
	else
	{
		if (sameline) ImGui::SameLine();
		if (ImGui::ImageButton((GLuint*)texture_id[texture], ImVec2(42, 44)))
		{
			MenuTab = tab;
			RotateImage = 0.0f;
			RotateButton = M_PI * 2 / 4;
			RotateReset = 0.0f;
		}
	}
}

void ImageBorder(int from, int to)
{
	static float RotateImage = 0.0f;
	static DWORD Tickcount = 0;
	static DWORD Tickcheck = 0;
	if (cvar.gui_menu_image_spin)
	{
		if (GetTickCount() - Tickcount >= 1)
		{
			if (Tickcheck != Tickcount)
			{
				RotateImage += 0.001f * cvar.gui_menu_image_spin;
				Tickcheck = Tickcount;
			}
			Tickcount = GetTickCount();
		}

		if (RotateImage > 1.0f || RotateImage < -1.0f)
			RotateImage = 0.0f;
	}
	else
		RotateImage = 0.0f;

	float xfrom = ImGui::GetCurrentWindow()->Pos.x;
	float xto = ImGui::GetCurrentWindow()->Pos.x + ImGui::GetCurrentWindow()->Size.x;
	const ImRect bb(ImVec2(xfrom, ImGui::GetCurrentWindow()->DC.CursorPos.y + from), ImVec2(xto, ImGui::GetCurrentWindow()->DC.CursorPos.y + to));
	if (cvar.gui_menu_image)ImGui::GetWindowDrawList()->AddImage((GLuint*)texture_id[MENUBACK], ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), ImVec2(0.0f + RotateImage, 0), ImVec2(1.0f + RotateImage, 1));
}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		* out_text = items[idx];
	return true;
}

bool ComboBox(const char* label, float* current_item, const char* const items[], int items_count, int height_in_items) 
{
	ImGui::PushItemWidth(-1);
	char str[256];
	sprintf(str, "##%s", label);
	const bool value_changed = ImGui::Combo(str, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
	ImGui::PopItemWidth();
	return value_changed;
}

bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format)
{
	ImGui::PushItemWidth(-1);
	char str[256];
	sprintf(str, "##%s", label);
	const bool value_changed = ImGui::SliderFloat(str, v, v_min, v_max, format);
	ImGui::PopItemWidth();
	return value_changed;
}

tstring VirtualKeyCodeToString(UCHAR virtualKey)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	TCHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
	default:
		result = GetKeyNameText(scanCode << 16, szName, 128);
	}
	if (result == 0)
		throw std::system_error(std::error_code(GetLastError(), std::system_category()),
			"WinAPI Error occured.");
	return szName;
}

void KeyBind(float& key, char* keyname)
{
	bool clicked = false;
	char str[256];

	if (key == -2)
	{
		for (unsigned int i = 0; i < 255; i++)
		{
			if (keys[i])
			{
				if (i == VK_ESCAPE || i == cvar.gui_key)
				{
					key = -1;
					break;
				}
				key = i;
			}
		}
	}

	if (key == -1)
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		sprintf(str, "[No key]##%s", keyname);
		if (ImGui::Button(str))
			clicked = true;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		if (keyname != NULL)ImGui::SameLine();
		if (keyname != NULL)ImGui::Text(keyname);
	}
	else if (key == -2)
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		sprintf(str, "[Press key]##%s", keyname);
		if (ImGui::Button(str))
			clicked = true;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		if (keyname != NULL)ImGui::SameLine();
		if (keyname != NULL)ImGui::Text(keyname);
	}
	else
	{
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		sprintf(str, "[%s]##%s", VirtualKeyCodeToString((int)key).c_str(), keyname);
		if (ImGui::Button(str))
			clicked = true;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		if (keyname != NULL)ImGui::SameLine();
		if (keyname != NULL)ImGui::Text(keyname);
	}

	if (clicked)
	{
		if (key == -2)
			key = -1;
		else
			key = -2;
	}
}

int Weapon_ID()
{
	if (cvar.menu_legit_global_section == 0)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_GLOCK18;
		else if (cvar.menu_legit_sub_section == 1)
			return WEAPON_P228;
		else if (cvar.menu_legit_sub_section == 2)
			return WEAPON_DEAGLE;
		else if (cvar.menu_legit_sub_section == 3)
			return WEAPON_ELITE;
		else if (cvar.menu_legit_sub_section == 4)
			return WEAPON_FIVESEVEN;
		else if (cvar.menu_legit_sub_section == 5)
			return WEAPON_USP;
	}
	else if (cvar.menu_legit_global_section == 1)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_AWP;
		else if (cvar.menu_legit_sub_section == 1)
			return WEAPON_SCOUT;
		else if (cvar.menu_legit_sub_section == 2)
			return WEAPON_G3SG1;
		else if (cvar.menu_legit_sub_section == 3)
			return WEAPON_SG550;
	}
	else if (cvar.menu_legit_global_section == 2)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_M4A1;
		else if (cvar.menu_legit_sub_section == 1)
			return WEAPON_GALIL;
		else if (cvar.menu_legit_sub_section == 2)
			return WEAPON_FAMAS;
		else if (cvar.menu_legit_sub_section == 3)
			return WEAPON_AUG;
		else if (cvar.menu_legit_sub_section == 4)
			return WEAPON_AK47;
		else if (cvar.menu_legit_sub_section == 5)
			return WEAPON_SG552;
	}
	else if (cvar.menu_legit_global_section == 3)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_XM1014;
		else if (cvar.menu_legit_sub_section == 1)
			return WEAPON_M3;
	}
	else if (cvar.menu_legit_global_section == 4)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_M249;
	}
	else if (cvar.menu_legit_global_section == 5)
	{
		if (cvar.menu_legit_sub_section == 0)
			return WEAPON_TMP;
		else if (cvar.menu_legit_sub_section == 1)
			return WEAPON_P90;
		else if (cvar.menu_legit_sub_section == 2)
			return WEAPON_MP5N;
		else if (cvar.menu_legit_sub_section == 3)
			return WEAPON_MAC10;
		else if (cvar.menu_legit_sub_section == 4)
			return WEAPON_UMP45;
	}
	else return 0;
}

int RageWeapon_ID()
{
	if (cvar.menu_rage_global_section == 0)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_GLOCK18;
		else if (cvar.menu_rage_sub_section == 1)
			return WEAPON_P228;
		else if (cvar.menu_rage_sub_section == 2)
			return WEAPON_DEAGLE;
		else if (cvar.menu_rage_sub_section == 3)
			return WEAPON_ELITE;
		else if (cvar.menu_rage_sub_section == 4)
			return WEAPON_FIVESEVEN;
		else if (cvar.menu_rage_sub_section == 5)
			return WEAPON_USP;
	}
	else if (cvar.menu_rage_global_section == 1)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_AWP;
		else if (cvar.menu_rage_sub_section == 1)
			return WEAPON_SCOUT;
		else if (cvar.menu_rage_sub_section == 2)
			return WEAPON_G3SG1;
		else if (cvar.menu_rage_sub_section == 3)
			return WEAPON_SG550;
	}
	else if (cvar.menu_rage_global_section == 2)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_M4A1;
		else if (cvar.menu_rage_sub_section == 1)
			return WEAPON_GALIL;
		else if (cvar.menu_rage_sub_section == 2)
			return WEAPON_FAMAS;
		else if (cvar.menu_rage_sub_section == 3)
			return WEAPON_AUG;
		else if (cvar.menu_rage_sub_section == 4)
			return WEAPON_AK47;
		else if (cvar.menu_rage_sub_section == 5)
			return WEAPON_SG552;
	}
	else if (cvar.menu_rage_global_section == 3)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_XM1014;
		else if (cvar.menu_rage_sub_section == 1)
			return WEAPON_M3;
	}
	else if (cvar.menu_rage_global_section == 4)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_M249;
	}
	else if (cvar.menu_rage_global_section == 5)
	{
		if (cvar.menu_rage_sub_section == 0)
			return WEAPON_TMP;
		else if (cvar.menu_rage_sub_section == 1)
			return WEAPON_P90;
		else if (cvar.menu_rage_sub_section == 2)
			return WEAPON_MP5N;
		else if (cvar.menu_rage_sub_section == 3)
			return WEAPON_MAC10;
		else if (cvar.menu_rage_sub_section == 4)
			return WEAPON_UMP45;
	}
	else return 0;
}

void ChangeSubSection()
{
	if (cvar.menu_legit_global_section == 0)
	{
		if (cvar.menu_legit_sub_section > 5)
			cvar.menu_legit_sub_section = 5;
	}
	else if (cvar.menu_legit_global_section == 1)
	{
		if (cvar.menu_legit_sub_section > 3)
			cvar.menu_legit_sub_section = 3;
	}
	else if (cvar.menu_legit_global_section == 2)
	{
		if (cvar.menu_legit_sub_section > 5)
			cvar.menu_legit_sub_section = 5;
	}
	else if (cvar.menu_legit_global_section == 3)
	{
		if (cvar.menu_legit_sub_section > 1)
			cvar.menu_legit_sub_section = 1;
	}
	else if (cvar.menu_legit_global_section == 4)
	{
		if (cvar.menu_legit_sub_section > 0)
			cvar.menu_legit_sub_section = 0;
	}
	else if (cvar.menu_legit_global_section == 5)
	{
		if (cvar.menu_legit_sub_section > 4)
			cvar.menu_legit_sub_section = 4;
	}
}

void ChangeSubSectionRage()
{
	if (cvar.menu_rage_global_section == 0)
	{
		if (cvar.menu_rage_sub_section > 5)
			cvar.menu_rage_sub_section = 5;
	}
	else if (cvar.menu_rage_global_section == 1)
	{
		if (cvar.menu_rage_sub_section > 3)
			cvar.menu_rage_sub_section = 3;
	}
	else if (cvar.menu_rage_global_section == 2)
	{
		if (cvar.menu_rage_sub_section > 5)
			cvar.menu_rage_sub_section = 5;
	}
	else if (cvar.menu_rage_global_section == 3)
	{
		if (cvar.menu_rage_sub_section > 1)
			cvar.menu_rage_sub_section = 1;
	}
	else if (cvar.menu_rage_global_section == 4)
	{
		if (cvar.menu_rage_sub_section > 0)
			cvar.menu_rage_sub_section = 0;
	}
	else if (cvar.menu_rage_global_section == 5)
	{
		if (cvar.menu_rage_sub_section > 4)
			cvar.menu_rage_sub_section = 4;
	}
}

void DrawRage()
{
	ImGui::BeginChild("Aimbot - General", ImVec2(200, 371), true);
	{
		ImGui::Text("Rage Aimbot"), ImGui::Separator();

		ImGui::Checkbox("Activate", &cvar.rage_active);
		ImGui::Checkbox("Auto Fire", &cvar.rage_auto_fire);
		ImGui::Checkbox("Automatic Penetration", &cvar.rage_penetration);
		ImGui::Checkbox("Silent Aim", &cvar.rage_silent);
		ImGui::Checkbox("Shield Attack", &cvar.rage_shield_attack);
		ImGui::Checkbox("Team Attack", &cvar.rage_team_attack);
		ImGui::Text("Target Selection");
		const char* listbox_target[] = { "Field Of View", "Distance", "Cycle" };
		ComboBox("Target Selection", &cvar.rage_target_selection, listbox_target, IM_ARRAYSIZE(listbox_target), 3);
		ImGui::Text("Target Bone");
		const char* listbox_bone[] = { "None", "Head", "Chest", "Stomach", "All" };
		ComboBox("Target Bone", &cvar.rage_bone, listbox_bone, IM_ARRAYSIZE(listbox_bone), 5);
		ImGui::Text("Target Multi Bone");
		const char* listbox_multi[] = { "None", "Only Head", "Below Head", "All" };
		ComboBox("Multi Bone", &cvar.rage_multipoint_bone, listbox_multi, IM_ARRAYSIZE(listbox_multi), 4);
		ImGui::Text("Knife Attack Method");
		const char* listbox_knife[] = { "Slash", "Stab" };
		ComboBox("Knife Attack Method", &cvar.rage_knife_attack, listbox_knife, IM_ARRAYSIZE(listbox_knife), 2);
		ImGui::Text("Remove Spread Method");
		const char* listbox_spread[] = { "Pitch / Yaw", "Roll" };
		ComboBox("Remove Spread Method", &cvar.rage_nospread_method, listbox_spread, IM_ARRAYSIZE(listbox_spread), 2);
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Aimbot - Other", ImVec2(200, 371), true);
	{
		ImGui::Text("Weapon Settings"); ImGui::Separator();

		const char* listbox_edge[] = { "Pistol", "Sniper", "Rifle", "Shotgun", "Machine Gun", "Submachine Gun" };
		ComboBox("Section", &cvar.menu_rage_global_section, listbox_edge, IM_ARRAYSIZE(listbox_edge), 6);
		ChangeSubSectionRage();
		if (cvar.menu_rage_global_section == 0)
		{
			const char* listbox_sub1[] = { "GLOCK18", "P228", "DEAGLE", "ELITE", "FIVESEVEN", "USP" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub1, IM_ARRAYSIZE(listbox_sub1), 6);
		}
		else if (cvar.menu_rage_global_section == 1)
		{
			const char* listbox_sub2[] = { "AWP", "SCOUT", "G3SG1", "SG550" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub2, IM_ARRAYSIZE(listbox_sub2), 4);
		}
		else if (cvar.menu_rage_global_section == 2)
		{
			const char* listbox_sub3[] = { "M4A1", "GALIL", "FAMAS", "AUG", "AK47", "SG552" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub3, IM_ARRAYSIZE(listbox_sub3), 6);
		}
		else if (cvar.menu_rage_global_section == 3)
		{
			const char* listbox_sub4[] = { "XM1014", "M3" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub4, IM_ARRAYSIZE(listbox_sub4), 2);
		}
		else if (cvar.menu_rage_global_section == 4)
		{
			const char* listbox_sub5[] = { "M249" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub5, IM_ARRAYSIZE(listbox_sub5), 1);
		}
		else if (cvar.menu_rage_global_section == 5)
		{
			const char* listbox_sub6[] = { "TMP", "P90", "MP5N", "MAC10", "UMP45" };
			ComboBox("Weapon", &cvar.menu_rage_sub_section, listbox_sub6, IM_ARRAYSIZE(listbox_sub6), 5);
		}
		if (RageWeapon_ID())
		{
			if (WeaponTiming(RageWeapon_ID()))
			{
				SliderFloat("Shot Delay", &cvar.rage[RageWeapon_ID()].rage_delay_shot, 0.f, 1000.f, "Shot Delay: %.0fms");
				const char* listbox_count[] = { "Count", "Random" };
				ComboBox("Shot Type", &cvar.rage[RageWeapon_ID()].rage_shot_type, listbox_count, IM_ARRAYSIZE(listbox_count), 2);
				if (!cvar.rage[RageWeapon_ID()].rage_shot_type) SliderFloat("Shot Count", &cvar.rage[RageWeapon_ID()].rage_shot_count, 1.f, 5.f, "Shot Count: %.0f");
				if (cvar.rage[RageWeapon_ID()].rage_shot_type) SliderFloat("Random Max", &cvar.rage[RageWeapon_ID()].rage_random_max, 2.f, 5.f, "Random Max: %.0f");
			}
			SliderFloat("Recoil", &cvar.rage[RageWeapon_ID()].recoil, 0.f, 2.f, "Recoil: %.4f");
		}
		ImGui::Separator();
	}
	ImGui::EndChild();

	ImGui::BeginChild("Aimbot - Fakelag", ImVec2(200, 216), true);
	{
		ImGui::Text("Empty"), ImGui::Separator();
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Aimbot - Anti-Aimbot", ImVec2(200, 216), true);
	{
		ImGui::Text("Anti Aim"), ImGui::Separator();

		ImGui::Text("Pitch");
		const char* listbox_pitch[] = { "None", "Fakedown", "Random" };
		ComboBox("Pitch", &cvar.aa_pitch, listbox_pitch, IM_ARRAYSIZE(listbox_pitch), 3);
		ImGui::Text("Yaw");
		const char* listbox_yaw[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
		ComboBox("Yaw", &cvar.aa_yaw, listbox_yaw, IM_ARRAYSIZE(listbox_yaw), 8);
		ImGui::Text("Yaw While Running");
		const char* listbox_yawrun[] = { "None", "180", "180 Jitter", "Spin", "Jitter", "Sideways", "Random", "Static" };
		ComboBox("Yaw While Running", &cvar.aa_yaw_while_running, listbox_yawrun, IM_ARRAYSIZE(listbox_yawrun), 8);
		SliderFloat("Static Yaw", &cvar.aa_yaw_static, -180.f, 180.f, "Static Yaw: %.0f");
		ImGui::Text("Edge");
		const char* listbox_edge[] = { "None", "Standing", "In Move", "All", "All + Fake" };
		ComboBox("Edge", &cvar.aa_edge, listbox_edge, IM_ARRAYSIZE(listbox_edge), 5);
	}
	ImGui::EndChild();
}

void DrawLegit()
{
	ImGui::BeginChild("Legit - Section", ImVec2(200, 35), true);
	{
		const char* listbox_section[] = { "Pistol", "Sniper", "Rifle", "Shotgun", "Machine Gun", "Submachine Gun" };
		ComboBox("Section", &cvar.menu_legit_global_section, listbox_section, IM_ARRAYSIZE(listbox_section), 6);
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Legit - Weapon", ImVec2(200, 35), true);
	{
		ChangeSubSection();

		if (cvar.menu_legit_global_section == 0)
		{
			const char* listbox_sub1[] = { "GLOCK18", "P228", "DEAGLE", "ELITE", "FIVESEVEN", "USP" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub1, IM_ARRAYSIZE(listbox_sub1), 6);
		}
		else if (cvar.menu_legit_global_section == 1)
		{
			const char* listbox_sub2[] = { "AWP", "SCOUT", "G3SG1", "SG550" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub2, IM_ARRAYSIZE(listbox_sub2), 4);
		}
		else if (cvar.menu_legit_global_section == 2)
		{
			const char* listbox_sub3[] = { "M4A1", "GALIL", "FAMAS", "AUG", "AK47", "SG552" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub3, IM_ARRAYSIZE(listbox_sub3), 6);
		}
		else if (cvar.menu_legit_global_section == 3)
		{
			const char* listbox_sub4[] = { "XM1014", "M3" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub4, IM_ARRAYSIZE(listbox_sub4), 2);
		}
		else if (cvar.menu_legit_global_section == 4)
		{
			const char* listbox_sub5[] = { "M249" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub5, IM_ARRAYSIZE(listbox_sub5), 1);
		}
		else if (cvar.menu_legit_global_section == 5)
		{
			const char* listbox_sub6[] = { "TMP", "P90", "MP5N", "MAC10", "UMP45" };
			ComboBox("Weapon", &cvar.menu_legit_sub_section, listbox_sub6, IM_ARRAYSIZE(listbox_sub6), 5);
		}
	}
	ImGui::EndChild();

	if (Weapon_ID())
	{
		ImGui::BeginChild("Legit - General", ImVec2(200, 441), true);
		{
			ImGui::Text("Legit Aimbot"), ImGui::Separator();

			ImGui::Checkbox("Activate", &cvar.legit[Weapon_ID()].active);
			ImGui::Checkbox("Humanize", &cvar.legit[Weapon_ID()].humanize);
			ImGui::Checkbox("Silent", &cvar.legit[Weapon_ID()].silent);
			ImGui::Checkbox("Head Bone", &cvar.legit[Weapon_ID()].head_bone);
			ImGui::Checkbox("Chest Bone", &cvar.legit[Weapon_ID()].chest_bone);
			ImGui::Checkbox("Stomach Bone", &cvar.legit[Weapon_ID()].stomach_bone);
			ImGui::Checkbox("All Other Bone", &cvar.legit[Weapon_ID()].all_bone);
			const char* listbox_accuracy[] = { "None", "Aiming", "Recoil", "Recoil / Spread" };
			SliderFloat("Auto Speed", &cvar.legit[Weapon_ID()].speed, 0.f, 100.f, "Auto Speed: %.0f%%");
			SliderFloat("Attack Speed", &cvar.legit[Weapon_ID()].speed_in_attack, 0.f, 100.f, "Attack Speed: %.0f%%");
			SliderFloat("Block Attack", &cvar.legit[Weapon_ID()].block_attack_after_kill, 0.f, 1000.f, "Block Attack: %.0fms");
			SliderFloat("Maximum FOV", &cvar.legit[Weapon_ID()].fov, 0.f, 45.f, "Maximum FOV: %.0f");
			SliderFloat("Recoil Pitch", &cvar.legit[Weapon_ID()].recoil_compensation_pitch, 0.f, 100.f, "Recoil Pitch: %.0f%%");
			SliderFloat("Recoil Yaw", &cvar.legit[Weapon_ID()].recoil_compensation_yaw, 0.f, 100.f, "Recoil Yaw: %.0f%%");
			SliderFloat("Recoil Compensate", &cvar.legit[Weapon_ID()].recoil_compensation_after_shots_fired, 0.f, 10.f, "Recoil Compensate: %.0f");
			SliderFloat("Speed Scale FOV", &cvar.legit[Weapon_ID()].speed_scale_fov, 0.f, 100.f, "Speed Scale FOV: %.0f%%");
			SliderFloat("Shot Delay", &cvar.legit[Weapon_ID()].reaction_time, 0.f, 1000.f, "Shot Delay: %.0fms");
			ImGui::Text("Accuracy Boost");
			ComboBox("Accuracy Boost", &cvar.legit[Weapon_ID()].accuracy, listbox_accuracy, IM_ARRAYSIZE(listbox_accuracy), 4);
		}
		ImGui::EndChild();

		Spacing();

		ImGui::BeginChild("Aimbot - Trigger", ImVec2(200, 441), true);
		{
			ImGui::Text("Trigger Aimbot"), ImGui::Separator();

			ImGui::Checkbox("Activate", &cvar.legit[Weapon_ID()].trigger_active);
			ImGui::Checkbox("Automatic Penetration", &cvar.legit[Weapon_ID()].trigger_penetration);
			ImGui::Checkbox("Only Zoom", &cvar.legit_trigger_only_zoom);
			ImGui::Checkbox("Head Bone", &cvar.legit[Weapon_ID()].trigger_bone_head);
			ImGui::Checkbox("Chest Bone", &cvar.legit[Weapon_ID()].trigger_bone_chest);
			ImGui::Checkbox("Stomach Bone", &cvar.legit[Weapon_ID()].trigger_bone_stomach);
			ImGui::Checkbox("All Other Bone", &cvar.legit[Weapon_ID()].trigger_bone_all);
			if (WeaponTiming(Weapon_ID())) SliderFloat("Shot Delay", &cvar.legit[Weapon_ID()].trigger_delay_shot, 0.f, 1000.f, "Shot Delay: %.0fms");
			const char* listbox_count[] = { "Count", "Random" };
			if (WeaponTiming(Weapon_ID()))ComboBox("Shot Type", &cvar.legit[Weapon_ID()].trigger_shot_type, listbox_count, IM_ARRAYSIZE(listbox_count), 2);
			if (WeaponTiming(Weapon_ID()) && !cvar.legit[Weapon_ID()].trigger_shot_type) SliderFloat("Shot Count", &cvar.legit[Weapon_ID()].trigger_shot_count, 1.f, 5.f, "Shot Count: %.0f");
			if (WeaponTiming(Weapon_ID()) && cvar.legit[Weapon_ID()].trigger_shot_type) SliderFloat("Random Max", &cvar.legit[Weapon_ID()].trigger_random_max, 2.f, 5.f, "Random Max: %.0f");
			ImGui::Text("Accuracy Boost");
			const char* listbox_accuracy[] = { "None", "Recoil", "Recoil / Spread" };
			ComboBox("Accuracy Boost", &cvar.legit[Weapon_ID()].trigger_accuracy, listbox_accuracy, IM_ARRAYSIZE(listbox_accuracy), 3);
			ImGui::Separator();
			ImGui::Text("Other");
			ImGui::Checkbox("Fast Zoom", &cvar.legit_fast_zoom);
			ImGui::Checkbox("Team Attack", &cvar.legit_team_attack);
			ImGui::Separator();
		}
		ImGui::EndChild();
	}
}

void DrawKnifebot()
{
	ImGui::BeginChild("Knifebot - General", ImVec2(200, 142), true);
	{
		ImGui::Text("Knifebot"), ImGui::Separator();

		ImGui::Checkbox("Activate", &cvar.knifebot_active);
		ImGui::Checkbox("Silent", &cvar.knifebot_silent);
		ImGui::Text("Knife Attack Method");
		const char* listbox_attack[] = { "Slash", "Stab" };
		ComboBox("Knife Attack Method", &cvar.knifebot_attack, listbox_attack, IM_ARRAYSIZE(listbox_attack), 2);
		if (cvar.knifebot_attack == 0)SliderFloat("Slash Distance", &cvar.knifebot_attack_distance, 0.f, 100.f, "Slash Distance: %.0f");
		if (cvar.knifebot_attack == 1)SliderFloat("Stab Distance", &cvar.knifebot_attack2_distance, 0.f, 100.f, "Stab Distance: %.0f");
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Empty", ImVec2(200, 142), true);
	{
		ImGui::Text("Empty"), ImGui::Separator();
	}
	ImGui::EndChild();
}

void DrawMisc()
{
	ImGui::BeginChild("Misc - General", ImVec2(200, 148), true);
	{
		ImGui::Text("Misc1"), ImGui::Separator();

		ImGui::Checkbox("Debug Bypass Trace", &cvar.bypass_trace_blockers);
		ImGui::Checkbox("Display My Weapon Name", &cvar.misc_weapon_local);
		ImGui::Checkbox("Clean Snapshot", &cvar.misc_snapshot);
		SliderFloat("Snapshot Time", &cvar.misc_snapshot_time, 1.f, 60.f, "Snapshot Time: %.0f");
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Misc - General2", ImVec2(200, 148), true);
	{
		ImGui::Text("Misc2"), ImGui::Separator();

		ImGui::Checkbox("Gui Chat", &cvar.gui_chat);
		KeyBind(cvar.gui_chat_key, "Gui Chat Key");
		KeyBind(cvar.gui_chat_key_team, "Chat Key Team");
		ImGui::Checkbox("Quick Change", &cvar.misc_quick_change);
		KeyBind(cvar.misc_quick_change_key, "Quick Change Key");
	}
	ImGui::EndChild();
}

void DrawChams()
{
	ImGui::BeginChild("Chams - General", ImVec2(200, 665), true);
	{
		ImGui::Text("Chams"), ImGui::Separator();

		ImGui::Text("Player");
		ImGui::Checkbox("Glow", &cvar.chams_player_glow);
		ImGui::Checkbox("Wall", &cvar.chams_player_wall);
		const char* listbox_player[] = { "None", "Material", "Texture", "Flat" };
		ComboBox("Player.", &cvar.chams_player, listbox_player, IM_ARRAYSIZE(listbox_player), 4);
		ImGui::Separator();
		ImGui::Text("Third Person");
		ImGui::Checkbox("Glow##2", &cvar.chams_third_person_glow);
		ImGui::Checkbox("Wall##2", &cvar.chams_third_person_wall);
		const char* listbox_third_person[] = { "None", "Material", "Texture", "Flat" };
		ComboBox("Third Person.", &cvar.chams_third_person, listbox_third_person, IM_ARRAYSIZE(listbox_third_person), 4);
		ImGui::Separator();
		ImGui::Text("View Model");
		ImGui::Checkbox("Glow##3", &cvar.chams_view_model_glow);
		const char* listbox_weapon[] = { "None", "Material", "Texture", "Flat" };
		ComboBox("View Model.", &cvar.chams_view_model, listbox_weapon, IM_ARRAYSIZE(listbox_weapon), 4);
		ImGui::Separator();
		ImGui::Text("Weapon World");
		ImGui::Checkbox("Glow##4", &cvar.chams_weapon_glow);
		ImGui::Checkbox("Wall##3", &cvar.chams_weapon_wall);
		const char* listbox_weapon_world[] = { "None", "Material", "Texture", "Flat" };
		ComboBox("Weapon World.", &cvar.chams_weapon, listbox_weapon_world, IM_ARRAYSIZE(listbox_weapon_world), 4);
		ImGui::Separator();
		ImGui::Text("Chicken");
		ImGui::Checkbox("Glow##5", &cvar.chams_chicken_glow);
		ImGui::Checkbox("Wall##4", &cvar.chams_chicken_wall);
		const char* listbox_chicken[] = { "None", "Material", "Texture", "Flat" };
		ComboBox("Chicken.", &cvar.chams_chicken, listbox_chicken, IM_ARRAYSIZE(listbox_chicken), 4);
		ImGui::Separator();
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Skeleton - General", ImVec2(200, 665), true);
	{
		ImGui::Text("Skeleton"), ImGui::Separator();
		
		ImGui::Text("Player");
		ImGui::Checkbox("Bone##1", &cvar.skeleton_player_body);
		ImGui::Checkbox("BoneBox##1", &cvar.skeleton_player_bonebox);
		ImGui::Checkbox("Bone Number##1", &cvar.skeleton_player_bonebox_num);
		ImGui::Checkbox("HitBox##1", &cvar.skeleton_player_hitbox);
		ImGui::Checkbox("HitBox Number##1", &cvar.skeleton_player_hitbox_num);
		ImGui::Checkbox("Stick Figure##1", &cvar.skeleton_player_stick_figure);
		ImGui::Checkbox("Stick Figure Only##1", &cvar.skeleton_player_stick_figure_only);
		ImGui::Checkbox("Face##1", &cvar.skeleton_face_player);
		const char* listbox_skeleton_model[] = { "None", "Draw Bones", "Draw Hulls" };
		ComboBox("Player.", &cvar.skeleton_models, listbox_skeleton_model, IM_ARRAYSIZE(listbox_skeleton_model), 3);
		ImGui::Separator();
		ImGui::Text("Third Person");
		ImGui::Checkbox("Bone##2", &cvar.skeleton_third_person_body);
		ImGui::Checkbox("BoneBox##2", &cvar.skeleton_third_person_bonebox);
		ImGui::Checkbox("Bone Number##2", &cvar.skeleton_third_person_bonebox_num);
		ImGui::Checkbox("HitBox##2", &cvar.skeleton_third_person_hitbox);
		ImGui::Checkbox("HitBox Number##2", &cvar.skeleton_third_person_hitbox_num);
		ImGui::Checkbox("Stick Figure##2", &cvar.skeleton_third_person_stick_figure);
		ImGui::Checkbox("Stick Figure Only##2", &cvar.skeleton_third_person_stick_figure_only);
		ImGui::Checkbox("Face##2", &cvar.skeleton_face_third_person);
		const char* listbox_skeleton_model_tp[] = { "None", "Draw Bones", "Draw Hulls" };
		ComboBox("Third Person.", &cvar.skeleton_models_third_person, listbox_skeleton_model_tp, IM_ARRAYSIZE(listbox_skeleton_model_tp), 3);
		ImGui::Separator();
		ImGui::Text("Weapon World");
		ImGui::Checkbox("Hitbox##3", &cvar.skeleton_weapon_world_box);
		ImGui::Separator();
		ImGui::Text("Chicken");
		ImGui::Checkbox("Bone##3", &cvar.skeleton_chicken_bone);
		ImGui::Checkbox("Bone Number##3", &cvar.skeleton_chicken_bone_num);
		ImGui::Checkbox("Hitbox##4", &cvar.skeleton_chicken_box);
		ImGui::Checkbox("Hitbox Number##3", &cvar.skeleton_chicken_box_num);
		const char* listbox_skeleton_model_chicken[] = { "None", "Draw Bones", "Draw Hulls" };
		ComboBox("Chicken.", &cvar.skeleton_models_chicken, listbox_skeleton_model_chicken, IM_ARRAYSIZE(listbox_skeleton_model_chicken), 3);
	}
	ImGui::EndChild();
}

void DrawEsp()
{
	ImGui::BeginChild("Visual - General", ImVec2(200, 550), true);
	{
		ImGui::Text("Visual"), ImGui::Separator();

		ImGui::Text("Player");
		ImGui::Checkbox("Box", &cvar.esp_box);
		ImGui::Checkbox("Headshot Mark", &cvar.esp_headshot_mark);
		ImGui::Checkbox("Health Bar", &cvar.esp_health);
		ImGui::Checkbox("Kill Lightning", &cvar.esp_kill_lightning);
		ImGui::Checkbox("Lambert", &cvar.esp_lambert);
		ImGui::Checkbox("Model Name", &cvar.esp_model_name);
		ImGui::Checkbox("Name", &cvar.esp_name);
		ImGui::Checkbox("Radar", &cvar.esp_radar);
		ImGui::Checkbox("Reload", &cvar.esp_reload_bar);
		ImGui::Checkbox("Sound", &cvar.esp_sound);
		ImGui::Checkbox("Vip", &cvar.esp_vip);
		ImGui::Checkbox("Visual Team", &cvar.esp_visual_team);
		ImGui::Checkbox("Wall", &cvar.esp_wall);
		ImGui::Checkbox("Weapon Name", &cvar.esp_weapon);
		ImGui::Separator();
		ImGui::Text("Third Person");
		ImGui::Checkbox("Chase", &cvar.esp_chase_cam);
		ImGui::Separator();
		ImGui::Text("View model");
		ImGui::Checkbox("No Hands", &cvar.esp_view_model_no_hands);
		ImGui::Separator();
		ImGui::Text("Other");
		ImGui::Checkbox("Crosshair", &cvar.esp_crosshair);
		ImGui::Checkbox("Grenade Trajectory", &cvar.esp_grenade_trajectory);
		ImGui::Checkbox("Skin", &cvar.esp_skins);
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Visual - Other", ImVec2(200, 550), true);
	{
		ImGui::Text("Visual Adjust"), ImGui::Separator();

		SliderFloat("Alpha Color All", &cvar.esp_alpha, 0.f, 1.f, "Alpha Color All: %.2f");
		ImGui::Separator();
		ImGui::Text("Player");
		SliderFloat("Box Height", &cvar.esp_box_height, 0.05f, 1.f, "Box Height: %.2f");
		SliderFloat("Box Width", &cvar.esp_box_width, 0.05f, 1.f, "Box Width: %.2f");
		SliderFloat("Box Top", &cvar.esp_box_size_top, 1.f, 100.f, "Box Top: %.0f");
		SliderFloat("Box Bot", &cvar.esp_box_size_bot, 1.f, 100.f, "Box Bot: %.0f");
		SliderFloat("Radar Size", &cvar.esp_radar_size, 25.f, 100.f, "Radar Size: %.0f");
		SliderFloat("Rounding", &cvar.esp_rounding, 0.f, 5.f, "Rounding: %.0f");
		ImGui::Separator();
		ImGui::Text("Other");
		SliderFloat("Crosshair Offset", &cvar.esp_crosshair_offset, 0.f, 25.f, "Crosshair Offset: %.0f");
		SliderFloat("Crosshair Spin Speed", &cvar.esp_crosshair_spin, -10.f, 10.f, "Crosshair Spin: %.1f");
		SliderFloat("Custom FOV", &cvar.esp_custom_fov, 45.f, 150.f, "FOV: %.0f");
		ImGui::Separator();
	}
	ImGui::EndChild();
}

void DrawKz()
{
	ImGui::BeginChild("Kz - General", ImVec2(200, 171), true);
	{
		ImGui::Text("Kz1"), ImGui::Separator();
		
		ImGui::Checkbox("Bhop", &cvar.kz_bhop);
		ImGui::Checkbox("Fast Run", &cvar.kz_fast_run);
		ImGui::Checkbox("GStrafe", &cvar.kz_gstrafe);
		ImGui::Checkbox("Jump Bug", &cvar.kz_jump_bug);
		ImGui::Checkbox("Jump Bug Auto", &cvar.kz_jump_bug_auto);
		ImGui::Checkbox("Strafe", &cvar.kz_strafe);
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Kz2 - General", ImVec2(200, 171), true);
	{
		ImGui::Text("Kz2"), ImGui::Separator();
		
		ImGui::Checkbox("Kz Window", &cvar.kz_show_kz);
		KeyBind(cvar.kz_bhop_key, "Bhop Key");
		KeyBind(cvar.kz_fastrun_key, "Fastrun Key");
		KeyBind(cvar.kz_gstrafe_key, "Gstrafe Key");
		KeyBind(cvar.kz_jumpbug_key, "Jumpbug Key");
		KeyBind(cvar.kz_strafe_key, "Strafe Key");
	}
	ImGui::EndChild();
}

void DrawColor()
{
	ImGui::BeginChild("Color - General", ImVec2(200, 236), true);
	{
		ImGui::Text("Color"), ImGui::Separator();
		
		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		ImVec4 prevColor2 = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.5f);
		SliderFloat("Color Red", &cvar.color_red, 0.f, 1.f, "Color Red: %.3f");
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = prevColor;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor2;

		prevColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		prevColor2 = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 1.0f, 0.0f, 0.5f);
		SliderFloat("Color Green", &cvar.color_green, 0.f, 1.f, "Color Green: %.3f");
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = prevColor;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor2;

		prevColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		prevColor2 = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 1.0f, 0.5f);
		SliderFloat("Color Blue", &cvar.color_blue, 0.f, 1.f, "Color Blue: %.3f");
		ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = prevColor;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor2;

		float crl[3];
		crl[0] = cvar.color_red, crl[1] = cvar.color_green, crl[2] = cvar.color_blue;
		ImGui::ColorPicker4("##1", (float*)&crl, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_PickerHueWheel, NULL);
		cvar.color_red = crl[0], cvar.color_green = crl[1], cvar.color_blue = crl[2];
	}
	ImGui::EndChild();

	Spacing();

	ImGui::BeginChild("Color2", ImVec2(200, 236), true);
	{
		ImGui::Text("Color2"), ImGui::Separator();
		
		ImGui::Checkbox("Random Color", &cvar.color_random);
		SliderFloat("Speed", &cvar.color_speed, -10.f, 10.f, "Color Change Speed: %.0f");
	}
	ImGui::EndChild();
}

void DrawMenuWindow()
{
	if (!bShowMenu)
		return;

	static bool freegui = false;

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, 0), freegui? ImGuiCond_Once:ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImageBorder(1, 120);
		ImGui::BeginMenuBar();
		{
			if (ImGui::BeginMenu("Gui"))
			{
				ImGui::MenuItem("Image Mouse", NULL, &cvar.gui_mouse_image);
				ImGui::MenuItem("Image Menu", NULL, &cvar.gui_menu_image);
				ImGui::MenuItem("Free Gui", NULL, &freegui);
				if (!loadtexture)ImGui::MenuItem("Reload Menu Texture", NULL, &loadtexture);
				if (!loadtexturemodel)ImGui::MenuItem("Reload Model Texture", NULL, &loadtexturemodel);
				ImGui::MenuItem("ShowDemo", NULL, &show_demo);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Key"))
			{
				KeyBind(cvar.gui_key, NULL);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Image"))
			{
				ImGui::SliderFloat("##1", &cvar.gui_menu_button_spin, -10.f, 10.f, "Image Button Speed: %.1f");
				ImGui::SliderFloat("##2", &cvar.gui_menu_image_spin, -10.f, 10.f, "Image Menu Speed: %.1f");
				ImGui::SliderFloat("##3", &cvar.gui_mouse_image_spin, -10.f, 10.f, "Image Mouse Speed: %.1f");
				ImGui::SliderFloat("##4", &cvar.gui_mouse_image_amount, 1.f, 10.f, "Image Mouse Amount: %.0f");
				ImGui::EndMenu();
			}
			ImGui::SameLine(); 
			SYSTEMTIME SysTime;
			GetLocalTime(&SysTime);
			ImGui::SameLine(300); ImGui::Text("Time: %02d:%02d:%02d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
			ImGui::EndMenuBar();
		}
	
		ImGui::Separator();

		if (ImGui::Button("ID Hook")) func_player_toggle();
		ImGui::SameLine(); if (ImGui::Button("Console")) show_app_console = !show_app_console;

		ImGui::Separator();
		ImGui::Spacing();

		ImVec2 LastSapcing = ImGui::GetStyle().ItemSpacing;
		ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);
		Menutabs(1, RAGE, false);
		Menutabs(2, LEGIT, true);
		Menutabs(3, KNIFE, true);
		Menutabs(4, SETTINGS, true);
		Menutabs(5, MODEL, true);
		Menutabs(6, VISUAL, true);
		Menutabs(7, BUNNY, true);
		Menutabs(8, COLOR, true);
		ImGui::GetStyle().ItemSpacing = LastSapcing;

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		ImVec4 prevColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		if (ImGui::Button("Rage"))cvar.rage_active = 1;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImGui::SameLine();
		if (ImGui::Button("Legit"))cvar.rage_active = 0;
		ImGui::GetStyle().Colors[ImGuiCol_Text] = prevColor;
		
		ImGui::SameLine();
		if (cvar.rage_active)
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Rage Active");
		else
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Legit Active");
		
		ImGui::Separator();
		if (MenuTab == 1)
			DrawRage();
		if (MenuTab == 2)
			DrawLegit();
		if (MenuTab == 3)
			DrawKnifebot();
		if (MenuTab == 4)
			DrawMisc();
		if (MenuTab == 5)
			DrawChams();
		if (MenuTab == 6)
			DrawEsp();
		if (MenuTab == 7)
			DrawKz();
		if (MenuTab == 8)
			DrawColor();

		ImGui::End();
	}
}