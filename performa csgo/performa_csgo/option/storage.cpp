#include "storage.hpp"

#include "../valve/source/player.hpp"
#include "../valve/source/weapon.hpp"

using namespace std::string_literals;

namespace option
{

weapon_data_t m_current = { };
weapon_data_t m_weapon[ ITEM_MAX ] = { };

aimbot_data_t m_aimbot = { };
triggerbot_data_t m_triggerbot = { };
esp_data_t m_esp = { };
removal_data_t m_removal = { };
misc_data_t m_misc = { };

std::string m_directory = { };
std::string m_directory_current = { };


auto Create() -> bool
{
	auto& shared = core::Shared::Instance();

	m_directory = core::m_data.m_loader_directory;
	m_directory.append( "\\csgo\\" );

	shared.SetExecuteDirectory( "auto" );
	shared.SetExecuteState( core::ExecuteLoad );

	return true;
}

auto Destroy() -> void
{

}

auto get_bool( const std::string& app_name, const std::string& key_name ) -> bool
{
	char key_data[ MAX_PATH ] = { };
	GetPrivateProfileStringA( app_name.c_str(), key_name.c_str(), "false", key_data, MAX_PATH, m_directory_current.c_str() );

	auto data = std::string( key_data );

	return ( !data.compare( "true" ) ||
			 !data.compare( "on" ) ||
			 !data.compare( "1" ) );
}

auto get_int( const std::string& app_name, const std::string& key_name ) -> int
{
	char key_data[ MAX_PATH ] = { };
	GetPrivateProfileStringA( app_name.c_str(), key_name.c_str(), "0", key_data, MAX_PATH, m_directory_current.c_str() );

	auto data = std::string( key_data );
	
	return ( data.empty() ? 0 : std::stoi( data ) );
}

auto get_float( const std::string& app_name, const std::string& key_name ) -> float
{
	char key_data[ MAX_PATH ] = { };
	GetPrivateProfileStringA( app_name.c_str(), key_name.c_str(), "0.0000", key_data, MAX_PATH, m_directory_current.c_str() );

	auto data = std::string( key_data );

	return ( data.empty() ? 0.f : std::stof( data ) );
}

auto set_bool( const std::string& app_name, const std::string& key_name, bool value ) -> void
{
	char key_data[ MAX_PATH ] = { };
	sprintf_s( key_data, "%s", value ? "true" : "false" );

	WritePrivateProfileStringA( app_name.c_str(), key_name.c_str(), key_data, m_directory_current.c_str() );
}

auto set_int( const std::string& app_name, const std::string& key_name, int value ) -> void
{
	char key_data[ MAX_PATH ] = { };
	sprintf_s( key_data, "%i", value );

	WritePrivateProfileStringA( app_name.c_str(), key_name.c_str(), key_data, m_directory_current.c_str() );
}

auto set_float( const std::string& app_name, const std::string& key_name, float value ) -> void
{
	char key_data[ MAX_PATH ] = { };
	sprintf_s( key_data, "%f", value );

	WritePrivateProfileStringA( app_name.c_str(), key_name.c_str(), key_data, m_directory_current.c_str() );
}

auto is_good( const std::string& name )
{
	std::ifstream stream( name );
	return stream.good();
}

auto create( const std::string& name )
{
	auto directory = name.substr( 0u, name.find_last_of( '\\' ) );

	if( CreateDirectoryA( directory.c_str(), nullptr ) || GetLastError() == ERROR_ALREADY_EXISTS )
	{
		std::ofstream stream( name );
		return stream.good();
	}

	return false;
}

auto Load( const std::string& name ) -> void
{
	auto& shared = core::Shared::Instance();

	auto current = name;

	if( current.find( ".ini" ) == std::string::npos )
		current.append( ".ini" );

	m_directory_current = m_directory + current;

	if( !is_good( m_directory_current ) )
		Save( name );

	auto app_name = "main"s;

	m_aimbot.m_per_weapon = get_bool( app_name, "aimbot.per.weapon" );
	m_aimbot.m_mode = get_int( app_name, "aimbot.mode" );
	m_aimbot.m_key = get_int( app_name, "aimbot.key" );
	m_aimbot.m_auto_fire = get_bool( app_name, "aimbot.auto.fire" );
	m_aimbot.m_auto_cycle = get_bool( app_name, "aimbot.auto.cycle" );
	m_aimbot.m_silent = get_int( app_name, "aimbot.silent" );
	m_aimbot.m_hitchance = get_int( app_name, "aimbot.hitchance" );
	m_aimbot.m_target_spot = get_int( app_name, "aimbot.target.spot" );
	m_aimbot.m_target_height_correction = get_bool( app_name, "aimbot.target.height.correction" );
	m_aimbot.m_target_adjustment = get_int( app_name, "aimbot.target.adjustment" );
	m_aimbot.m_target_resolver = get_int( app_name, "aimbot.target.resolver" );
	m_aimbot.m_target_priority = get_int( app_name, "aimbot.target.priority" );
	m_aimbot.m_target_fov = get_float( app_name, "aimbot.target.fov" );
	m_aimbot.m_target_friendly = get_bool( app_name, "aimbot.target.friendly" );
	m_aimbot.m_step_mode = get_int( app_name, "aimbot.step.mode" );
	m_aimbot.m_step_pitch = get_float( app_name, "aimbot.step.pitch" );
	m_aimbot.m_step_yaw = get_float( app_name, "aimbot.step.yaw" );
	m_aimbot.m_time_delay = get_int( app_name, "aimbot.time.delay" );
	m_aimbot.m_time_duration = get_int( app_name, "aimbot.time.duration" );
	m_aimbot.m_time_cycle_delay = get_int( app_name, "aimbot.time.cycle.delay" );
	m_aimbot.m_rcs_mode = get_int( app_name, "aimbot.rcs.mode" );
	m_aimbot.m_rcs_delay = get_int( app_name, "aimbot.rcs.delay" );
	m_aimbot.m_rcs_pitch = get_int( app_name, "aimbot.rcs.pitch" );
	m_aimbot.m_rcs_yaw = get_int( app_name, "aimbot.rcs.yaw" );
	m_aimbot.m_auto_wall = get_bool( app_name, "aimbot.auto.wall" );
	m_aimbot.m_min_damage = get_int( app_name, "aimbot.min.damage" );
	// m_aimbot.m_spot_scan = get_int( app_name, "aimbot.spot.scan" );
	// m_aimbot.m_spot_scale = get_int( app_name, "aimbot.spot.scale" );

	m_triggerbot.m_per_weapon = get_bool( app_name, "triggerbot.per.weapon" );
	m_triggerbot.m_mode = get_int( app_name, "triggerbot.mode" );
	m_triggerbot.m_key = get_int( app_name, "triggerbot.key" );
	m_triggerbot.m_hitchance = get_int( app_name, "triggerbot.auto.hitchance" );
	m_triggerbot.m_fire_delay = get_int( app_name, "triggerbot.fire.delay" );
	m_triggerbot.m_fire_burst = get_int( app_name, "triggerbot.fire.burst" );
	m_triggerbot.m_target_friendly = get_bool( app_name, "triggerbot.target.friendly" );
	m_triggerbot.m_target_head = get_bool( app_name, "triggerbot.target.head" );
	m_triggerbot.m_target_chest = get_bool( app_name, "triggerbot.target.chest" );
	m_triggerbot.m_target_stomach = get_bool( app_name, "triggerbot.target.stomach" );
	m_triggerbot.m_target_arms = get_bool( app_name, "triggerbot.target.arms" );
	m_triggerbot.m_target_legs = get_bool( app_name, "triggerbot.target.legs" );
	m_triggerbot.m_auto_wall = get_bool( app_name, "triggerbot.auto.wall" );
	m_triggerbot.m_min_damage = get_int( app_name, "triggerbot.min.damage" );

	app_name = "esp"s;

	m_esp.m_box_mode = get_int( app_name, "box.mode" );
	m_esp.m_chams = get_bool( app_name, "chams" );
	m_esp.m_outlined = get_bool( app_name, "outlined" );
	m_esp.m_target_friendly = get_bool( app_name, "target.friendly" );
	m_esp.m_target_bomb = get_bool( app_name, "target.bomb" );
	m_esp.m_name = get_bool( app_name, "name" );
	m_esp.m_weapon = get_bool( app_name, "weapon" );
	m_esp.m_health = get_int( app_name, "health" );
	m_esp.m_armor = get_int( app_name, "armor" );
	m_esp.m_skeleton = get_bool( app_name, "skeleton" );
	m_esp.m_defusing = get_bool( app_name, "defusing" );

	app_name = "removal"s;

	m_removal.m_spread = get_bool( app_name, "spread" );
	m_removal.m_recoil = get_bool( app_name, "recoil" );
	m_removal.m_visual_recoil = get_bool( app_name, "visual.recoil" );
	m_removal.m_visual_smoke = get_bool( app_name, "visual.smoke" );
	m_removal.m_visual_flash = get_int( app_name, "visual.flash" );

	app_name = "misc"s;

	m_misc.m_auto_jump = get_bool( app_name, "auto.jump" );
	m_misc.m_auto_pistol = get_bool( app_name, "auto.pistol" );
	m_misc.m_bomb_warning = get_bool( app_name, "bomb.warning" );
	m_misc.m_restriction = get_int( app_name, "restriction" );
	m_misc.m_strafe_mode = get_int( app_name, "strafe.mode" );
	m_misc.m_strafe_ground = get_bool( app_name, "strafe.ground" );
	m_misc.m_crosshair_mode = get_int( app_name, "crosshair.mode" );
	m_misc.m_crosshair_outlined = get_bool( app_name, "crosshair.outlined" );
	m_misc.m_crosshair_recoil = get_bool( app_name, "crosshair.recoil" );


	auto weapon_array = GetWeaponNameArray();

	for( const auto& weapon : weapon_array )
	{
		app_name = weapon;

		m_aimbot.m_per_weapon = get_bool( app_name, "aimbot.per.weapon" );
		m_aimbot.m_mode = get_int( app_name, "aimbot.mode" );
		m_aimbot.m_key = get_int( app_name, "aimbot.key" );
		m_aimbot.m_auto_fire = get_bool( app_name, "aimbot.auto.fire" );
		m_aimbot.m_auto_cycle = get_bool( app_name, "aimbot.auto.cycle" );
		m_aimbot.m_silent = get_int( app_name, "aimbot.silent" );
		m_aimbot.m_hitchance = get_int( app_name, "aimbot.hitchance" );
		m_aimbot.m_target_spot = get_int( app_name, "aimbot.target.spot" );
		m_aimbot.m_target_height_correction = get_bool( app_name, "aimbot.target.height.correction" );
		m_aimbot.m_target_adjustment = get_int( app_name, "aimbot.target.adjustment" );
		m_aimbot.m_target_resolver = get_int( app_name, "aimbot.target.resolver" );
		m_aimbot.m_target_priority = get_int( app_name, "aimbot.target.priority" );
		m_aimbot.m_target_fov = get_float( app_name, "aimbot.target.fov" );
		m_aimbot.m_target_friendly = get_bool( app_name, "aimbot.target.friendly" );
		m_aimbot.m_step_mode = get_int( app_name, "aimbot.step.mode" );
		m_aimbot.m_step_pitch = get_float( app_name, "aimbot.step.pitch" );
		m_aimbot.m_step_yaw = get_float( app_name, "aimbot.step.yaw" );
		m_aimbot.m_time_delay = get_int( app_name, "aimbot.time.delay" );
		m_aimbot.m_time_duration = get_int( app_name, "aimbot.time.duration" );
		m_aimbot.m_time_cycle_delay = get_int( app_name, "aimbot.time.cycle.delay" );
		m_aimbot.m_rcs_mode = get_int( app_name, "aimbot.rcs.mode" );
		m_aimbot.m_rcs_delay = get_int( app_name, "aimbot.rcs.delay" );
		m_aimbot.m_rcs_pitch = get_int( app_name, "aimbot.rcs.pitch" );
		m_aimbot.m_rcs_yaw = get_int( app_name, "aimbot.rcs.yaw" );
		m_aimbot.m_auto_wall = get_bool( app_name, "aimbot.auto.wall" );
		m_aimbot.m_min_damage = get_int( app_name, "aimbot.min.damage" );
		// m_aimbot.m_spot_scan = get_int( app_name, "aimbot.spot.scan" );
		// m_aimbot.m_spot_scale = get_int( app_name, "aimbot.spot.scale" );

		m_triggerbot.m_per_weapon = get_bool( app_name, "triggerbot.per.weapon" );
		m_triggerbot.m_mode = get_int( app_name, "triggerbot.mode" );
		m_triggerbot.m_key = get_int( app_name, "triggerbot.key" );
		m_triggerbot.m_hitchance = get_int( app_name, "triggerbot.auto.hitchance" );
		m_triggerbot.m_fire_delay = get_int( app_name, "triggerbot.fire.delay" );
		m_triggerbot.m_fire_burst = get_int( app_name, "triggerbot.fire.burst" );
		m_triggerbot.m_target_friendly = get_bool( app_name, "triggerbot.target.friendly" );
		m_triggerbot.m_target_head = get_bool( app_name, "triggerbot.target.head" );
		m_triggerbot.m_target_chest = get_bool( app_name, "triggerbot.target.chest" );
		m_triggerbot.m_target_stomach = get_bool( app_name, "triggerbot.target.stomach" );
		m_triggerbot.m_target_arms = get_bool( app_name, "triggerbot.target.arms" );
		m_triggerbot.m_target_legs = get_bool( app_name, "triggerbot.target.legs" );
		m_triggerbot.m_auto_wall = get_bool( app_name, "triggerbot.auto.wall" );
		m_triggerbot.m_min_damage = get_int( app_name, "triggerbot.min.damage" );
	}

	shared.SetExecuteState( core::ExecuteNone );
}

auto Save( const std::string& name ) -> void
{
	auto& shared = core::Shared::Instance();

	auto current = name;

	if( current.find( ".ini" ) == std::string::npos )
		current.append( ".ini" );

	m_directory_current = m_directory + current;

	if( !is_good( m_directory_current ) )
		create( m_directory_current );

	auto app_name = "main"s;

	set_bool( app_name, "aimbot.per.weapon", m_aimbot.m_per_weapon );
	set_int( app_name, "aimbot.mode", m_aimbot.m_mode );
	set_int( app_name, "aimbot.key", m_aimbot.m_key );
	set_bool( app_name, "aimbot.auto.fire", m_aimbot.m_auto_fire );
	set_bool( app_name, "aimbot.auto.cycle", m_aimbot.m_auto_cycle );
	set_int( app_name, "aimbot.silent", m_aimbot.m_silent );
	set_int( app_name, "aimbot.hitchance", m_aimbot.m_hitchance );
	set_int( app_name, "aimbot.target.spot", m_aimbot.m_target_spot );
	set_bool( app_name, "aimbot.target.height.correction", m_aimbot.m_target_spot );
	set_int( app_name, "aimbot.target.adjustment", m_aimbot.m_target_adjustment );
	set_int( app_name, "aimbot.target.resolver", m_aimbot.m_target_resolver );
	set_int( app_name, "aimbot.target.priority", m_aimbot.m_target_priority );
	set_float( app_name, "aimbot.target.fov", m_aimbot.m_target_fov );
	set_bool( app_name, "aimbot.target.friendly", m_aimbot.m_target_friendly );
	set_int( app_name, "aimbot.step.mode", m_aimbot.m_step_mode );
	set_float( app_name, "aimbot.step.pitch", m_aimbot.m_step_pitch );
	set_float( app_name, "aimbot.step.yaw", m_aimbot.m_step_yaw );
	set_int( app_name, "aimbot.time.delay", m_aimbot.m_time_delay );
	set_int( app_name, "aimbot.time.duration", m_aimbot.m_time_duration );
	set_int( app_name, "aimbot.time.cycle.delay", m_aimbot.m_time_cycle_delay );
	set_int( app_name, "aimbot.rcs.mode", m_aimbot.m_rcs_mode );
	set_int( app_name, "aimbot.rcs.delay", m_aimbot.m_rcs_delay );
	set_int( app_name, "aimbot.rcs.pitch", m_aimbot.m_rcs_pitch );
	set_int( app_name, "aimbot.rcs.yaw", m_aimbot.m_rcs_yaw );
	set_bool( app_name, "aimbot.auto.wall", m_aimbot.m_auto_wall );
	set_int( app_name, "aimbot.min.damage", m_aimbot.m_min_damage );
	// set_int( app_name, "aimbot.spot.scan", m_aimbot.m_spot_scan );
	// set_int( app_name, "aimbot.spot.scale", m_aimbot.m_spot_scale );

	set_bool( app_name, "triggerbot.per.weapon", m_triggerbot.m_per_weapon );
	set_int( app_name, "triggerbot.mode", m_triggerbot.m_mode );
	set_int( app_name, "triggerbot.key", m_triggerbot.m_key );
	set_int( app_name, "triggerbot.auto.hitchance", m_triggerbot.m_hitchance );
	set_int( app_name, "triggerbot.fire.delay", m_triggerbot.m_fire_delay );
	set_int( app_name, "triggerbot.fire.burst", m_triggerbot.m_fire_burst );
	set_bool( app_name, "triggerbot.target.friendly", m_triggerbot.m_target_friendly );
	set_bool( app_name, "triggerbot.target.head", m_triggerbot.m_target_head );
	set_bool( app_name, "triggerbot.target.chest", m_triggerbot.m_target_chest );
	set_bool( app_name, "triggerbot.target.stomach", m_triggerbot.m_target_stomach );
	set_bool( app_name, "triggerbot.target.arms", m_triggerbot.m_target_arms );
	set_bool( app_name, "triggerbot.target.legs", m_triggerbot.m_target_legs );
	set_bool( app_name, "triggerbot.auto.wall", m_triggerbot.m_auto_wall );
	set_int( app_name, "triggerbot.min.damage", m_triggerbot.m_min_damage );

	app_name = "esp"s;

	set_int( app_name, "box.mode", m_esp.m_box_mode );
	set_bool( app_name, "chams", m_esp.m_chams );
	set_bool( app_name, "outlined", m_esp.m_outlined );
	set_bool( app_name, "target.friendly", m_esp.m_target_friendly );
	set_bool( app_name, "target.bomb", m_esp.m_target_bomb );
	set_bool( app_name, "name", m_esp.m_name );
	set_bool( app_name, "weapon", m_esp.m_weapon );
	set_int( app_name, "health", m_esp.m_health );
	set_int( app_name, "armor", m_esp.m_armor );
	set_bool( app_name, "skeleton", m_esp.m_skeleton );
	set_bool( app_name, "defusing", m_esp.m_defusing );

	app_name = "removal"s;

	set_bool( app_name, "spread", m_removal.m_spread );
	set_bool( app_name, "recoil", m_removal.m_recoil );
	set_bool( app_name, "visual.recoil", m_removal.m_visual_recoil );
	set_bool( app_name, "visual.smoke", m_removal.m_visual_smoke );
	set_int( app_name, "visual.flash", m_removal.m_visual_flash );

	app_name = "misc"s;

	set_bool( app_name, "auto.jump", m_misc.m_auto_jump );
	set_bool( app_name, "auto.pistol", m_misc.m_auto_pistol );
	set_bool( app_name, "bomb.warning", m_misc.m_bomb_warning );
	set_int( app_name, "restriction", m_misc.m_restriction );
	set_int( app_name, "strafe.mode", m_misc.m_strafe_mode );
	set_bool( app_name, "strafe.ground", m_misc.m_strafe_ground );
	set_int( app_name, "crosshair.mode", m_misc.m_crosshair_mode );
	set_bool( app_name, "crosshair.outlined", m_misc.m_crosshair_outlined );
	set_bool( app_name, "crosshair.recoil", m_misc.m_crosshair_recoil );


	auto weapon_array = GetWeaponNameArray();
	
	for( const auto& weapon : weapon_array )
	{
		app_name = weapon;

		set_bool( app_name, "aimbot.per.weapon", m_aimbot.m_per_weapon );
		set_int( app_name, "aimbot.mode", m_aimbot.m_mode );
		set_int( app_name, "aimbot.key", m_aimbot.m_key );
		set_bool( app_name, "aimbot.auto.fire", m_aimbot.m_auto_fire );
		set_bool( app_name, "aimbot.auto.cycle", m_aimbot.m_auto_cycle );
		set_int( app_name, "aimbot.silent", m_aimbot.m_silent );
		set_int( app_name, "aimbot.hitchance", m_aimbot.m_hitchance );
		set_int( app_name, "aimbot.target.spot", m_aimbot.m_target_spot );
		set_bool( app_name, "aimbot.target.height.correction", m_aimbot.m_target_spot );
		set_int( app_name, "aimbot.target.adjustment", m_aimbot.m_target_adjustment );
		set_int( app_name, "aimbot.target.resolver", m_aimbot.m_target_resolver );
		set_int( app_name, "aimbot.target.priority", m_aimbot.m_target_priority );
		set_float( app_name, "aimbot.target.fov", m_aimbot.m_target_fov );
		set_bool( app_name, "aimbot.target.friendly", m_aimbot.m_target_friendly );
		set_int( app_name, "aimbot.step.mode", m_aimbot.m_step_mode );
		set_float( app_name, "aimbot.step.pitch", m_aimbot.m_step_pitch );
		set_float( app_name, "aimbot.step.yaw", m_aimbot.m_step_yaw );
		set_int( app_name, "aimbot.time.delay", m_aimbot.m_time_delay );
		set_int( app_name, "aimbot.time.duration", m_aimbot.m_time_duration );
		set_int( app_name, "aimbot.time.cycle.delay", m_aimbot.m_time_cycle_delay );
		set_int( app_name, "aimbot.rcs.mode", m_aimbot.m_rcs_mode );
		set_int( app_name, "aimbot.rcs.delay", m_aimbot.m_rcs_delay );
		set_int( app_name, "aimbot.rcs.pitch", m_aimbot.m_rcs_pitch );
		set_int( app_name, "aimbot.rcs.yaw", m_aimbot.m_rcs_yaw );
		set_bool( app_name, "aimbot.auto.wall", m_aimbot.m_auto_wall );
		set_int( app_name, "aimbot.min.damage", m_aimbot.m_min_damage );
		// set_int( app_name, "aimbot.spot.scan", m_aimbot.m_spot_scan );
		// set_int( app_name, "aimbot.spot.scale", m_aimbot.m_spot_scale );

		set_bool( app_name, "triggerbot.per.weapon", m_triggerbot.m_per_weapon );
		set_int( app_name, "triggerbot.mode", m_triggerbot.m_mode );
		set_int( app_name, "triggerbot.key", m_triggerbot.m_key );
		set_int( app_name, "triggerbot.auto.hitchance", m_triggerbot.m_hitchance );
		set_int( app_name, "triggerbot.fire.delay", m_triggerbot.m_fire_delay );
		set_int( app_name, "triggerbot.fire.burst", m_triggerbot.m_fire_burst );
		set_bool( app_name, "triggerbot.target.friendly", m_triggerbot.m_target_friendly );
		set_bool( app_name, "triggerbot.target.head", m_triggerbot.m_target_head );
		set_bool( app_name, "triggerbot.target.chest", m_triggerbot.m_target_chest );
		set_bool( app_name, "triggerbot.target.stomach", m_triggerbot.m_target_stomach );
		set_bool( app_name, "triggerbot.target.arms", m_triggerbot.m_target_arms );
		set_bool( app_name, "triggerbot.target.legs", m_triggerbot.m_target_legs );
		set_bool( app_name, "triggerbot.auto.wall", m_triggerbot.m_auto_wall );
		set_int( app_name, "triggerbot.min.damage", m_triggerbot.m_min_damage );
	}

	shared.SetExecuteState( core::ExecuteNone );
}

auto Delete( const std::string& name ) -> void
{
	auto current = name;

	if( current.find( ".ini" ) == std::string::npos )
		current.append( ".ini" );

	m_directory_current = m_directory + current;

	DeleteFileA( m_directory_current.c_str() );
}

auto CreateMove() -> void
{
	auto player = C_CSPlayer::GetLocalCSPlayer();

	if( !player )
		return;

	auto weapon = player->GetActiveWeapon();

	if( !weapon )
		return;

	auto index = weapon->m_iItemDefinitionIndex();

	if( m_aimbot.m_per_weapon )
		std::memcpy( &m_current.m_aimbot, &m_weapon[ index ].m_aimbot, sizeof( aimbot_data_t ) );
	else
		std::memcpy( &m_current.m_aimbot, &m_aimbot, sizeof( aimbot_data_t ) );

	if( m_triggerbot.m_per_weapon )
		std::memcpy( &m_current.m_triggerbot, &m_weapon[ index ].m_triggerbot, sizeof( triggerbot_data_t ) );
	else
		std::memcpy( &m_current.m_triggerbot, &m_triggerbot, sizeof( triggerbot_data_t ) );
}

static std::vector< std::string > m_weapon_name_array =
{
	"desert eagle",
	"dual berettas",
	"five-seven",
	"glock-18",
	"ak-47",
	"aug",
	"awp",
	"famas",
	"g3sg1",
	"galil ar",
	"m249",
	"m4a4",
	"mac-10",
	"p90",
	"ump-45",
	"xm1014",
	"pp-bizon",
	"nag-7",
	"negev",
	"sawed-off",
	"tec-9",
	"zeus x27",
	"p2000",
	"mp7",
	"mp9",
	"nova",
	"p250",
	"scar-20",
	"sg 556",
	"ssg 08",
	"m4a1-s",
	"usp-s",
	"cz75 auto",
	"r8 revolver"
};

auto GetWeaponNameArray() -> const std::vector< std::string >&
{
	return m_weapon_name_array;
}

auto GetWeaponIndex( const std::string& name ) -> int
{
	if( !name.compare( "desert eagle" ) )
		return WEAPON_DEAGLE;
	else if( !name.compare( "dual berettas" ) )
		return WEAPON_ELITE;
	else if( !name.compare( "five-seven" ) )
		return WEAPON_FIVESEVEN;
	else if( !name.compare( "glock-18" ) )
		return WEAPON_GLOCK;
	else if( !name.compare( "ak-47" ) )
		return WEAPON_AK47;
	else if( !name.compare( "aug" ) )
		return WEAPON_AUG;
	else if( !name.compare( "awp" ) )
		return WEAPON_AWP;
	else if( !name.compare( "famas" ) )
		return WEAPON_FAMAS;
	else if( !name.compare( "g3sg1" ) )
		return WEAPON_G3SG1;
	else if( !name.compare( "galil ar" ) )
		return WEAPON_GALILAR;
	else if( !name.compare( "m249" ) )
		return WEAPON_M249;
	else if( !name.compare( "m4a4" ) )
		return WEAPON_M4A1;
	else if( !name.compare( "mac-10" ) )
		return WEAPON_MAC10;
	else if( !name.compare( "p90" ) )
		return WEAPON_P90;
	else if( !name.compare( "ump-45" ) )
		return WEAPON_UMP45;
	else if( !name.compare( "xm1014" ) )
		return WEAPON_XM1014;
	else if( !name.compare( "pp-bizon" ) )
		return WEAPON_BIZON;
	else if( !name.compare( "mag-7" ) )
		return WEAPON_MAG7;
	else if( !name.compare( "negev" ) )
		return WEAPON_NEGEV;
	else if( !name.compare( "sawed-off" ) )
		return WEAPON_SAWEDOFF;
	else if( !name.compare( "tec-9" ) )
		return WEAPON_TEC9;
	else if( !name.compare( "zeus x27" ) )
		return WEAPON_TASER;
	else if( !name.compare( "p2000" ) )
		return WEAPON_HKP2000;
	else if( !name.compare( "mp7" ) )
		return WEAPON_MP7;
	else if( !name.compare( "mp9" ) )
		return WEAPON_MP9;
	else if( !name.compare( "nova" ) )
		return WEAPON_NOVA;
	else if( !name.compare( "p250" ) )
		return WEAPON_P250;
	else if( !name.compare( "scar-20" ) )
		return WEAPON_SCAR20;
	else if( !name.compare( "sg 556" ) )
		return WEAPON_SG556;
	else if( !name.compare( "ssg 08" ) )
		return WEAPON_SSG08;
	else if( !name.compare( "m4a1-s" ) )
		return WEAPON_M4A1_SILENCER;
	else if( !name.compare( "usp-s" ) )
		return WEAPON_USP_SILENCER;
	else if( !name.compare( "cz75 auto" ) )
		return WEAPON_CZ75A;
	else if( !name.compare( "r8 revolver" ) )
		return WEAPON_REVOLVER;
	else
		return ITEM_NONE;
}

std::vector< std::string > aimbot_data_t::m_mode_array =
{
	"off",
	"auto",
	"key",
};

std::vector< std::string > aimbot_data_t::m_silent_array =
{
	"off",
	"normal",
	"perfect",
};

std::vector< std::string > aimbot_data_t::m_target_spot_array =
{
	"head",			   // 0
	"neck",			   // 1
	"lower neck",	   // 2
	"pelvis",		   // 3
	"body",			   // 4
	"thorax",		   // 5
	"chest",		   // 6
	"upper chest",	   // 7
	"right thigh",	   // 8
	"left thigh",	   // 9
	"right calf",	   // 10
	"left calf",	   // 11
	"right foot",	   // 12
	"left foot",	   // 13
	"right hand",	   // 14
	"left hand",	   // 15
	"right upper arm", // 16
	"right forearm",   // 17
	"left upper arm",  // 18
	"left forearm"	   // 19
};

std::vector< std::string > aimbot_data_t::m_target_hitscan_array =
{
	"off",
	"low",
	"medium",
	"high",
};

std::vector< std::string > aimbot_data_t::m_target_adjustment_array =
{
	"off",
	"normal",
	"backtrack",
};

std::vector< std::string > aimbot_data_t::m_target_resolve_array =
{
	"off",
	"bruteforce",
	"smart",
};

std::vector< std::string > aimbot_data_t::m_target_priority_array =
{
	"fast",
	"distance",
	"fov",
};

std::vector< std::string > aimbot_data_t::m_step_mode_array =
{
	"off",
	"constant",
	"decreasing",
};

std::vector< std::string > aimbot_data_t::m_rcs_mode_array =
{
	"off",
	"target",
	"standalone",
};


std::vector< std::string > triggerbot_data_t::m_mode_array =
{
	"off",
	"auto",
	"key",
};


std::vector< std::string > esp_data_t::m_box_mode_array =
{
	"off",
	"normal",
	"corners",
};

std::vector< std::string > esp_data_t::m_style_array =
{
	"off",
	"text",
	"bar",
};


std::vector< std::string > misc_data_t::m_restriction_array =
{
	"off",
	"matchmaking",
	"faceit",
};

std::vector< std::string > misc_data_t::m_strafe_mode_array =
{
	"off",
	"normal",
	"boost",
};

std::vector< std::string > misc_data_t::m_antiaim_pitch_array =
{
	"off",
	"zero",
	"up",
	"down",
	"fake up",
};

std::vector< std::string > misc_data_t::m_antiaim_yaw_array =
{
	"off",
	"zero",
	"left",
	"right",
	"forward",
	"backward",
};

std::vector< std::string > misc_data_t::m_crosshair_mode_array =
{
	"off",
	"dot",
	"cross",
};

}