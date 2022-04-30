#include "aimbot.hpp"
#include "accuracy.hpp"
#include "lag_compensation.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

#include "../hooked/procedure.hpp"

namespace source
{

namespace feature
{

auto Aimbot::Create( win32::Input& input ) -> bool
{
	input.AddEvent( &Aimbot::Event );
	return true;
}

auto Aimbot::Destroy() -> void
{

}

#define CONTENTS_HITBOX 0x40000000

auto GetHitChance( const Vector& point, float chance ) -> bool
{
	auto& prediction = engine::Prediction::Instance();

	auto local = C_CSPlayer::GetLocalCSPlayer();
	auto gun = ( C_WeaponCSBase* )local->GetActiveWeapon();
	auto gun_data = gun->GetCSWeaponData();

	if( local->m_vecVelocity().Length() > 0.1f )
	{
		auto is_crouching = prediction.GetFlags() & FL_DUCKING;
		auto spread_cone = gun->m_fAccuracyPenalty() * 256.f / g_pi_radians + ( is_crouching ? gun_data->m_inaccuracy_crouch : gun_data->m_inaccuracy_move ) * local->m_vecVelocity().Length() / 3000.f;

		auto a = ( point - local->EyePosition() ).Length();
		auto b = std::sqrt( std::tan( spread_cone * g_pi_radians / 180.f ) * a );

		if( 2.2f > b )
			return true;

		return ( ( 2.2f / std::max( b, 2.2f ) ) * 100.f ) > chance / 1.5f;
	}
	else
	{
		auto spread = gun->m_fAccuracyPenalty();

		if( spread <= ( ( ( 101.f - chance ) / 20.f ) / ( 100.f ) ) )
			return true;
		else
			return false;
	}
}

auto Aimbot::CreateMove( bool& send_packet ) -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& aimbot = option::m_current.m_aimbot;

	m_cmd = prediction.GetUserCmd();
	m_player = prediction.GetPlayer();
	m_weapon = prediction.GetWeapon();
	m_data = m_weapon->GetCSWeaponData();

	if( !aimbot.m_mode )
		return;

	if( aimbot.m_mode == 2 )
	{
		if( !m_key_pressed )
		{
			m_target_cycle = true;
			m_timer.Reset();
			return;
		}
	}

	if( !m_weapon->IsGun() )
		return;

	if( !m_weapon->m_iClip1() )
		return;

	if( m_target_cycle )
		ScanTarget();

	if( IsPlayerValid( m_target_player ) )
	{
		if( aimbot.m_time_delay )
		{
			if( m_timer.Elapsed() < aimbot.m_time_delay )
				return;
		}

		if( aimbot.m_time_duration )
		{
			if( m_timer.Elapsed() > aimbot.m_time_duration )
				return;
		}

		auto direction = m_target_spot - m_player->EyePosition();
		direction.Normalize();

		auto angles = direction.ToEulerAngles();
		angles.Normalize();

		if( aimbot.m_rcs_mode )
		{
			auto angles_recoil = m_player->m_aimPunchAngle();

			if( m_player->m_iShotsFired() >= aimbot.m_rcs_delay )
			{
				if( aimbot.m_rcs_pitch )
					angles[ 0 ] -= angles_recoil[ 0 ] * ( static_cast< float >( aimbot.m_rcs_pitch ) / 50.f );

				if( aimbot.m_rcs_yaw )
					angles[ 1 ] -= angles_recoil[ 1 ] * ( static_cast< float >( aimbot.m_rcs_yaw ) / 50.f );
			}
		}

		if( aimbot.m_step_mode == 1 )
			ConstantStep( angles );
		else if( aimbot.m_step_mode == 2 )
			DecreasingStep( angles );

		
		m_cmd->viewangles = angles;
		m_cmd->viewangles.Normalize();

		if( !aimbot.m_silent )
			source::m_engine_client->SetViewAngles( m_cmd->viewangles );

		if( aimbot.m_auto_fire )
			m_cmd->buttons |= IN_ATTACK;

		if( aimbot.m_hitchance )
		{
			if( !GetHitChance( m_target_spot, static_cast< float >( aimbot.m_hitchance ) ) )
			{
				m_cmd->buttons &= ~IN_ATTACK;
				m_target_tick = -1;
			}
		}

		if( m_target_tick != -1 )
			m_cmd->tick_count = m_target_tick;
	}
	else
	{
		if( aimbot.m_auto_cycle )
		{
			if( aimbot.m_time_cycle_delay )
			{
				if( m_timer.Elapsed() < aimbot.m_time_cycle_delay )
					return;
			}

			m_target_cycle = true;
		}
	}

