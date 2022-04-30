#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

struct player_cache_entry_t
{
	QAngle m_aimPunchAngle = { };
	QAngle m_viewPunchAngle = { };
};

class Prediction : public core::Singleton< Prediction >
{
public:
	auto Begin( CUserCmd* cmd, bool* send_packet ) -> bool;
	auto End() -> void;

	auto FrameStageNotify( ClientFrameStage_t stage ) -> void;
	auto RunCommand( C_BasePlayer* base_player ) -> void;

	auto GetUserCmd() const -> CUserCmd*;
	auto GetPlayer() const -> C_CSPlayer*;
	auto GetWeapon() const -> C_WeaponCSBase*;

	auto GetFlags() const -> int;

	auto GetServerCommand() const -> int;
	auto GetServerTime() const -> float;

private:
	auto GetPlayerCacheEntry( int index ) -> player_cache_entry_t*;

private:
	CUserCmd* m_cmd = nullptr;
	CUserCmd* m_cmd_previous = nullptr;
	bool* m_send_packet = nullptr;
	C_CSPlayer* m_player = nullptr;
	C_WeaponCSBase* m_weapon = nullptr;

	float m_time_current = 0.f;
	float m_time_frame = 0.f;

	int m_flags = 0;

	int m_server_command = 0;
	float m_server_time = 0.f;

	player_cache_entry_t m_player_cache[ 128 ] = { };
};

}

}