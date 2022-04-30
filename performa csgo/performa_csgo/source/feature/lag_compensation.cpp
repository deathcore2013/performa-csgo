#include "lag_compensation.hpp"
#include "resolver.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace feature
{

auto LagCompensation::FrameStageNotify( ClientFrameStage_t stage ) -> void
{
	static auto cvar_unlag = source::m_cvar->FindVar( "sv_unlag" );
	static auto cvar_maxunlag = source::m_cvar->FindVar( "sv_maxunlag" );

	if( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	m_player = C_CSPlayer::GetLocalCSPlayer();

	if( !m_player->IsGood() )
		return;

	if( source::m_engine_client->GetMaxClients() <= 1 )
	{
		ClearHistory();
		return;
	}

	auto time_dead = static_cast< int >( source::m_globals->curtime - cvar_maxunlag->GetFloat() );

	for( auto player_index = 1; player_index <= source::m_engine_client->GetMaxClients(); player_index++ )
	{
		auto player = C_CSPlayer::GetCSPlayer( player_index );
		auto& player_history = m_lag_record[ player_index - 1 ];

		if( !player->IsGood() )
		{
			player_history.clear();
			continue;
		}

		*( int* )( player + 0x0A28 ) = 0;
		*( int* )( player + 0x0A30 ) = source::m_globals->framecount;

		DisableInterpolation( player );

		while( !player_history.empty() )
		{
			auto& record = player_history.back();

			if( record.m_flSimulationTime > static_cast< float >( time_dead ) )
				break;

			player_history.pop_back();
		}

		if( !player_history.empty() )
		{
			auto& record = player_history.front();

			if( record.m_flSimulationTime >= player->m_flSimulationTime() )
				continue;
		}

		auto& record = player_history.emplace_front();

		player->InvalidateBoneCache();

		record.m_nSequence = player->m_nSequence();
		record.m_flCycle = player->m_flCycle();

		record.m_flSimulationTime = player->m_flSimulationTime();
		record.m_flLowerBodyYawTarget = player->m_flLowerBodyYawTarget();

		std::memcpy( record.m_flPoseParameter, player->m_flPoseParameter(), sizeof( float[ 24 ] ) );

		record.m_vecOrigin = player->m_vecOrigin();
		record.m_angEyeAngles = player->GetAbsAngles();

		for( auto index = 0; index < MAX_LAYER_RECORDS; index++ )
		{
			auto layer = player->GetAnimOverlay( index );

			record.m_LayerRecord[ index ].m_nOrder = layer->m_nOrder;
			record.m_LayerRecord[ index ].m_nSequence = layer->m_nSequence;

			record.m_LayerRecord[ index ].m_flCycle = layer->m_flCycle;
			record.m_LayerRecord[ index ].m_flWeight = layer->m_flWeight;
		}
	}
}

auto LagCompensation::IsRecordOutOfBounds( const LagRecord& record ) -> bool
{
	static auto cvar_maxunlag = source::m_cvar->FindVar( "sv_maxunlag" );

	auto net = source::m_engine_client->GetNetChannelInfo();

	auto time_lerp = GetLerpTime();
	auto time_outgoing = net->GetLatency( FLOW_OUTGOING );
	auto time_incoming = net->GetLatency( FLOW_INCOMING );

	auto time_correct = ( time_outgoing + time_incoming + time_lerp );
	time_correct = std::clamp( time_correct, 0.f, cvar_maxunlag->GetFloat() );

	auto time_distance = time_correct - ( source::m_globals->curtime - record.m_flSimulationTime );

	return ( std::abs( time_distance ) > 0.2f );
}

auto LagCompensation::StartLagCompensation( C_CSPlayer* player, LagRecord& record ) -> bool
{
	if( !player->IsGood() || player->IsLocal() )
		return false;

	if( IsRecordOutOfBounds( record ) )
		return false;

	BacktrackPlayer( player, record );
	return true;
}

auto LagCompensation::BacktrackPlayer( C_CSPlayer* player, LagRecord& record ) -> void
{
	if( !player->IsGood() || player->IsLocal() )
		return;

	/*auto best_record_index = -1;
	auto best_record_time = 0.f;

	for( auto index = 0u; index < m_lag_record[ player->entindex() - 1 ].size(); index++ )
	{
		auto& record_last = m_lag_record[ player->entindex() - 1 ].at( index );

		if( record_last.m_flSimulationTime < record.m_flSimulationTime )
		{
			if( record_last.m_flSimulationTime > best_record_time )
			{
				best_record_time = record_last.m_flSimulationTime;
				best_record_index = static_cast< int >( index );
			}
		}
	}

	if( best_record_index == -1 )
		return;

	auto& record_recent = m_lag_record[ player->entindex() - 1 ].at( best_record_index );
	auto acceleration = ( record.m_vecOrigin - record_recent.m_vecOrigin );

	if( acceleration.LengthSquared() > 4096.f )
	{
		auto command_distance = TIME_TO_TICKS( record.m_flSimulationTime - record_recent.m_flSimulationTime );

		if( command_distance )
		{
			acceleration /= static_cast< float >( command_distance );

			for( auto command = 0; command < command_distance; command++ )
				record.m_vecOrigin += acceleration * source::m_globals->interval_per_tick;
		}
	}*/

	player->InvalidateBoneCache();

	player->m_nSequence() = record.m_nSequence;
	player->m_flCycle() = record.m_flCycle;

	player->m_flSimulationTime() = record.m_flSimulationTime;
	player->m_flLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

	std::memcpy( player->m_flPoseParameter(), record.m_flPoseParameter, sizeof( float[ 24 ] ) );

	player->SetAbsOrigin( record.m_vecOrigin );
	player->SetAbsAngles( record.m_angEyeAngles );

	for( auto index = 0; index < MAX_LAYER_RECORDS; index++ )
	{
		auto layer = player->GetAnimOverlay( index );

		layer->m_nOrder = record.m_LayerRecord[ index ].m_nOrder;
		layer->m_nSequence = record.m_LayerRecord[ index ].m_nSequence;

		layer->m_flCycle = record.m_LayerRecord[ index ].m_flCycle;
		layer->m_flWeight = record.m_LayerRecord[ index ].m_flWeight;
	}

	player->UpdateClientSideAnimation();
}

auto LagCompensation::FinishLagCompensation( C_CSPlayer* player ) -> void
{
	
}

auto LagCompensation::GetCorrectCommand( C_CSPlayer* player, bool backtrack /*= false*/ ) -> int
{
	auto& resolver = Resolver::Instance();

	if( !player->IsGood() || player->IsLocal() )
		return -1;

	auto player_index = ( player->entindex() - 1 );
	auto& player_history = m_lag_record[ player_index ];

	if( player_history.empty() )
		return -1;

	auto command = -1;

	if( backtrack )
	{
		// try backtrack to lower_body update
		auto& player_data = resolver.GetPlayerData( player );

		if( player_data.m_bLowerBodyUpdate )
		{
			for( auto& record : player_history )
			{
				// is there desired record?
				if( record.m_flSimulationTime == resolver.GetLowerBodyUpdate( player ) )
				{
					if( StartLagCompensation( player, record ) )
					{
						command = GetRecordCommand( player, record );
						FinishLagCompensation( player );
						break;
					}
				}
			}
		}
		else
		{
			for( auto index = ( player_history.size() - 1u ); index >= 0; index-- )
			{
				auto& record = player_history[ index ];

				if( StartLagCompensation( player, record ) )
				{
					command = GetRecordCommand( player, record );
					FinishLagCompensation( player );
					break;
				}
			}
		}
	}
	else
	{
		for( auto& record : player_history )
		{
			if( StartLagCompensation( player, record ) )
			{
				command = GetRecordCommand( player, record );
				FinishLagCompensation( player );
				break;
			}
		}
	}

	return command;
}

auto LagCompensation::ClearHistory() -> void
{
	for( auto& player_history : m_lag_record )
		player_history.clear();
}

auto LagCompensation::BackupPlayer( C_CSPlayer* player ) -> void
{
	if( !player->IsGood() || player->IsLocal() )
		return;

	auto player_index = ( player->entindex() - 1 );
	auto& record = m_restore_record[ player_index ];

	player->InvalidateBoneCache();

	record.m_nSequence = player->m_nSequence();
	record.m_flCycle = player->m_flCycle();

	record.m_flSimulationTime = player->m_flSimulationTime();
	record.m_flLowerBodyYawTarget = player->m_flLowerBodyYawTarget();

	std::memcpy( record.m_flPoseParameter, player->m_flPoseParameter(), sizeof( float[ 24 ] ) );

	record.m_vecOrigin = player->GetAbsOrigin();
	record.m_angEyeAngles = player->GetAbsAngles();

	for( auto index = 0; index < MAX_LAYER_RECORDS; index++ )
	{
		auto layer = player->GetAnimOverlay( index );

		record.m_LayerRecord[ index ].m_nOrder = layer->m_nOrder;
		record.m_LayerRecord[ index ].m_nSequence = layer->m_nSequence;

		record.m_LayerRecord[ index ].m_flCycle = layer->m_flCycle;
		record.m_LayerRecord[ index ].m_flWeight = layer->m_flWeight;
	}
}

auto LagCompensation::RestorePlayer( C_CSPlayer* player ) -> void
{
	if( !player->IsGood() || player->IsLocal() )
		return;

	auto player_index = ( player->entindex() - 1 );
	auto& record = m_restore_record[ player_index ];

	player->InvalidateBoneCache();

	player->m_nSequence() = record.m_nSequence;
	player->m_flCycle() = record.m_flCycle;

	player->m_flSimulationTime() = record.m_flSimulationTime;
	player->m_flLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

	std::memcpy( player->m_flPoseParameter(), record.m_flPoseParameter, sizeof( float[ 24 ] ) );

	player->SetAbsOrigin( record.m_vecOrigin );
	player->SetAbsAngles( record.m_angEyeAngles );

	for( auto index = 0; index < MAX_LAYER_RECORDS; index++ )
	{
		auto layer = player->GetAnimOverlay( index );

		layer->m_nOrder = record.m_LayerRecord[ index ].m_nOrder;
		layer->m_nSequence = record.m_LayerRecord[ index ].m_nSequence;

		layer->m_flCycle = record.m_LayerRecord[ index ].m_flCycle;
		layer->m_flWeight = record.m_LayerRecord[ index ].m_flWeight;
	}

	player->UpdateClientSideAnimation();
}

auto LagCompensation::GetRecordCommand( C_CSPlayer* player, LagRecord& record ) -> int
{
	if( !player->IsGood() || player->IsLocal() )
		return -1;

	auto time_lerp = GetLerpTime();
	auto time_record = record.m_flSimulationTime;
	
	if( time_record == 0.f )
		time_record = player->m_flSimulationTime();

	auto command_lerp = TIME_TO_TICKS( time_lerp );
	auto command_record = TIME_TO_TICKS( time_record );

	return ( command_record + command_lerp );
}

auto LagCompensation::GetLerpTime() -> float
{
	static auto cvar_interp = source::m_cvar->FindVar( "cl_interp" );
	static auto cvar_interp_ratio = source::m_cvar->FindVar( "cl_interp_ratio" );
	static auto cvar_min_interp_ratio = source::m_cvar->FindVar( "sv_client_min_interp_ratio" );
	static auto cvar_max_interp_ratio = source::m_cvar->FindVar( "sv_client_max_interp_ratio" );
	static auto cvar_update_rate = source::m_cvar->FindVar( "cl_updaterate" );
	static auto cvar_min_update_rate = source::m_cvar->FindVar( "sv_minupdaterate" );
	static auto cvar_max_update_rate = source::m_cvar->FindVar( "sv_maxupdaterate" );

	auto interp = cvar_interp->GetFloat();
	auto interp_ratio = cvar_interp_ratio->GetFloat();
	auto min_interp_ration = cvar_min_interp_ratio->GetFloat();
	auto max_interp_ration = cvar_max_interp_ratio->GetFloat();
	auto update_rate = cvar_update_rate->GetInt();
	auto min_update_rate = cvar_min_update_rate->GetInt();
	auto max_update_rate = cvar_max_update_rate->GetInt();

	auto clamp_interp_ration = std::clamp( interp_ratio, min_interp_ration, max_interp_ration );
	auto clamp_update_rate = std::clamp( update_rate, min_update_rate, max_update_rate );

	auto lerp = clamp_interp_ration / static_cast< float >( clamp_update_rate );

	if( lerp <= interp )
		lerp = interp;

	return lerp;
}

}

}