	if( aimbot.m_silent == 2 )
	{
		if( m_cmd->buttons & IN_ATTACK && m_weapon->IsFireTime() )
			send_packet = false;
	}
}

auto Aimbot::SetKeyPressed( bool value ) -> void
{
	m_key_pressed = value;
}

auto Aimbot::GetTargetPlayer() -> int
{
	return m_target_player;
}

enum CSGOHitboxID
{
	Head = 0,		// 0
	Neck,			// 1
	NeckLower,		// 2
	Pelvis,			// 3
	Stomach,		// 4
	LowerChest,		// 5
	Chest,			// 6
	UpperChest,		// 7
	RightThigh,		// 8
	LeftThigh,		// 9
	RightShin,		// 10
	LeftShin,		// 11
	RightFoot,		// 12
	LeftFoot,		// 13
	RightHand,		// 14
	LeftHand,		// 15
	RightUpperArm,	// 16
	RightLowerArm,	// 17
	LeftUpperArm,	// 18
	LeftLowerArm	// 19
};

auto GetHitscanSpotArray( C_CSPlayer* player, std::vector< int >& output ) -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	output.clear();

	switch( aimbot.m_target_hitscan )
	{
		case 0:
		{
			// off
			break;
		}
		case 1:
		{
			if( aimbot.m_target_spot != 0 )
				output.emplace_back( Head );
			else if( aimbot.m_target_spot != 1 )
				output.emplace_back( Neck );
			else if( aimbot.m_target_spot != 2 )
				output.emplace_back( UpperChest );
			else if( aimbot.m_target_spot != 3 )
				output.emplace_back( Pelvis );
			else if( aimbot.m_target_spot != 4 )
				output.emplace_back( Stomach );

			output.emplace_back( Chest );
			break;
		}
		case 2:
		{
			if( aimbot.m_target_spot != 0 )
				output.emplace_back( Head );
			else if( aimbot.m_target_spot != 1 )
				output.emplace_back( Neck );
			else if( aimbot.m_target_spot != 2 )
				output.emplace_back( UpperChest );
			else if( aimbot.m_target_spot != 3 )
				output.emplace_back( Pelvis );
			else if( aimbot.m_target_spot != 4 )
				output.emplace_back( Stomach );

			output.emplace_back( Chest );
			output.emplace_back( LeftUpperArm );
			output.emplace_back( RightUpperArm );
			output.emplace_back( LeftThigh );
			output.emplace_back( RightThigh );
			break;
		}
		case 3:
		{
			if( aimbot.m_target_spot != 0 )
				output.emplace_back( Head );
			else if( aimbot.m_target_spot != 1 )
				output.emplace_back( Neck );
			else if( aimbot.m_target_spot != 2 )
				output.emplace_back( UpperChest );
			else if( aimbot.m_target_spot != 3 )
				output.emplace_back( Pelvis );
			else if( aimbot.m_target_spot != 4 )
				output.emplace_back( Stomach );

			output.emplace_back( Chest );
			output.emplace_back( LeftUpperArm );
			output.emplace_back( RightUpperArm );
			output.emplace_back( LeftThigh );
			output.emplace_back( RightThigh );
			output.emplace_back( LeftHand );
			output.emplace_back( RightHand );
			output.emplace_back( LeftFoot );
			output.emplace_back( RightFoot );
			output.emplace_back( LeftShin );
			output.emplace_back( RightShin );
			output.emplace_back( LeftLowerArm );
			output.emplace_back( RightLowerArm );
			break;
		}
	}
}

