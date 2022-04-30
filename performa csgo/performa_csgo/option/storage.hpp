#pragma once

#include "../valve/sdk.hpp"

namespace option
{

struct aimbot_data_t
{
	// no category
	bool m_per_weapon = false;					// 

	// core
	int m_mode = 0;								// 0 - 2 | off, auto, key
	int m_key = 0;								// 0 - 128
	bool m_auto_fire = false;					// 
	bool m_auto_cycle = false;					// 
	int m_silent = 0;							// 0 - 2 | off, normal, perfect
	int m_hitchance = 0;						// 0 - 100 %

	// target
	int m_target_spot = 0;						// 0 - 19
	bool m_target_height_correction = false;	// 
	int m_target_hitscan = 0;					// 0 - 3 | off, low, medium, high
	float m_target_hitscan_scale = 1.f;			// 0.f - 1.f
	int m_target_adjustment = 0;				// 0 - 2 | normal, backtrack
	int m_target_resolver = 0;					// 0 - 2 | off, bruteforce, smart
	int m_target_priority = 0;					// 0 - 2 | fast, distance, fov
	float m_target_fov = 0.f;					// 0.f - 180.f
	bool m_target_friendly = false;				// 
	
	// step
	int m_step_mode = 0;						// 0 - 2 | off, constant, decreasing
	float m_step_pitch = 0.f;					// 0.f - 100.f
	float m_step_yaw = 0.f;						// 0.f - 100.f

	// time
	int m_time_delay = 0;						// 0 - 5000 ms
	int m_time_duration = 0;					// 0 - 5000 ms
	int m_time_cycle_delay = 0;					// 0 - 5000 ms

	// rcs
	int m_rcs_mode = 0;							// off, target, standalone
	int m_rcs_delay = 0;						// 0 - 10 bullet
	int m_rcs_pitch = 0;						// 0 - 100 %
	int m_rcs_yaw = 0;							// 0 - 100 %

	// advanced
	bool m_auto_wall = false;					// 
	int m_min_damage = 0;						// 0 - 100

	static std::vector< std::string > m_mode_array;
	static std::vector< std::string > m_silent_array;
	static std::vector< std::string > m_target_spot_array;
	static std::vector< std::string > m_target_hitscan_array;
	static std::vector< std::string > m_target_adjustment_array;
	static std::vector< std::string > m_target_resolve_array;
	static std::vector< std::string > m_target_priority_array;
	static std::vector< std::string > m_step_mode_array;
	static std::vector< std::string > m_rcs_mode_array;
};

struct triggerbot_data_t
{
	// no category
	bool m_per_weapon = false;		// 

	// core
	int m_mode = 0;					// 0 - 2 | off, auto, key
	int m_key = 0;					// 0 - 128
	int m_hitchance = 0;			// 0 - 100 %

	// fire
	int m_fire_delay = 0;			// 0 - 5000 ms
	int m_fire_burst = 0;			// 0 - 10 bullet

	// target
	bool m_target_friendly = false;	// 
	bool m_target_head = false;		// 
	bool m_target_chest = false;	// 
	bool m_target_stomach = false;	// 
	bool m_target_arms = false;		// 
	bool m_target_legs = false;		// 

	// penetration
	bool m_auto_wall = false;		// 
	int m_min_damage = 0;			// 0 - 100

	static std::vector< std::string > m_mode_array;
};

struct weapon_data_t
{
	aimbot_data_t m_aimbot = { };
	triggerbot_data_t m_triggerbot = { };
};

struct esp_data_t
{
	// core
	int m_box_mode = 0;				// 0 - 2 | off, normal, corners
	bool m_chams = false;			// 
	bool m_outlined = false;		// 

	// target
	bool m_target_friendly = false;	// 
	bool m_target_bomb = false;		// 

	// information
	bool m_name = false;			// 
	bool m_weapon = false;			// 
	int m_health = 0;				// 0 - 2 | off, text, bar
	int m_armor = 0;				// 0 - 2 | off, text, bar
	bool m_skeleton = false;		// 
	bool m_defusing = false;		// 

	static std::vector< std::string > m_box_mode_array;
	static std::vector< std::string > m_style_array;
};

struct removal_data_t
{
	// core
	bool m_spread = false;			// 
	bool m_recoil = false;			// 
	
	// visual
	bool m_visual_recoil = false;	// 
	bool m_visual_smoke = false;	// 
	int m_visual_flash = 100;		// 0 - 100 %
};

struct misc_data_t
{
	// core
	bool m_auto_jump = false;					// 
	bool m_auto_pistol = false;					// 
	bool m_bomb_warning = false;				// 
	bool m_thirdperson = false;					// 
	bool m_ghost_fake = false;					// 
	bool m_ghost_lower_body = false;			// 
	int m_restriction = 0;						// 0 - 2 | off, matchmaking, faceit

	// strafe
	int m_strafe_mode = 0;						// 0 - 2 | off, normal, boost
	bool m_strafe_ground = false;				// 

	// antiaim
	bool m_antiaim_target = false;				// 
	bool m_antiaim_break_lower_body = false;	// 
	int m_antiaim_packet = 1;					// 0 - 15
	
	int m_antiaim_pitch = 0;					// 0 - 4 | off, zero, up, down, fake up
	float m_antiaim_pitch_mod = 0.f;			// -180.f - 180.f
	
	int m_antiaim_yaw_real = 0;					// 0 - 5 | off, zero, left, right, forward, backward
	float m_antiaim_yaw_real_mod = 0.f;			// -180.f - 180.f

	int m_antiaim_yaw_fake = 0;					// 0 - 5 | off, zero, left, right, forward, backward
	float m_antiaim_yaw_fake_mod = 0.f;			// -180.f - 180.f

	// crosshair
	int m_crosshair_mode = 0;					// 0 - 3 | off, dot, cross
	bool m_crosshair_outlined = false;			// 
	bool m_crosshair_recoil = false;			// 

	static std::vector< std::string > m_restriction_array;
	static std::vector< std::string > m_strafe_mode_array;
	static std::vector< std::string > m_antiaim_pitch_array;
	static std::vector< std::string > m_antiaim_yaw_array;
	static std::vector< std::string > m_crosshair_mode_array;
};


extern weapon_data_t m_current;
extern weapon_data_t m_weapon[ ITEM_MAX ];

extern aimbot_data_t m_aimbot;
extern triggerbot_data_t m_triggerbot;
extern esp_data_t m_esp;
extern removal_data_t m_removal;
extern misc_data_t m_misc;

auto Create() -> bool;
auto Destroy() -> void;

auto Load( const std::string& name ) -> void;
auto Save( const std::string& name ) -> void;
auto Delete( const std::string& name ) -> void;

auto CreateMove() -> void;

auto GetWeaponNameArray() -> const std::vector< std::string >&;
auto GetWeaponIndex( const std::string& name ) -> int;

}