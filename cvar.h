#ifndef _CVARS_
#define _CVARS_

class CVARlist
{
public:
	void init();

	float rage_active;
	float rage_penetration;
	float rage_team_attack;
	float rage_bone;
	float rage_multipoint_bone;
	float rage_target_selection;
	float rage_silent;
	float rage_knife_attack;
	float rage_shield_attack;
	float rage_nospread_method;
	float rage_auto_fire;

	struct legit_weapons
	{
		float active;
		float speed;
		float speed_scale_fov;
		float reaction_time;
		float humanize;
		float recoil_compensation_pitch;
		float recoil_compensation_yaw;
		float recoil_compensation_after_shots_fired;
		float block_attack_after_kill;
		float accuracy;
		float silent;
		float fov;
		float speed_in_attack;
		float stomach_bone;
		float chest_bone;
		float head_bone;
		float all_bone;

		float trigger_active;
		float trigger_penetration;
		float trigger_accuracy;
		float trigger_bone_head;
		float trigger_bone_chest;
		float trigger_bone_stomach;
		float trigger_bone_all;
		float trigger_delay_shot;
		float trigger_shot_count;
		float trigger_shot_type;
		float trigger_random_max;
	} legit[31];

	struct rage_weapons
	{
		float recoil;
		float rage_delay_shot;
		float rage_shot_count;
		float rage_shot_type;
		float rage_random_max;
	} rage[31];

	float legit_team_attack;
	float legit_trigger_only_zoom;
	float legit_fast_zoom;

	float menu_legit_global_section;
	float menu_legit_sub_section;
	float menu_rage_global_section;
	float menu_rage_sub_section;

	float knifebot_active;
	float knifebot_attack;
	float knifebot_attack_distance;
	float knifebot_attack2_distance;
	float knifebot_silent;

	float bypass_trace_blockers;

	float aa_pitch;
	float aa_edge;
	float aa_yaw;
	float aa_yaw_static;
	float aa_yaw_while_running;

	float aim_id_mode;

	float wav_speed;

	float misc_snapshot;
	float misc_snapshot_time;
	float misc_quick_change;
	float misc_quick_change_key;
	float misc_weapon_local;

	float chams_view_model;
	float chams_view_model_glow;
	float chams_player;
	float chams_player_glow;
	float chams_player_wall;
	float chams_third_person;
	float chams_third_person_glow;
	float chams_third_person_wall;
	float chams_weapon;
	float chams_weapon_wall;
	float chams_weapon_glow;
	float chams_chicken;
	float chams_chicken_wall;
	float chams_chicken_glow;

	float kz_strafe;
	float kz_fast_run;
	float kz_gstrafe;
	float kz_bhop;
	float kz_jump_bug;
	float kz_jump_bug_auto;
	float kz_show_kz;
	float kz_strafe_key;
	float kz_fastrun_key;
	float kz_gstrafe_key;
	float kz_bhop_key;
	float kz_jumpbug_key;

	float color_red;
	float color_green;
	float color_blue;
	float color_random;
	float color_speed;

	float esp_name;
	float esp_weapon;
	float esp_model_name;
	float esp_reload_bar;
	float esp_box;
	float esp_health;
	float esp_alpha;
	float esp_visual_team;
	float esp_box_width;
	float esp_box_height;
	float esp_box_size_bot;
	float esp_box_size_top;
	float esp_sound;
	float esp_idhook_only;
	float esp_chase_cam;
	float esp_headshot_mark;
	float esp_kill_lightning;
	float esp_rounding;
	float esp_radar;
	float esp_radar_size;
	float esp_grenade_trajectory;
	float esp_crosshair;
	float esp_crosshair_offset;
	float esp_crosshair_spin;
	float esp_vip;
	float esp_custom_fov;
	float esp_skins;
	float esp_view_model_no_hands;
	float esp_wall;
	float esp_lambert;

	float skeleton_player_body;
	float skeleton_player_stick_figure_only;
	float skeleton_player_stick_figure;
	float skeleton_player_bonebox;
	float skeleton_player_bonebox_num;
	float skeleton_player_hitbox;
	float skeleton_player_hitbox_num;
	float skeleton_third_person_body;
	float skeleton_third_person_stick_figure_only;
	float skeleton_third_person_stick_figure;
	float skeleton_third_person_bonebox;
	float skeleton_third_person_bonebox_num;
	float skeleton_third_person_hitbox;
	float skeleton_third_person_hitbox_num;
	float skeleton_models;
	float skeleton_models_third_person;
	float skeleton_models_chicken;
	float skeleton_weapon_world_box;
	float skeleton_chicken_box;
	float skeleton_chicken_box_num;
	float skeleton_chicken_bone;
	float skeleton_chicken_bone_num;
	float skeleton_face_player;
	float skeleton_face_third_person;

	float gui_key;
	float gui_chat;
	float gui_chat_key;
	float gui_chat_key_team;
	float gui_menu_button_spin;
	float gui_menu_image;
	float gui_menu_image_spin;
	float gui_mouse_image;
	float gui_mouse_image_spin;
	float gui_mouse_image_amount;

	float test1;
	float test2;
	float test3;
	float test4;
	float test5;
	float test6;
};

extern CVARlist cvar;

#endif