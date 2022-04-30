#include "movement.hpp"
#include "anim_state.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

namespace source
{

namespace feature
{

auto Movement::Begin( engine::Prediction& prediction, bool& send_packet ) -> bool
{
	auto& misc = option::m_misc;

	m_send_packet = &send_packet;

	m_cmd = prediction.GetUserCmd();
	m_player = prediction.GetPlayer();
	m_weapon = prediction.GetWeapon();

	m_flags = prediction.GetFlags();

	m_angles = m_cmd->viewangles;
	m_angles_render = m_cmd->viewangles;

	if( !m_cvar_sensitivity )
		m_cvar_sensitivity = source::m_cvar->FindVar( "sensitivity" );

	if( !m_cvar_m_pitch )
		m_cvar_m_pitch = source::m_cvar->FindVar( "m_pitch" );

	if( !m_cvar_m_yaw )
		m_cvar_m_yaw = source::m_cvar->FindVar( "m_yaw" );

	auto velocity = m_player->m_vecVelocity();

	if( misc.m_auto_jump )
	{
		if( m_cmd->buttons & IN_JUMP )
		{
			if( misc.m_restriction )
			{
				if( !m_jump_first )
				{
					m_jump_first = true;
					m_jump_fake = true;
				}
				else if( !( m_flags & FL_ONGROUND ) )
				{
					if( m_jump_fake && velocity[ 2 ] < 0.f )
						m_jump_fake = false;
					else
						m_cmd->buttons &= ~IN_JUMP;
				}
				else
				{
					m_jump_fake = true;
				}
			}
			else
			{
				if( !( m_flags & FL_ONGROUND ) )
					m_cmd->buttons &= ~IN_JUMP;
			}
		}
	}

	if( misc.m_auto_pistol )
	{
		if( m_weapon->IsPistol() )
		{
			if( m_cmd->buttons & IN_ATTACK )
			{
				if( m_weapon->IsFireTime() )
					m_cmd->buttons &= ~IN_ATTACK;
			}

			if( m_cmd->buttons & IN_ATTACK2 )
			{
				if( m_weapon->IsSecondaryFireTime() )
					m_cmd->buttons &= ~IN_ATTACK2;
			}
		}
	}

	if( misc.m_strafe_mode )
	{
		if( !( m_flags & FL_ONGROUND ) )
		{
			auto speed = velocity.Length2D();

			if( speed > 10.f )
			{
				auto angles_distance = m_angles_previous - m_angles_render;
				angles_distance.Normalize();

				auto yaw_snap = ToDegrees( std::atan2( 15.f, speed ) );

				if( yaw_snap < 0.f )
					yaw_snap += 360.f;

				AngleNormalize( yaw_snap );

				if( misc.m_strafe_mode == 1 || std::abs( angles_distance[ 1 ] ) > yaw_snap )
				{
					if( angles_distance[ 1 ] > 0.f )
						m_cmd->sidemove = 450.f;
					else if( angles_distance[ 1 ] < 0.f )
						m_cmd->sidemove = -450.f;
				}
				else
				{
					if( m_cmd->command_number % 2 )
					{
						m_angles_render[ 1 ] += yaw_snap;
						m_cmd->sidemove = 450.f;
					}
					else
					{
						m_angles_render[ 1 ] -= yaw_snap;
						m_cmd->sidemove = -450.f;
					}
				}
			}
			else
			{
				m_cmd->forwardmove = 450.f;
			}
		}
		else
		{
			if( misc.m_strafe_ground )
			{
				if( m_cmd->command_number % 2 )
					m_angles_render[ 1 ] += 26.86f;
				else
					m_angles_render[ 1 ] -= 26.86f;
			}
		}
	}

	return true;
}

auto Movement::FrameStageNotify( ClientFrameStage_t stage ) -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& anim_state = feature::AnimState::Instance();
	
