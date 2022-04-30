#include "procedure.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../feature/anim_state.hpp"
#include "../feature/lag_compensation.hpp"
#include "../feature/resolver.hpp"
#include "../feature/movement.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

namespace source
{

namespace hooked
{

auto __fastcall FrameStageNotify( void* ecx, void* edx, ClientFrameStage_t curStage ) -> void
{
	auto& misc = option::m_misc;
	auto& prediction = engine::Prediction::Instance();

	auto& anim_state = feature::AnimState::Instance();
	auto& lag_compensation = feature::LagCompensation::Instance();
	auto& resolver = feature::Resolver::Instance();
	auto& movement = feature::Movement::Instance();

	lag_compensation.FrameStageNotify( curStage );

	if( curStage == FRAME_RENDER_START )
	{
		auto local = C_CSPlayer::GetLocalCSPlayer();

		if( local->IsGood() )
		{
			static auto cheats = false;

			if( misc.m_thirdperson )
			{
				if( !cheats )
				{
					auto cvar_cheats = source::m_cvar->FindVar( "sv_cheats" );
					auto cvar_cheats_spoofed = new SpoofedConVar( cvar_cheats );

					cvar_cheats_spoofed->SetBool( true );
					
					cheats = true;
				}

				auto angles = QAngle::Zero;
				source::m_engine_client->GetViewAngles( angles );

				source::m_input->m_fCameraInThirdPerson = true;
				source::m_input->m_vecCameraOffset = { angles[ 0 ], angles[ 1 ], 128.f };
				
				local->v_angle() = movement.GetRealAngles();
			}
			else
			{
				source::m_input->m_fCameraInThirdPerson = false;
				source::m_input->m_vecCameraOffset[ 2 ] = 150.f;
			}
		}
		else
		{
			source::m_input->m_fCameraInThirdPerson = false;
			source::m_input->m_vecCameraOffset[ 2 ] = 150.f;
		}
	}

	auto fn = ( source::m_base_client_swap ?
				source::m_base_client_swap->Get< FrameStageNotifyFn >( index::IBaseClientDLL::FrameStageNotify ) :
				memory::vget< FrameStageNotifyFn >( ecx, index::IBaseClientDLL::FrameStageNotify ) );

	fn( ecx, curStage );

	if( curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START )
	{
		auto local = C_CSPlayer::GetLocalCSPlayer();

		if( local->IsGood() )
		{
			anim_state.UpdateServerAnimation( local->m_angEyeAngles() );
			movement.FrameStageNotify( curStage );

			auto& data = g_render_data;

			data.m_flSimulationTime = local->m_flSimulationTime();
			data.m_flLowerBodyYawTarget = local->m_flLowerBodyYawTarget();

			data.m_angEyeAngles = local->m_angEyeAngles();
			data.m_angEyeAngles.Normalize();

			for( auto layer_index = 0; layer_index < MAX_LAYER_RECORDS; layer_index++ )
			{
				auto layer = local->GetAnimOverlay( layer_index );

				if( layer )
					std::memcpy( &data.m_AnimationLayers[ layer_index ], layer, sizeof( C_AnimationLayer ) );
			}
		}
	}

	resolver.FrameStageNotify( curStage );
	prediction.FrameStageNotify( curStage );
}

}

}