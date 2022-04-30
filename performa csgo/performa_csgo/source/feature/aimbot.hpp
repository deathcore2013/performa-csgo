#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

class Prediction;

}

namespace feature
{

class Aimbot : public core::Singleton< Aimbot >
{
public:
	auto Create( win32::Input& input ) -> bool;
	auto Destroy() -> void;

	auto CreateMove( bool& send_packet ) -> void;

	auto SetKeyPressed( bool value ) -> void;

	auto GetTargetPlayer() -> int;

private:
	auto IsPlayerValid( int index ) -> bool;
	auto DropTarget() -> void;
	auto ScanTarget() -> void;

	auto ConstantStep( QAngle& angles ) -> void;
	auto DecreasingStep( QAngle& angles ) -> void;

	auto GetPlayerSpot( C_CSPlayer* player, int hitbox, Vector& output ) -> bool;

private:
	static auto Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool;

private:
	CUserCmd* m_cmd = nullptr;
	C_CSPlayer* m_player = nullptr;
	C_WeaponCSBase* m_weapon = nullptr;
	CCSWeaponInfo* m_data = nullptr;

	bool m_key_pressed = false;
	
	core::Timer m_timer = { };

	bool m_target_cycle = false;
	int m_target_tick = -1;
	int m_target_player = -1;
	Vector m_target_spot = { };
};

}

}