	if( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	auto local = C_CSPlayer::GetLocalCSPlayer();

	if( !local->IsGood() )
		return;

	auto flServerTime = prediction.GetServerTime();

	auto bIsOnGround = ( local->m_fFlags() & FL_ONGROUND );
	auto bIsMoving = ( local->m_vecVelocity().Length2D() > 0.1f );

	m_angEyeAngles = local->m_angEyeAngles();
	m_flLowerBody = anim_state.GetLowerBody();

	auto flLowerBodySnap = std::abs( AngleNormalize( m_angEyeAngles[ 1 ] - m_flLowerBody ) );

	if( bIsOnGround && bIsMoving )
	{
		m_bLowerBodyUpdate = true;

		m_flLowerBodyNextUpdate = 0.22f;
		m_flLowerBodyLastUpdate = flServerTime;
	}
	else if( bIsOnGround /*&& ( flLowerBodySnap > 35.f )*/ && ( flServerTime > ( m_flLowerBodyLastUpdate + m_flLowerBodyNextUpdate ) ) )
	{
		m_bLowerBodyUpdate = true;

		m_flLowerBodyNextUpdate = 1.1f;
		m_flLowerBodyLastUpdate = flServerTime;
	}
	else
	{
		m_bLowerBodyUpdate = false;
	}
}

auto Movement::AnglesToPixels( const QAngle& angles_begin, const QAngle& angles_end ) -> QAngle
{
	auto sensitivity = m_cvar_sensitivity->GetFloat();

	auto m_pitch = m_cvar_m_pitch->GetFloat();
	auto m_yaw = m_cvar_m_yaw->GetFloat();

	auto angles_distance = angles_begin - angles_end;
	angles_distance.Normalize();

	auto pixel_move_x = ( -angles_distance[ 0 ] ) / ( m_yaw * sensitivity );
	auto pixel_move_y = ( angles_distance[ 1 ] ) / ( m_pitch * sensitivity );

	return { pixel_move_y, pixel_move_x, 0.f };
}

auto Movement::PixelsDistanceToAnglesDistance( const QAngle& pixels_distance ) -> QAngle
{
	auto sensitivity = m_cvar_sensitivity->GetFloat();

	auto m_pitch = m_cvar_m_pitch->GetFloat();
	auto m_yaw = m_cvar_m_yaw->GetFloat();

	auto angle_move_x = ( -pixels_distance[ 0 ] ) * ( m_yaw * sensitivity );
	auto angle_move_y = ( pixels_distance[ 1 ] ) * ( m_pitch * sensitivity );

	return { angle_move_y, angle_move_x, 0.f };
}

auto Movement::End() -> void
{
	auto& misc = option::m_misc;
	auto& anim_state = AnimState::Instance();

	if( m_cmd->buttons & IN_ATTACK && !m_weapon->IsFireTime() )
		m_cmd->viewangles = m_angles;

	auto get_target_yaw = []( C_CSPlayer* local ) -> float
	{
		auto best_spot = Vector::Zero;
		auto best_player = ( C_CSPlayer* )nullptr;
		auto best_distance = 8192.f;

		for( auto index = 1; index <= source::m_engine_client->GetMaxClients(); index++ )
		{
			auto player = C_CSPlayer::GetCSPlayer( index );

			if( !player->IsGood() )
				continue;

			if( local->m_iTeamNum() == player->m_iTeamNum() )
				continue;

			matrix3x4_t transform[ 128 ] = { };

			if( !player->GetBoneTransform( transform ) )
				continue;

			Vector spot = { };

			if( !player->GetBoxWorld( 0, transform, spot ) )
				continue;

			auto distance = spot.Distance( local->EyePosition() );

			if( distance < best_distance )
			{
				best_spot = spot;
				best_player = player;
				best_distance = distance;
			}
		}

		if( best_player )
		{
			auto direction = ( best_spot - local->EyePosition() );
			direction.Normalize();

			auto angles = direction.ToEulerAngles();
			angles.Normalize();

			return angles[ 1 ];
		}

		return 0.f;
	};

	if( !( m_cmd->buttons & IN_ATTACK && m_weapon->IsFireTime() ) )
	{
		if( misc.m_antiaim_target )
		{
			auto target_yaw = get_target_yaw( m_player );

			if( target_yaw != 0.f )
				m_cmd->viewangles[ 1 ] = target_yaw;
		}

		if( misc.m_antiaim_packet )
		{
			static auto command_lag = misc.m_antiaim_packet;

			if( !command_lag )
			{
				command_lag = misc.m_antiaim_packet;
				*m_send_packet = true;
			}
			else
			{
				command_lag--;
				*m_send_packet = false;
			}
		}

		auto correct_pitch = []( CUserCmd* cmd, int mode ) -> void
		{
			if( mode == 1 )
				cmd->viewangles[ 0 ] = 0.f;
			else if( mode == 2 )
				cmd->viewangles[ 0 ] = -89.f;
			else if( mode == 3 )
				cmd->viewangles[ 0 ] = 89.f;
			else if( mode == 4 )
				cmd->viewangles[ 0 ] = 180.f;
		};

		correct_pitch( m_cmd, misc.m_antiaim_pitch );
		m_cmd->viewangles[ 0 ] += misc.m_antiaim_pitch_mod;

		auto correct_yaw = []( CUserCmd* cmd, int mode ) -> void
		{
			if( mode == 1 )
				cmd->viewangles[ 1 ] = 0.f;
			else if( mode == 2 )
				cmd->viewangles[ 1 ] += 90.f;
			else if( mode == 3 )
				cmd->viewangles[ 1 ] -= 90.f;
			else if( mode == 5 )
				cmd->viewangles[ 1 ] += 180.f;
		};

		if( *m_send_packet )
		{
			correct_yaw( m_cmd, misc.m_antiaim_yaw_fake );
			m_cmd->viewangles[ 1 ] += misc.m_antiaim_yaw_fake_mod;
		}
		else
		{
			correct_yaw( m_cmd, misc.m_antiaim_yaw_real );
			m_cmd->viewangles[ 1 ] += misc.m_antiaim_yaw_real_mod;
		}

		if( misc.m_antiaim_break_lower_body )
		{
			if( *m_send_packet )
			{
				m_cmd->viewangles[ 1 ] += 90.f;
			}
			else
			{
				if( m_bLowerBodyUpdate )
					m_cmd->viewangles[ 1 ] += 90.f;
				else
					m_cmd->viewangles[ 1 ] -= 90.f;
			}
		}

		static auto side = false;

		if( GetAsyncKeyState( 'N' ) & 1 )
			side = !side;

		if( side )
			m_cmd->viewangles[ 1 ] += 180.f;
	}

	auto move = Vector( m_cmd->forwardmove, m_cmd->sidemove, m_cmd->upmove );
	auto speed = move.Length2D();

	auto yaw_move = ToDegrees( std::atan2( move[ 1 ], move[ 0 ] ) );

	if( yaw_move < 0.f )
		yaw_move += 360.f;

	auto yaw_distance = m_cmd->viewangles[ 1 ] - m_angles_render[ 1 ] + yaw_move;

	if( yaw_distance < 0.f )
		yaw_distance += 360.f;

	auto yaw_radians = ToRadians( yaw_distance );

	m_cmd->forwardmove = std::cos( yaw_radians ) * speed;
	m_cmd->sidemove = std::sin( yaw_radians ) * speed;


	if( misc.m_restriction == 1 )
	{
		/*if( m_cmd->viewangles != m_angles_previous )
		{
			constexpr auto angle_yaw_distance = 40.f;

			auto angles_distance = m_cmd->viewangles - m_angles_previous;
			angles_distance.Normalize();

			if( angles_distance.Length() > angle_yaw_distance )
				m_cmd->viewangles = m_angles_previous + angles_distance * angle_yaw_distance;
		}*/

		m_cmd->viewangles.Normalize();
		
	}
	else if( misc.m_restriction == 2 )
	{
		if( m_cmd->viewangles != m_angles_previous )
		{
			auto sensitivity = m_cvar_sensitivity->GetFloat();

			auto m_pitch = m_cvar_m_pitch->GetFloat();
			auto m_yaw = m_cvar_m_yaw->GetFloat();


			auto angles_pixels = AnglesToPixels( m_angles_previous, m_cmd->viewangles );

			angles_pixels[ 0 ] = std::round( angles_pixels[ 0 ] / sensitivity ) * sensitivity;
			angles_pixels[ 1 ] = std::round( angles_pixels[ 1 ] / sensitivity ) * sensitivity;

			auto angles_distance = PixelsDistanceToAnglesDistance( angles_pixels );

			m_cmd->viewangles = m_angles_previous + angles_distance;

			auto pixels = AnglesToPixels( m_angles_previous, m_cmd->viewangles );

			m_cmd->mousedx = static_cast< short >( pixels[ 0 ] );
			m_cmd->mousedy = static_cast< short >( pixels[ 1 ] );
		}
	}

	m_angles_previous = m_angles;

	if( misc.m_restriction )
		m_cmd->viewangles.Clamp();

	if( *m_send_packet )
		m_angles_fake = QAngle{ 0.f, m_cmd->viewangles[ 1 ], 0.f };
	else
		m_angles_real = QAngle{ m_cmd->viewangles[ 0 ], m_cmd->viewangles[ 1 ], 0.f };

	m_angles_lower_body = QAngle{ 0.f, anim_state.GetLowerBody(), 0.f };
}

auto Movement::GetRealAngles() const -> const QAngle&
{
	return m_angles_real;
}

auto Movement::GetFakeAngles() const -> const QAngle&
{
	return m_angles_fake;
}

auto Movement::GetLowerBodyAngles() const -> const QAngle&
{
	return m_angles_lower_body;
}

auto Movement::IsLowerBodyUpdate() const -> bool
{
	return m_bLowerBodyUpdate;
}

}

}