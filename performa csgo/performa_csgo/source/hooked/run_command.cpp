#include "procedure.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../feature/anim_state.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace hooked
{

auto __fastcall RunCommand( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper ) -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& anim_state = feature::AnimState::Instance();

	auto fn = ( source::m_prediction_swap ?
				source::m_prediction_swap->Get< RunCommandFn >( index::IPrediction::RunCommand ) :
				memory::vget< RunCommandFn >( ecx, index::IPrediction::RunCommand ) );

	fn( ecx, player, ucmd, moveHelper );

	prediction.RunCommand( player );
}

}

}