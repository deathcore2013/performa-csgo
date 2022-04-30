#include "prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace engine
{

auto Prediction::Begin( CUserCmd* cmd, bool* send_packet ) -> bool
{
	m_cmd = cmd;
	m_send_packet = send_packet;

	m_player = C_CSPlayer::GetLocalCSPlayer();

	if( !m_player )
		return false;

	if( m_player->m_lifeState() != LIFE_ALIVE )
		return false;

	m_weapon = reinterpret_cast< C_WeaponCSBase* >( m_player->GetActiveWeapon() );

	if( !m_weapon )
		return false;

	m_weapon->UpdateAccuracyPenalty();

	m_time_current = source::m_globals->curtime;
	m_time_frame = source::m_globals->frametime;

	m_flags = m_player->m_fFlags();

	if( m_cmd_previous )
	{
		if( m_cmd_previous->hasbeenpredicted )
			m_server_command = m_player->m_nTickBase();
		else
			m_server_command++;
	}
	else
	{
		m_server_command = m_player->m_nTickBase();
	}

	m_server_time = static_cast< float >( m_server_command ) * source::m_globals->interval_per_tick;


	source::m_globals->curtime = static_cast< float >( m_player->m_nTickBase() ) * source::m_globals->interval_per_tick;
	source::m_globals->frametime = source::m_globals->interval_per_tick;

	source::m_move_helper->SetHost( m_player );

	m_player->m_pCurrentCommand() = m_cmd;

	C_BaseEntity::SetPredictionSeed( m_cmd );

	CMoveData move = { };

	source::m_game_movement->StartTrackPredictionErrors( m_player );
	source::m_prediction->SetupMove( m_player, m_cmd, source::m_move_helper, &move );
	source::m_game_movement->ProcessMovement( m_player, &move );
	source::m_prediction->FinishMove( m_player, m_cmd, &move );
	source::m_game_movement->FinishTrackPredictionErrors( m_player );

	C_BaseEntity::SetPredictionSeed( nullptr );

	m_player->m_pCurrentCommand() = nullptr;

	source::m_move_helper->SetHost( nullptr );

	return true;
}

auto Prediction::End() -> void
{
	source::m_globals->curtime = m_time_current;
	source::m_globals->frametime = m_time_frame;

	m_cmd_previous = m_cmd;
}

auto Prediction::FrameStageNotify( ClientFrameStage_t stage ) -> void
{
	if( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	auto player = C_CSPlayer::GetLocalCSPlayer();

	if( !player )
		return;

	auto entry_index = ( player->m_nTickBase() % 128 );
	auto entry = GetPlayerCacheEntry( entry_index );

	if( !entry )
		return;

	player->m_aimPunchAngle() = entry->m_aimPunchAngle;
	player->m_viewPunchAngle() = entry->m_viewPunchAngle;
}

auto Prediction::RunCommand( C_BasePlayer* base_player ) -> void
{
	auto player = C_CSPlayer::GetLocalCSPlayer();

	if( !player )
		return;

	if( player != base_player )
		return;

	auto entry_index = ( player->m_nTickBase() % 128 );
	auto entry = GetPlayerCacheEntry( entry_index );

	if( !entry )
		return;

	entry->m_aimPunchAngle = player->m_aimPunchAngle();
	entry->m_viewPunchAngle = player->m_viewPunchAngle();
}

auto Prediction::GetUserCmd() const -> CUserCmd*
{
	return m_cmd;
}

auto Prediction::GetPlayer() const -> C_CSPlayer*
{
	return m_player;
}

auto Prediction::GetWeapon() const -> C_WeaponCSBase*
{
	return m_weapon;
}

auto Prediction::GetFlags() const -> int
{
	return m_flags;
}

auto Prediction::GetServerCommand() const -> int
{
	return m_server_command;
}

auto Prediction::GetServerTime() const -> float
{
	return m_server_time;
}

auto Prediction::GetPlayerCacheEntry( int index ) -> player_cache_entry_t*
{
	if( index < 0 || index >= 128 )
		return nullptr;

	return &m_player_cache[ index ];
}

}

}