auto GetHitscanPointArray( C_CSPlayer* player, matrix3x4_t* transform, std::vector< Vector >& output ) -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	output.clear();

	auto local = C_CSPlayer::GetLocalCSPlayer();

	switch( aimbot.m_target_spot )
	{
		// head
		case 0:
		{
			player->GetSpotPointArray( 0, transform, output );
			break;
		}
		// neck
		case 1:
		{
			player->GetSpotPointArray( 1, transform, output );
			break;
		}
		// chest
		case 2:
		{
			player->GetSpotPointArray( 6, transform, output );
			break;
		}
		// pelvis
		case 3:
		{
			player->GetSpotPointArray( 3, transform, output );
			break;
		}
		// stomach
		case 4:
		{
			player->GetSpotPointArray( 4, transform, output );
			break;
		}
		// intelligent
		case 5:
		{
			auto shots = local->m_iShotsFired();

			if( shots > 1 )
				player->GetSpotPointArray( 0, transform, output );
			else
				player->GetSpotPointArray( 3, transform, output );

			break;
		}
	}
}

auto Aimbot::IsPlayerValid( int index ) -> bool
{
	auto& accuracy = Accuracy::Instance();
	auto& lag_compensation = LagCompensation::Instance();
	auto& aimbot = option::m_current.m_aimbot;

	auto player = C_CSPlayer::GetCSPlayer( index );

	if( !player )
		return false;

	if( player == m_player )
		return false;

	if( player->IsDormant() )
		return false;

	if( player->m_lifeState() != LIFE_ALIVE )
		return false;

	if( !aimbot.m_target_friendly )
	{
		if( player->m_iTeamNum() == m_player->m_iTeamNum() )
			return false;
	}

	lag_compensation.BackupPlayer( player );

	if( aimbot.m_target_adjustment )
		m_target_tick = lag_compensation.GetCorrectCommand( player, ( aimbot.m_target_adjustment == 2 ) );

	matrix3x4_t transform[ 128 ] = { };

	if( !player->GetBoneTransform( transform ) )
	{
		lag_compensation.RestorePlayer( player );
		return false;
	}

	Vector spot = { };

	if( !player->GetBoxWorld( aimbot.m_target_spot, transform, spot ) )
	{
		lag_compensation.RestorePlayer( player );
		return false;
	}

	auto damage = -1.f;

	if( aimbot.m_auto_wall )
	{
		damage = accuracy.GetPenetrationDamage( player, m_player->EyePosition(), spot );
	}
	else
	{
		if( !accuracy.TraceLine( spot, player ) )
		{
			lag_compensation.RestorePlayer( player );
			return false;
		}

		damage = accuracy.GetDamage( player, m_player->EyePosition(), spot );
	}

	if( damage < static_cast< float >( aimbot.m_min_damage ) )
	{
		lag_compensation.RestorePlayer( player );
		return false;
	}

	m_target_spot = spot;

	/*if( aimbot.m_target_hitscan )
	{
		auto damage = accuracy.GetDamage( player, m_player->EyePosition(), spot );

		if( damage < static_cast< float >( aimbot.m_min_damage ) )
		{
			std::vector< Vector > point_array = { };
			GetHitscanPointArray( player, transform, point_array );

			for( const auto& point : point_array )
			{
				auto damage = accuracy.GetDamage( player, m_player->EyePosition(), point );

				if( damage < static_cast< float >( aimbot.m_min_damage ) )
				{
					if( aimbot.m_auto_wall )
						damage = accuracy.GetPenetrationDamage( player, m_player->EyePosition(), point );
				}
				
				if( damage >= static_cast< float >( aimbot.m_min_damage ) )
					m_target_spot = point;
			}

			point_array.clear();

			std::vector< int > spot_array = { };
			GetHitscanSpotArray( player, spot_array );

			auto best_spot_scan = false;
			auto best_spot_damage = 0.f;

			for( const auto& spot : spot_array )
			{
				player->GetSpotPointArray( spot, transform, point_array );

				auto best_point_scan = false;
				auto best_point_damage = 0.f;

				for( const auto& point : point_array )
				{
					auto damage = accuracy.GetDamage( player, m_player->EyePosition(), point );

					if( damage < static_cast< float >( aimbot.m_min_damage ) )
					{
						if( aimbot.m_auto_wall )
							damage = accuracy.GetPenetrationDamage( player, m_player->EyePosition(), point );
					}
					
					if( damage > best_point_damage )
					{
						best_point_scan = true;
						best_point_damage = damage;

						m_target_spot = point;
					}
				}

				if( !best_point_scan )
					continue;

				if( best_point_damage > best_spot_damage )
				{
					best_spot_scan = true;
					best_spot_damage = best_point_damage;
				}
			}

			if( !best_spot_scan )
			{
				lag_compensation.RestorePlayer( player );
				return false;
			}
		}
		else
		{
			m_target_spot = spot;
		}
	}
	else
	{
		if( accuracy.TraceLine( spot, player ) )
		{
			auto damage = accuracy.GetDamage( player, m_player->EyePosition(), spot );

			if( damage >= static_cast< float >( aimbot.m_min_damage ) )
			{
				m_target_spot = spot;
			}
			else
			{
				lag_compensation.RestorePlayer( player );
				return false;
			}
		}
		else
		{
			if( aimbot.m_auto_wall )
			{
				auto damage = accuracy.GetPenetrationDamage( player, m_player->EyePosition(), spot );

				if( damage >= static_cast< float >( aimbot.m_min_damage ) )
				{
					m_target_spot = spot;
				}
				else
				{
					lag_compensation.RestorePlayer( player );
					return false;
				}
			}
			else
			{
				lag_compensation.RestorePlayer( player );
				return false;
			}
		}
	}*/

	if( m_target_spot.Distance( m_player->EyePosition() ) > m_data->m_range )
	{
		lag_compensation.RestorePlayer( player );
		return false;
	}

	if( aimbot.m_target_priority == 2 )
	{
		if( AngleDistance( m_cmd->viewangles + m_player->m_aimPunchAngle() * 2.f, m_player->EyePosition(), m_target_spot ) > aimbot.m_target_fov )
		{
			lag_compensation.RestorePlayer( player );
			return false;
		}
	}

	lag_compensation.RestorePlayer( player );
	return true;
}

