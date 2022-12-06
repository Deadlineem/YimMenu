#pragma once

#include "common.hpp"

#include "services/players/player_service.hpp"

namespace big
{
	class looped {
	public:
		static void hud_transition_state();

		static void tunables_disable_phone();

		static void player_good_options();
		static void player_spectate();
		static void player_remote_control_vehicle();

		static void self_clean_player();
		static void self_free_cam_disable_control_action();
		static void self_free_cam();
		static void self_godmode();
		static void self_invisibility();
		static void self_noclip_disable_control_action();
		static void self_noclip();
		static void self_no_ragdoll();
		static void self_off_radar();
		static void self_police();
		static void self_super_run();
		static void self_no_collision();
		static void self_hud();
		static void self_unlimited_oxygen();
		static void self_no_water_collision();
		static void self_mobile_radio();
		static void self_dance_mode();
		static void self_fast_respawn();

		static void session_local_time();
		static void session_pop_multiplier_areas();
		static void session_force_thunder();
		static void session_randomize_ceo_colors();
		static void session_auto_kick_host();

		static void system_disable_sigscanner();
		static void system_self_globals();
		static void system_update_pointers();
		static void system_desync_kick_protection();
		static void system_force_session_host();
		static void system_mission_creator();
		static void system_auto_tp();

		static void vehicle_auto_drive();
		static void vehicle_boost_behavior();
		static void vehicle_drive_on_water();
		static void vehicle_fly();
		static void vehicle_god_mode();
		static void vehicle_horn_boost();
		static void vehicle_jump();
		static void vehicle_instant_brake();
		static void vehicle_is_targetable();
		static void vehicle_ls_customs();
		static void vehicle_rainbow_paint();
		static void vehicle_seatbelt();
		static void vehicle_speedo_meter();
		static void vehicle_turn_signals();
		static void vehicle_keep_vehicle_repaired();
		static void vehicle_no_water_collision();

		static void weapons_ammo_special_type();
		static void weapons_cage_gun();
		static void custom_gun_disable_control_action();
		static void weapons_delete_gun();
		static void weapons_force_crosshairs();
		static void weapons_gravity_gun();
		static void weapons_increased_damage();
		static void weapons_infinite_ammo();
		static void weapons_infinite_mag();
		static void weapons_no_recoil();
		static void weapons_no_spread();
		static void weapons_repair_gun();
		static void weapons_steal_vehicle_gun();
		static void weapons_vehicle_gun();
		static void weapons_rapid_fire();
	};
}