#include "resolver.hpp"
#include "aimbot.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

RenderData g_render_data = { };

namespace source
{

namespace feature
{

auto Resolver::FrameStageNotify( ClientFrameStage_t stage ) -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& aimbot = Aimbot::Instance();

	m_cmd = nullptr;

	auto frame_net_update_postdataupdate_start = ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START );
	auto frame_render_start = ( stage == FRAME_RENDER_START );

	if( !( frame_net_update_postdataupdate_start || frame_render_start ) )
		return;

	m_player = C_CSPlayer::GetLocalCSPlayer();

	if( !m_player->IsGood() )
		return;

	m_cmd = prediction.GetUserCmd();

	if( !m_cmd )
		return;

	auto target_player_index = aimbot.GetTargetPlayer();
	
	for( auto player_index = 1; player_index <= source::m_engine_client->GetMaxClients(); player_index++ )
	{
		auto player = C_CSPlayer::GetCSPlayer( player_index );

		if( !player )
			continue;

		if( player->IsLocal() )
			continue;

		if( !player->IsGood() )
		{
			ResetPlayerData( player );
			continue;
		}

		Correct( player, frame_render_start );
	}
}

auto Resolver::FireGameEvent( IGameEvent* game_event ) -> void
{
	if( !game_event )
		return;

	auto game_event_name = game_event->GetName();

	if( !game_event_name )
		return;

	if( !std::strcmp( game_event_name, "player_hurt" ) )
	{
		auto player_index = source::m_engine_client->GetPlayerForUserID( game_event->GetInt( "attacker" ) );
		auto player = C_CSPlayer::GetCSPlayer( player_index );

		if( player && player_index == source::m_engine_client->GetLocalPlayer() )
		{
			auto player_hit_index = source::m_engine_client->GetPlayerForUserID( game_event->GetInt( "userID" ) );
			auto player_hit = C_CSPlayer::GetCSPlayer( player_hit_index );

			auto hitgroup = game_event->GetInt( "hitgroup" );

			OnPlayerHurt( player_hit, hitgroup == 1 );
		}
	}
	else if( !std::strcmp( game_event_name, "bullet_impact" ) )
	{
		OnBulletImpact();
	}
}

auto IsFakeHead( C_CSPlayer* player ) -> bool
{
	return ( std::abs( AngleNormalize( player->m_angEyeAngles()[ 1 ] - player->m_flLowerBodyYawTarget() ) ) > 35.f );
}

