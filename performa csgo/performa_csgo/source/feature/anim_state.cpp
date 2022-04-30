#include "anim_state.hpp"
#include "lag_compensation.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace feature
{

auto AnimState::UpdateServerAnimation( const QAngle& angles ) -> void
{
	auto& prediction = engine::Prediction::Instance();
	auto& lag_compensation = LagCompensation::Instance();

	m_pLocalPlayer = C_CSPlayer::GetLocalCSPlayer();

	if( !m_pLocalPlayer )
		return;

	auto allocate = ( m_pAnimState == nullptr );
	auto change = ( !allocate ) && ( m_pLocalPlayer->GetRefEHandle() != m_uEntHandle );
	auto reset = ( !allocate && !change ) && ( m_pLocalPlayer->m_flSpawnTime() != m_flSpawnTime );

	if( change )
		source::m_mem_alloc->Free( m_pAnimState );

	if( reset )
	{
		ResetState( m_pAnimState );
		m_flSpawnTime = m_pLocalPlayer->m_flSpawnTime();
	}

	if( allocate || change )
	{
		auto state = ( CCSGOPlayerAnimState* )source::m_mem_alloc->Alloc( 0x0344 );

		if( state != nullptr )
			CreateState( state, m_pLocalPlayer );

		m_uEntHandle = m_pLocalPlayer->GetRefEHandle();
		m_flSpawnTime = m_pLocalPlayer->m_flSpawnTime();

		m_pAnimState = state;
	}
	else if( !source::m_client_state->m_nChokedCommands() )
	{
		m_angRealAngle = angles;

		// lag_compensation.BackupPlayer( m_pLocalPlayer );

		UpdateState( m_pAnimState, m_angRealAngle );
		m_flLowerBody = m_pLocalPlayer->m_flLowerBodyYawTarget();

		// lag_compensation.RestorePlayer( m_pLocalPlayer );

		auto server_time = prediction.GetServerTime();
		auto angle_distance = std::abs( AngleNormalize( m_angRealAngle[ 1 ] - m_pLocalPlayer->m_flLowerBodyYawTarget() ) );

		if( m_pAnimState->m_flSpeed > 0.1f )
			m_flNextBodyUpdate = ( server_time + 0.22f );
		else if( ( angle_distance > 35.f ) && ( server_time > m_flNextBodyUpdate ) )
			m_flNextBodyUpdate = ( server_time + 1.1f );
	}
}

auto AnimState::GetNextBodyUpdate() -> bool
{
	auto& prediction = engine::Prediction::Instance();

	if( !m_pAnimState || m_flNextBodyUpdate == 0.f || !m_pLocalPlayer || m_pLocalPlayer->m_lifeState() != LIFE_ALIVE )
		return false;

	auto server_time = prediction.GetServerTime();

	if( server_time > m_flNextBodyUpdate )
		return true;

	return false;
}

auto AnimState::GetLowerBody() -> float
{
	return m_flLowerBody;
}

auto AnimState::CreateState( CCSGOPlayerAnimState* state, C_CSPlayer* player ) -> void
{
	using Fn = void( __thiscall* )( CCSGOPlayerAnimState*, C_CSPlayer* );

	static auto fn = memory::scan< Fn >( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" );
	return fn( state, player );
}

auto AnimState::UpdateState( CCSGOPlayerAnimState* state, QAngle angles ) -> void
{
	using Fn = void( __vectorcall* )( void*, void*, float, float, float, void* );

	static auto fn = memory::scan< Fn >( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" );
	fn( state, nullptr, 0.f, angles[ 1 ], angles[ 0 ], nullptr );
}

auto AnimState::ResetState( CCSGOPlayerAnimState* state ) -> void
{
	using Fn = void( __thiscall* )( CCSGOPlayerAnimState* );

	static auto fn = memory::scan< Fn >( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" );
	fn( state );
}

}

}