auto Aimbot::GetPlayerSpot( C_CSPlayer* player, int hitbox, Vector& output ) -> bool
{
	auto& aimbot = option::m_current.m_aimbot;
	auto& lag_compensation = LagCompensation::Instance();

	matrix3x4_t player_transform[ 128 ] = { };
	Vector player_spot = { };

	if( aimbot.m_target_adjustment )
	{
		lag_compensation.BackupPlayer( player );
		m_target_tick = lag_compensation.GetCorrectCommand( player, aimbot.m_target_adjustment == 2 );

		if( !player->GetBoneTransform( player_transform ) )
		{
			lag_compensation.RestorePlayer( player );
			return false;
		}

		if( !player->GetBoxWorld( aimbot.m_target_spot, player_transform, player_spot ) )
		{
			lag_compensation.RestorePlayer( player );
			return false;
		}

		lag_compensation.RestorePlayer( player );
	}
	else
	{
		m_target_tick = -1;

		if( !player->GetBoneTransform( player_transform ) )
			return false;

		if( !player->GetBoxWorld( aimbot.m_target_spot, player_transform, player_spot ) )
			return false;
	}

	output = player_spot;
	return true;
}

auto Aimbot::DropTarget() -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	if( aimbot.m_auto_cycle )
		m_target_cycle = true;

	m_target_tick = -1;
	m_target_player = -1;
	m_target_spot.Init();
}

auto Aimbot::ScanTarget() -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	auto best_distance = m_data->m_range;

	if( aimbot.m_target_priority == 2 )
		best_distance = aimbot.m_target_fov;

	for( auto i = 1; i <= source::m_engine_client->GetMaxClients(); i++ )
	{
		if( IsPlayerValid( i ) )
		{
			if( aimbot.m_target_priority == 0 )
			{
				m_target_cycle = false;
				m_target_player = i;
				break;
			}
			else if( aimbot.m_target_priority == 1 )
			{
				auto distance = m_target_spot.Distance( m_player->EyePosition() );

				if( distance < best_distance )
				{
					best_distance = distance;
					m_target_cycle = false;
					m_target_player = i;
					break;
				}
			}
			else if( aimbot.m_target_priority == 2 )
			{
				auto distance = AngleDistance( m_cmd->viewangles + m_player->m_aimPunchAngle() * 2.f, m_player->EyePosition(), m_target_spot );

				if( distance < best_distance )
				{
					best_distance = distance;
					m_target_cycle = false;
					m_target_player = i;
					break;
				}
			}
		}
		else
		{
			m_target_tick = -1;
		}
	}
}

