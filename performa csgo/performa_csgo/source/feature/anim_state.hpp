#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace feature
{

class CCSGOPlayerAnimState
{
public:
	horizon_pad( 0x003C );				// 0x0000
	float m_flSpeed = 0.f;				// 0x003C
	horizon_pad( 0x0040 );				// 0x0040
	float m_flGoalFeetYaw = 0.f;		// 0x0080
	float m_flCurrentFeetYaw = 0.f;		// 0x0084
	float m_flCurrentTorsoYaw = 0.f;	// 0x0088
};

class AnimState : public core::Singleton< AnimState >
{
public:
	AnimState() = default;

public:
	auto UpdateServerAnimation( const QAngle& angles ) -> void;
	auto GetNextBodyUpdate() -> bool;
	auto GetLowerBody() -> float;

private:
	auto CreateState( CCSGOPlayerAnimState* state, C_CSPlayer* player ) -> void;
	auto UpdateState( CCSGOPlayerAnimState* state, QAngle angles ) -> void;
	auto ResetState( CCSGOPlayerAnimState* state ) -> void;

private:
	CCSGOPlayerAnimState* m_pAnimState = nullptr;
	CBaseHandle m_uEntHandle = 0u;
	float m_flSpawnTime = 0.f;
	QAngle m_angRealAngle = { };
	float m_flNextBodyUpdate = 0.f;
	float m_flLowerBody = 0.f;
	C_CSPlayer* m_pLocalPlayer = nullptr;
};

}

}