auto Resolver::Correct( C_CSPlayer* player, bool frame_render_start /*= false*/ ) -> void
{
	auto& prediction = engine::Prediction::Instance();

	auto player_index = ( player->entindex() - 1 );
	auto& player_data = m_player_resolve_data[ player_index ];

	if( frame_render_start )
	{
		player_data.m_angRender = player->m_angEyeAngles();
		return;
	}

	auto& player_eye_angles = player->m_angEyeAngles();

	player_data.m_vecVelocity = player->m_vecVelocity();
	player_data.m_angAngles = player->m_angEyeAngles();

	player_data.m_nShotsFired = m_player->m_iShotsFired();
	player_data.m_nShotsWrong = m_player->m_iShotsFired();
	
	player_data.m_flSimulationTime = player->m_flSimulationTime();
	player_data.m_flLowerBodyYaw = player->m_flLowerBodyYawTarget();
	
	/*auto is_moving = ( player->m_vecVelocity().Length() > 36.f );
	auto is_onground = ( player->m_fFlags() & FL_ONGROUND );

	player_data.m_bLowerBodyUpdate = ( is_onground && ( is_moving || player_data.m_flLowerBodyYaw != player_data.m_flLastLowerBodyYaw ) );
	
	if( player_data.m_bLowerBodyUpdate )
	{
		player_data.m_flLastLowerBodyYaw = player_data.m_flLowerBodyYaw;
		m_flLowerBodyUpdate[ player_index ] = player_data.m_flSimulationTime;
	}*/

	// if( player_data.m_flLowerBodyYaw != player_data.m_flLastLowerBodyYaw )
	// {
	// 	player_data.m_flLastLowerBodyYaw = player_data.m_flLowerBodyYaw;
	// 	player_data.m_flLowerBodyYawTime = prediction.GetServerTime();
	// }

	auto server_time = prediction.GetServerTime();

	auto onground = ( player->m_fFlags() & FL_ONGROUND );
	auto moving = ( onground && ( player->m_vecVelocity().Length2D() > 36.f ) );

	auto lower_body_snap = std::abs( AngleNormalize( player_data.m_angAngles[ 1 ] - player_data.m_flLowerBodyYaw ) );

	if( moving )
	{
		player_data.m_bLowerBodyUpdate = true;
		player_data.m_flLowerBodyNextUpdate = 0.22f;
		player_data.m_flLowerBodyLastUpdate = server_time;
	}
	else if( onground && ( server_time > ( player_data.m_flLowerBodyLastUpdate + player_data.m_flLowerBodyNextUpdate ) ) )
	{
		player_data.m_bLowerBodyUpdate = true;
		player_data.m_flLowerBodyNextUpdate = 1.1f;
		player_data.m_flLowerBodyLastUpdate = server_time;
	}
	else
	{
		player_data.m_bLowerBodyUpdate = false;
	}

	if( player_data.m_angAdjust[ 1 ] > 135.f )
		player_data.m_angAdjust[ 1 ] = 0.f;

	if( player_data.m_nShotsWrong > 2 )
		player_eye_angles[ 1 ] += player_data.m_flFakeBruteYaw;

	for( auto layer_index = 0; layer_index < MAX_LAYER_RECORDS; layer_index++ )
	{
		auto layer = player->GetAnimOverlay( layer_index );
		auto& layer_data = player_data.m_LayerData[ layer_index ];

		if( layer )
		{
			layer_data.m_nAct = player->GetSequenceAct( layer->m_nSequence );
			layer_data.m_nSequence = layer->m_nSequence;
			layer_data.m_nOrder = layer->m_nOrder;
			layer_data.m_flCycle = layer->m_flCycle;
			layer_data.m_flPrevCycle = layer->m_flPrevCycle;
			layer_data.m_flWeight = layer->m_flWeight;
			layer_data.m_flWeightDeltaRate = layer->m_flWeightDeltaRate;

			if( layer_data.m_nOrder == 12 )
			{
				player_data.m_flFakeWalkDetect[ m_cmd->command_number % 2 ] = layer_data.m_flWeight;
				player_data.m_flFakeWalkDetectTime[ m_cmd->command_number % 2 ] = player_data.m_flSimulationTime;

				for( auto index = 0; index < 2; index++ )
				{
					if( player_data.m_flFakeWalkDetect[ index ] > 0.f )
					{
						player_data.m_nResetWalk = index;
					}
					else if( index == 1 )
					{
						if( player_data.m_vecVelocity.Length2D() < 0.5f && layer_data.m_flWeight == 0.f )
							player_data.m_bFakeWalking = false;
					}
					else
					{
						if( player_data.m_nResetWalk )
							player_data.m_nResetWalkNext = player_data.m_nResetWalk - 1;
						else
							player_data.m_nResetWalkNext = player_data.m_nResetWalk + 1;

						if( player_data.m_flFakeWalkDetect[ player_data.m_nResetWalkNext ] == 0.f )
							player_data.m_bFakeWalking = true;
					}
				}
			}

			if( ( player_data.m_vecVelocity.Length2D() >= 0.5f && layer_data.m_nOrder == 6 && layer_data.m_flWeight >= 0.55f ) ||
				( player_data.m_vecVelocity.Length2D() > 0.5f && layer_data.m_nOrder == 5 && layer_data.m_flWeight >= 0.55f ) ||
				( player_data.m_vecVelocity.Length2D() >= 0.5f && !player_data.m_bFakeWalking ) )
			{
				player_data.m_flLastMoveLowerBodyYaw = player_data.m_flLowerBodyYaw;
				player_eye_angles[ 1 ] = player_data.m_flLowerBodyYaw;
			}
			else
			{
				if( layer_data.m_nAct == 979 && layer_data.m_flWeight > 0.05f )
				{
					if( player_data.m_angAdjust[ 1 ] > 0.f )
						player_eye_angles[ 1 ] = ( player_data.m_flLowerBodyYaw - player_data.m_angAdjust[ 1 ] - 75.f );
					else
						player_eye_angles[ 1 ] = ( player_data.m_flLowerBodyYaw - 75.f );
				}
				else if( ( layer_data.m_nAct == 979 && layer_data.m_flWeight == 0.f && layer_data.m_flCycle >= 0.97f ) || layer_data.m_nAct == 980 )
				{
					if( player_data.m_angAdjust[ 1 ] > 0.f )
						player_eye_angles[ 1 ] = ( player_data.m_flLowerBodyYaw - player_data.m_angAdjust[ 1 ] );
					else
						player_eye_angles[ 1 ] = ( player_data.m_flLowerBodyYaw - 15.f );
				}
			}
		}
	}

	player_eye_angles.Normalize();
}

auto Resolver::GetPlayerData( C_CSPlayer* player ) -> PlayerResolveData&
{
	auto player_index = ( player->entindex() - 1 );
	return m_player_resolve_data[ player_index ];
}

auto Resolver::ResetPlayerData( C_CSPlayer* player ) -> void
{
	auto player_index = ( player->entindex() - 1 );
	auto& player_data = m_player_resolve_data[ player_index ];

	player_data.m_bFakeAngles = false;
	player_data.m_bFakeWalking = false;

	player_data.m_nShotsFired = 0;
	player_data.m_nShotsHit = 0;
	player_data.m_nShotsWrong = 0;

	player_data.m_flFakeBruteYaw = 0.f;
}

auto Resolver::GetLowerBodyUpdate( C_CSPlayer* player ) -> float
{
	auto player_index = ( player->entindex() - 1 );
	return m_flLowerBodyUpdate[ player_index ];
}

auto Resolver::OnPlayerHurt( C_CSPlayer* player, bool is_headshot ) -> void
{
	if( !m_player->IsGood() )
		return;

	if( !player->IsGood() )
		return;

	auto player_index = ( player->entindex() - 1 );
	auto& player_data = m_player_resolve_data[ player_index ];

	if( is_headshot )
	{
		player_data.m_flFakeBruteYaw = 0.f;

		if( player_data.m_nShotsFired )
		{
			player_data.m_nShotsHit++;
			player_data.m_nShotsWrong = ( player_data.m_nShotsFired - player_data.m_nShotsHit );
		}
	}
}

auto Resolver::OnBulletImpact() -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& aimbot = feature::Aimbot::Instance();

	if( !m_player->IsGood() )
		return;

	auto cmd = prediction.GetUserCmd();

	if( cmd )
	{
		auto target_player_index = aimbot.GetTargetPlayer();
		auto target_player = C_CSPlayer::GetCSPlayer( target_player_index );

		if( target_player->IsGood() )
		{
			auto& player_data = m_player_resolve_data[ target_player_index - 1 ];
			auto& player_eye_angles = target_player->m_angEyeAngles();

			auto shots = m_player->m_iShotsFired();

			if( shots > 2 )
			{
				shots = ( shots - 2 );
				player_data.m_flFakeBruteYaw = ( ( shots % 5 ) * 45.f );
			}
		}
	}
}

}

}