auto Aimbot::ConstantStep( QAngle& angles ) -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	auto distance = angles - m_cmd->viewangles;
	distance.Normalize();

	if( aimbot.m_step_pitch > 0.f )
	{
		auto factor_pitch = ( aimbot.m_step_pitch / 100.f );

		if( distance[ 0 ] < 0.f )
		{
			if( factor_pitch > std::abs( distance[ 0 ] ) )
				factor_pitch = std::abs( distance[ 0 ] );

			angles[ 0 ] = m_cmd->viewangles[ 0 ] - factor_pitch;
		}
		else
		{
			if( factor_pitch > distance[ 0 ] )
				factor_pitch = distance[ 0 ];

			angles[ 0 ] = m_cmd->viewangles[ 0 ] + factor_pitch;
		}
	}

	if( aimbot.m_step_yaw > 0.f )
	{
		auto factor_yaw = ( aimbot.m_step_yaw / 100.f );

		if( distance[ 1 ] < 0.f )
		{
			if( factor_yaw > std::abs( distance[ 1 ] ) )
				factor_yaw = std::abs( distance[ 1 ] );

			angles[ 1 ] = m_cmd->viewangles[ 1 ] - factor_yaw;
		}
		else
		{
			if( factor_yaw > distance[ 1 ] )
				factor_yaw = distance[ 1 ];

			angles[ 1 ] = m_cmd->viewangles[ 1 ] + factor_yaw;
		}
	}
}

auto Aimbot::DecreasingStep( QAngle& angles ) -> void
{
	auto& aimbot = option::m_current.m_aimbot;

	auto distance = m_cmd->viewangles - angles;

	auto factor_pitch = aimbot.m_step_pitch;
	auto factor_yaw = aimbot.m_step_yaw;

	if( factor_pitch > 0.0f )
		angles[ 0 ] = m_cmd->viewangles[ 0 ] - distance[ 0 ] / factor_pitch;

	if( factor_yaw > 0.0f )
		angles[ 1 ] = m_cmd->viewangles[ 1 ] - distance[ 1 ] / factor_yaw;
}

auto Aimbot::Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool
{
	auto& data = Aimbot::Instance();
	auto& aimbot = option::m_current.m_aimbot;

	if( aimbot.m_mode != 2 )
		return false;

	if( aimbot.m_key == 1 ) // Mouse 1
	{
		if( message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_LBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( aimbot.m_key == 2 ) // Mouse 2
	{
		if( message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_RBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( aimbot.m_key == 4 ) // Mouse 3
	{
		if( message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_MBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( aimbot.m_key == 5 ) // Mouse 4
	{
		auto key = HIWORD( wparam );

		if( key == XBUTTON1 )
		{
			if( message == WM_XBUTTONDOWN || message == WM_XBUTTONDBLCLK )
				data.SetKeyPressed( true );
			else if( message == WM_XBUTTONUP )
				data.SetKeyPressed( false );
		}
	}
	else if( aimbot.m_key == 6 ) // Mouse 5
	{
		auto key = HIWORD( wparam );

		if( key == XBUTTON2 )
		{
			if( message == WM_XBUTTONDOWN || message == WM_XBUTTONDBLCLK )
				data.SetKeyPressed( true );
			else if( message == WM_XBUTTONUP )
				data.SetKeyPressed( false );
		}
	}
	else
	{
		if( wparam == aimbot.m_key )
		{
			if( message == WM_KEYDOWN || message == WM_SYSKEYDOWN )
				data.SetKeyPressed( true );
			else if( message == WM_KEYUP || message == WM_SYSKEYUP )
				data.SetKeyPressed( false );
		}
	}

	return false;
}

}

}