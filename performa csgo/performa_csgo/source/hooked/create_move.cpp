#include "procedure.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../feature/accuracy.hpp"
#include "../feature/aimbot.hpp"
#include "../feature/anim_state.hpp"
#include "../feature/movement.hpp"
#include "../feature/triggerbot.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

namespace source
{

namespace hooked
{

auto m_send_packet = false;

auto compensate_spread( CUserCmd* cmd ) -> void
{
	auto& accuracy = feature::Accuracy::Instance();
	auto& removal = option::m_removal;

	if( !removal.m_spread )
		return;

	auto local = C_CSPlayer::GetLocalCSPlayer();
	auto weapon = ( C_WeaponCSBase* )local->GetActiveWeapon();

	if( weapon->IsGun() )
	{
		auto gun = weapon;

		if( cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2 )
		{
			if( gun->IsFireTime() || gun->IsSecondaryFireTime() )
				accuracy.CompensateSpread( cmd->viewangles, cmd->viewangles, cmd->random_seed );
		}
	}
}

auto compensate_recoil( CUserCmd* cmd ) -> void
{
	auto& accuracy = feature::Accuracy::Instance();
	auto& removal = option::m_removal;

	if( !removal.m_recoil )
		return;

	auto local = C_CSPlayer::GetLocalCSPlayer();
	auto weapon = ( C_WeaponCSBase* )local->GetActiveWeapon();

	if( weapon->IsGun() )
	{
		auto gun = weapon;

		if( cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2 )
		{
			if( gun->IsFireTime() || gun->IsSecondaryFireTime() )
				accuracy.CompensateRecoil( cmd->viewangles, cmd->viewangles );
		}
	}
}



auto create_move_call( int sequence_number, float input_sample_frametime, bool active ) -> void
{
	auto& prediction = engine::Prediction::Instance();

	auto& aimbot = feature::Aimbot::Instance();
	auto& anim_state = feature::AnimState::Instance();
	auto& movement = feature::Movement::Instance();
	auto& triggerbot = feature::Triggerbot::Instance();

	auto& misc = option::m_misc;

	auto cmd = source::m_input->GetUserCmd( sequence_number );

	if( cmd )
	{
		if( prediction.Begin( cmd, &m_send_packet ) )
		{
			option::CreateMove();

			if( movement.Begin( prediction, m_send_packet ) )
			{
				auto angles = cmd->viewangles;

				aimbot.CreateMove( m_send_packet );
				triggerbot.CreateMove();

				compensate_spread( cmd );
				compensate_recoil( cmd );

				auto player = C_CSPlayer::GetLocalCSPlayer();
				auto weapon = ( C_WeaponCSBase* )player->GetActiveWeapon();

				if( cmd->buttons & IN_ATTACK && !weapon->IsFireTime() )
					cmd->viewangles = angles;

				/*if( !( cmd->buttons & IN_ATTACK && weapon->IsFireTime() ) )
				{
					static auto angle_snap_yaw = 25.f;

					cmd->viewangles[ 0 ] = 89.f;
					cmd->viewangles[ 1 ] += 180.f;

					static auto lag_command = 10;

					if( !lag_command )
					{
						lag_command = 10;
						m_send_packet = true;
						cmd->viewangles[ 1 ] -= angle_snap_yaw;
					}
					else
					{
						lag_command--;
						m_send_packet = false;
						cmd->viewangles[ 1 ] += angle_snap_yaw;
					}

					// if( anim_state.GetNextBodyUpdate() )
					// {
					// 	if( m_send_packet )
					// 		cmd->viewangles[ 1 ] -= 60.f;
					// 	else
					// 		cmd->viewangles[ 1 ] += 60.f;
					// 	// source::m_engine_client->ClientCmd_Unrestricted( "echo update" );
					// }
				}*/

				/*if( misc.m_antiaim_break_lower_body )
				{
					if( m_send_packet )
					{
						cmd->viewangles[ 1 ] += 90.f;
					}
					else
					{
						if( anim_state.GetNextBodyUpdate() )
							cmd->viewangles[ 1 ] += 90.f;
						else
							cmd->viewangles[ 1 ] -= 90.f;
					}
				}*/

				movement.End();
			}
			prediction.End();
		}

		auto verified = source::m_input->GetVerifiedCmd( sequence_number );

		if( verified )
		{
			verified->m_cmd = *cmd;
			verified->m_crc = cmd->GetChecksum();
		}
	}
}

std::uintptr_t m_create_move = 0u;
std::uintptr_t m_create_move_ret = 0u;

auto __declspec( naked ) __fastcall CreateMove( int sequence_number, float input_sample_frametime, bool active ) -> void
{
	m_create_move = source::m_base_client_swap->Get< std::uintptr_t >( index::IBaseClientDLL::CreateMove );

	__asm
	{
		push eax
		mov eax, [ esp + 4 ]
		mov m_create_move_ret, eax
		pop eax
		add esp, 4
		call m_create_move
		mov m_send_packet, bl
		pushad
	}

	create_move_call( sequence_number, input_sample_frametime, active );

	__asm
	{
		popad
		mov bl, m_send_packet
		jmp m_create_move_ret
	}
}

}

}