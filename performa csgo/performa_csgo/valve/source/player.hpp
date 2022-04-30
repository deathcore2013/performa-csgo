#pragma once

#include "entity.hpp"

inline auto ToBasePlayer( C_BaseEntity* base_entity ) -> C_BasePlayer*
{
	if( !base_entity || !base_entity->IsPlayer() )
		return nullptr;

	return reinterpret_cast< C_BasePlayer* >( base_entity );
}

inline auto ToCSPlayer( C_BaseEntity* base_entity ) -> C_CSPlayer*
{
	return reinterpret_cast< C_CSPlayer* >( ToBasePlayer( base_entity ) );
}


class C_BasePlayer : public C_BaseCombatCharacter
{
public:
	static auto GetBasePlayer( int index ) -> C_BasePlayer*;

public:
	auto EyePosition() -> Vector;
	auto GetSequenceAct( int sequence ) -> int;

	auto m_flSpawnTime() -> float;
	auto m_pCurrentCommand() -> CUserCmd*&;

public:
	horizon_netprop( char, m_lifeState, "DT_BasePlayer", "m_lifeState" );
	horizon_netprop( int, m_fFlags, "DT_BasePlayer", "m_fFlags" );
	horizon_netprop( int, m_iHealth, "DT_BasePlayer", "m_iHealth" );
	horizon_netprop( int, m_nTickBase, "DT_BasePlayer", "m_nTickBase" );
	horizon_netprop( Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]" );
	horizon_netprop( Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]" );
	horizon_netprop( QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle" );
	horizon_netprop( QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle" );
	horizon_netprop_ex( QAngle, v_angle, "DT_BasePlayer", "deadflag", 0x04 );
};

class C_CSPlayer : public C_BasePlayer
{
public:
	static auto GetCSPlayer( int index ) -> C_CSPlayer*;
	static auto GetLocalCSPlayer() -> C_CSPlayer*;

public:
	auto IsLocal() -> bool;
	auto IsGood() -> bool;

	auto GetSpotPointArray( int spot, matrix3x4_t* transform, std::vector< Vector >& output ) -> bool;

public:
	horizon_netprop( bool, m_bIsDefusing, "DT_CSPlayer", "m_bIsDefusing" );
	horizon_netprop( bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet" );
	horizon_netprop( bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor" );
	horizon_netprop( int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue" );
	horizon_netprop( int, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired" );
	horizon_netprop( float, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha" );
	horizon_netprop( float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget" );
	horizon_netprop( QAngle, m_angEyeAngles , "DT_CSPlayer", "m_angEyeAngles[0]" );
};