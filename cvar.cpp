#include "client.h"

CVARlist cvar;

void CVARlist::init()
{
	#define INITCVAR(name,value) cmd.AddCvarFloat(#name, &##name );##name=##value;

	INITCVAR(rage_active, 0.0);
	INITCVAR(rage_penetration, 0.0); 
	INITCVAR(rage_team_attack, 0.0);
	INITCVAR(rage_bone, 1.0);
	INITCVAR(rage_multipoint_bone, 0.0);
	INITCVAR(rage_target_selection, 2.0);
	INITCVAR(rage_silent, 0.0); 
	INITCVAR(rage_knife_attack, 0.0);
	INITCVAR(rage_shield_attack, 0.0);
	INITCVAR(rage_nospread_method, 0.0);
	INITCVAR(rage_auto_fire, 1.0);

	for (int i = 1; i <= 30; i++)
	{
		if (i == 2 || i == 4 || i == 6 || i == 9 || i == 25 || i == 29) continue;

		char str[256];
		sprintf(str, "legit[%d].active", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].active); legit[i].active = 0.0;
		sprintf(str, "legit[%d].speed", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].speed); legit[i].speed = 0.0;
		sprintf(str, "legit[%d].speed_scale_fov", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].speed_scale_fov); legit[i].speed_scale_fov = 0.0;
		sprintf(str, "legit[%d].reaction_time", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].reaction_time); legit[i].reaction_time = 0.0;
		sprintf(str, "legit[%d].humanize", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].humanize); legit[i].humanize = 0.0;
		sprintf(str, "legit[%d].recoil_compensation_pitch", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].recoil_compensation_pitch); legit[i].recoil_compensation_pitch = 0.0;
		sprintf(str, "legit[%d].recoil_compensation_yaw", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].recoil_compensation_yaw); legit[i].recoil_compensation_yaw = 0.0;
		sprintf(str, "legit[%d].recoil_compensation_after_shots_fired", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].recoil_compensation_after_shots_fired); legit[i].recoil_compensation_after_shots_fired = 0.0;
		sprintf(str, "legit[%d].block_attack_after_kill", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].block_attack_after_kill); legit[i].block_attack_after_kill = 0.0;
		sprintf(str, "legit[%d].accuracy", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].accuracy); legit[i].accuracy = 0.0;
		sprintf(str, "legit[%d].silent", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].silent); legit[i].silent = 0.0;
		sprintf(str, "legit[%d].fov", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].fov); legit[i].fov = 5.0;
		sprintf(str, "legit[%d].speed_in_attack", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].speed_in_attack); legit[i].speed_in_attack = 100;
		sprintf(str, "legit[%d].stomach_bone", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].stomach_bone); legit[i].stomach_bone = 0.0;
		sprintf(str, "legit[%d].chest_bone", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].chest_bone); legit[i].chest_bone = 0.0;
		sprintf(str, "legit[%d].head_bone", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].head_bone); legit[i].head_bone = 1.0;
		sprintf(str, "legit[%d].all_bone", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].all_bone); legit[i].all_bone = 0.0;
		sprintf(str, "legit[%d].trigger_active", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_active); legit[i].trigger_active = 0.0;
		sprintf(str, "legit[%d].trigger_penetration", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_penetration); legit[i].trigger_penetration = 0.0;
		sprintf(str, "legit[%d].trigger_accuracy", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_accuracy); legit[i].trigger_accuracy = 0.0;
		sprintf(str, "legit[%d].trigger_bone_head", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_bone_head); legit[i].trigger_bone_head = 1.0;
		sprintf(str, "legit[%d].trigger_bone_chest", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_bone_chest); legit[i].trigger_bone_chest = 0.0;
		sprintf(str, "legit[%d].trigger_bone_stomach", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_bone_stomach); legit[i].trigger_bone_stomach = 0.0;
		sprintf(str, "legit[%d].trigger_bone_all", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_bone_all); legit[i].trigger_bone_all = 0.0;
		sprintf(str, "legit[%d].trigger_delay_shot", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_delay_shot); legit[i].trigger_delay_shot = 0.0;
		sprintf(str, "legit[%d].trigger_shot_count", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_shot_count); legit[i].trigger_shot_count = 1.0;
		sprintf(str, "legit[%d].trigger_shot_type", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_shot_type); legit[i].trigger_shot_type = 0.0;
		sprintf(str, "legit[%d].trigger_random_max", i);
		cmd.AddCvarFloat(str, &cvar.legit[i].trigger_random_max); legit[i].trigger_random_max = 3.0;
	}

	for (int i = 1; i <= 30; i++)
	{
		if (i == 2 || i == 4 || i == 6 || i == 9 || i == 25 || i == 29) continue;

		char str[256];
		sprintf(str, "rage[%d].recoil", i);
		cmd.AddCvarFloat(str, &cvar.rage[i].recoil); rage[i].recoil = 2.0;
		sprintf(str, "rage[%d].rage_delay_shot", i);
		cmd.AddCvarFloat(str, &cvar.rage[i].rage_delay_shot); rage[i].rage_delay_shot = 0.0;
		sprintf(str, "rage[%d].rage_shot_count", i);
		cmd.AddCvarFloat(str, &cvar.rage[i].rage_shot_count); rage[i].rage_shot_count = 1.0;
		sprintf(str, "rage[%d].rage_shot_type", i);
		cmd.AddCvarFloat(str, &cvar.rage[i].rage_shot_type); rage[i].rage_shot_type = 0.0;
		sprintf(str, "rage[%d].rage_random_max", i);
		cmd.AddCvarFloat(str, &cvar.rage[i].rage_random_max); rage[i].rage_random_max = 3.0;
	}

	INITCVAR(legit_team_attack, 0.0);
	INITCVAR(legit_trigger_only_zoom, 0.0);
	INITCVAR(legit_fast_zoom, 0.0);

	INITCVAR(menu_legit_global_section, 0.0);
	INITCVAR(menu_legit_sub_section, 0.0);
	INITCVAR(menu_rage_global_section, 0.0);
	INITCVAR(menu_rage_sub_section, 0.0);

	INITCVAR(knifebot_active, 0.0);
	INITCVAR(knifebot_attack, 1.0);
	INITCVAR(knifebot_attack_distance, 72.0);
	INITCVAR(knifebot_attack2_distance, 64.0);
	INITCVAR(knifebot_silent, 0.0);

	INITCVAR(bypass_trace_blockers, 1.0);

	INITCVAR(aa_pitch, 0.0);
	INITCVAR(aa_edge, 0.0);
	INITCVAR(aa_yaw, 0.0);
	INITCVAR(aa_yaw_static, 0.0);
	INITCVAR(aa_yaw_while_running, 0.0);

	INITCVAR(aim_id_mode, 1.0);

	INITCVAR(wav_speed, 1.0);

	INITCVAR(misc_snapshot, 0.0);
	INITCVAR(misc_snapshot_time, 10.0);
	INITCVAR(misc_quick_change, 0.0);
	INITCVAR(misc_quick_change_key, -1.0);
	INITCVAR(misc_weapon_local, 0.0);

	INITCVAR(chams_view_model, 0.0);
	INITCVAR(chams_view_model_glow, 0.0);
	INITCVAR(chams_player, 0.0);
	INITCVAR(chams_player_glow, 0.0);
	INITCVAR(chams_player_wall, 0.0);
	INITCVAR(chams_third_person, 0.0);
	INITCVAR(chams_third_person_glow, 0.0);
	INITCVAR(chams_third_person_wall, 0.0);
	INITCVAR(chams_weapon, 0.0);
	INITCVAR(chams_weapon_wall, 0.0);
	INITCVAR(chams_weapon_glow, 0.0);
	INITCVAR(chams_chicken, 0.0);
	INITCVAR(chams_chicken_wall, 0.0);
	INITCVAR(chams_chicken_glow, 0.0);

	INITCVAR(kz_strafe, 0.0);
	INITCVAR(kz_fast_run, 0.0);
	INITCVAR(kz_gstrafe, 0.0);
	INITCVAR(kz_bhop, 0.0);
	INITCVAR(kz_jump_bug, 0.0);
	INITCVAR(kz_jump_bug_auto, 0.0);
	INITCVAR(kz_show_kz, 0.0);
	INITCVAR(kz_strafe_key, -1.0);
	INITCVAR(kz_fastrun_key, -1.0);
	INITCVAR(kz_gstrafe_key, -1.0);
	INITCVAR(kz_bhop_key, -1.0);
	INITCVAR(kz_jumpbug_key, -1.0);

	INITCVAR(color_red, 0.5);
	INITCVAR(color_green, 0.5);
	INITCVAR(color_blue, 0.5);
	INITCVAR(color_random, 0.0);
	INITCVAR(color_speed, 0.0);

	INITCVAR(esp_name, 1.0);
	INITCVAR(esp_weapon, 0.0);
	INITCVAR(esp_model_name, 0.0);
	INITCVAR(esp_reload_bar, 0.0);
	INITCVAR(esp_box, 0.0);
	INITCVAR(esp_health, 0.0);
	INITCVAR(esp_alpha, 0.5);
	INITCVAR(esp_visual_team, 1.0);
	INITCVAR(esp_box_width, 1.0);
	INITCVAR(esp_box_height, 1.0);
	INITCVAR(esp_box_size_bot, 80.0);
	INITCVAR(esp_box_size_top, 80.0);
	INITCVAR(esp_sound, 1.0);
	INITCVAR(esp_idhook_only, 0.0);
	INITCVAR(esp_chase_cam, 0.0);
	INITCVAR(esp_headshot_mark, 0.0);
	INITCVAR(esp_kill_lightning, 0.0);
	INITCVAR(esp_rounding, 3.0);
	INITCVAR(esp_radar, 0.0);
	INITCVAR(esp_radar_size, 100.0); 
	INITCVAR(esp_grenade_trajectory, 0.0);
	INITCVAR(esp_crosshair, 0.0);
	INITCVAR(esp_crosshair_offset, 0.0);
	INITCVAR(esp_crosshair_spin, 0.0);
	INITCVAR(esp_vip, 0.0);
	INITCVAR(esp_custom_fov, 90.0);
	INITCVAR(esp_skins, 0.0);
	INITCVAR(esp_view_model_no_hands, 0.0);
	INITCVAR(esp_wall, 0.0);
	INITCVAR(esp_lambert, 0.0);

	INITCVAR(skeleton_player_body, 0.0);
	INITCVAR(skeleton_player_stick_figure_only, 0.0);
	INITCVAR(skeleton_player_stick_figure, 0.0);
	INITCVAR(skeleton_player_bonebox, 0.0);
	INITCVAR(skeleton_player_bonebox_num, 0.0);
	INITCVAR(skeleton_player_hitbox, 0.0);
	INITCVAR(skeleton_player_hitbox_num, 0.0);
	INITCVAR(skeleton_third_person_body, 0.0);
	INITCVAR(skeleton_third_person_stick_figure_only, 0.0);
	INITCVAR(skeleton_third_person_stick_figure, 0.0);
	INITCVAR(skeleton_third_person_bonebox, 0.0);
	INITCVAR(skeleton_third_person_bonebox_num, 0.0);
	INITCVAR(skeleton_third_person_hitbox, 0.0);
	INITCVAR(skeleton_third_person_hitbox_num, 0.0);
	INITCVAR(skeleton_models, 0.0);
	INITCVAR(skeleton_models_third_person, 0.0);
	INITCVAR(skeleton_models_chicken, 0.0);
	INITCVAR(skeleton_weapon_world_box, 0.0);
	INITCVAR(skeleton_chicken_box, 0.0);
	INITCVAR(skeleton_chicken_box_num, 0.0);
	INITCVAR(skeleton_chicken_bone, 0.0);
	INITCVAR(skeleton_chicken_bone_num, 0.0);
	INITCVAR(skeleton_face_player, 0.0);
	INITCVAR(skeleton_face_third_person, 0.0);

	INITCVAR(gui_key, 45.0);
	INITCVAR(gui_chat, 0.0);
	INITCVAR(gui_chat_key, -1.0);
	INITCVAR(gui_chat_key_team, -1.0);
	INITCVAR(gui_menu_button_spin, 0.0);
	INITCVAR(gui_menu_image, 0.0);
	INITCVAR(gui_menu_image_spin, 0.0);
	INITCVAR(gui_mouse_image, 0.0);
	INITCVAR(gui_mouse_image_spin, 0.0);
	INITCVAR(gui_mouse_image_amount, 0.0);

	INITCVAR(test1, 0.0);
	INITCVAR(test2, 0.0);
	INITCVAR(test3, 0.0);
	INITCVAR(test4, 0.0);
	INITCVAR(test5, 0.0);
	INITCVAR(test6, 0.0);
}