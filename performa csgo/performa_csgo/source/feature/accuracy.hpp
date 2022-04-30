#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

class Prediction;

}

namespace feature
{

struct trace_data_t
{
	Vector m_start = { };
	trace_t m_enter_trace = { };
	Vector m_direction = { };
	float m_trace_length = 0.f;
	float m_trace_length_remaining = 0.f;
	float m_current_damage = 0.f;
	int m_penetrate_count = 0;
	C_CSPlayer* m_player = nullptr;
	C_CSPlayer* m_target_player = nullptr;
	int m_hitbox = 0;
	int m_hitgroup = 0;
	CTraceFilterSimple m_trace_skip = { };
};

struct BulletData
{
	Vector				src;
	trace_t				enter_trace;
	Vector				direction;
	CTraceFilterSimple	trace;
	float				trace_length;
	float				trace_length_remaining;
	float				current_damage;
	int					penetrate_count;
	C_BaseEntity*		hit;
	int					hitbox;
	int					hitgroup;
	int					team;
};

class Accuracy : public core::Singleton< Accuracy >
{
public:
	auto TraceLine( const Vector& end, C_BaseEntity* target = nullptr ) -> bool;

	auto CompensateSpread( const QAngle& angles_input, QAngle& angles_output, int random_seed, bool inverse = false ) -> void;
	auto CompensateRecoil( const QAngle& angles_input, QAngle& angles_output, bool inverse = false ) -> void;

	auto GetDamage( C_CSPlayer* target_player, const Vector& start, const Vector& end ) -> float;
	auto GetPenetrationDamage( C_CSPlayer* target_player, const Vector& start, const Vector& end, int* hitgroup = nullptr ) -> float;

private:
	auto IsArmored( C_CSPlayer* player, int hitgroup ) -> bool;
	auto ScaleDamage( C_CSPlayer* player, float damage, float armor_ratio, int hitgroup, bool friendlyfire, bool is_friendly ) -> float;

	auto TraceToExit( Vector& end, trace_t* enter_trace, Vector start, Vector direction, trace_t* exit_trace ) -> bool;
	auto TracePenetration( CCSWeaponInfo* gun_data, trace_data_t& data ) -> bool;
	auto TraceFire( C_CSPlayer* local, C_WeaponCSBase* gun, trace_data_t& data ) -> bool;

	auto IsBreakableEntity( C_BaseEntity* pEnt ) -> bool;
};

}

}