#pragma once

#include "../../valve/sdk.hpp"

#define MAX_LAYER_RECORDS 15
#define MAX_PLAYERS 64

namespace source
{

namespace engine
{

class Prediction;

}

namespace feature
{

struct LayerRecord
{
	int m_nOrder = 0;
	int m_nSequence = 0;

	float m_flCycle = 0.f;
	float m_flWeight = 0.f;


	LayerRecord() = default;

	LayerRecord( const LayerRecord& record )
	{
		m_nOrder = record.m_nOrder;
		m_nSequence = record.m_nSequence;

		m_flCycle = record.m_flCycle;
		m_flWeight = record.m_flWeight;
	}
};

struct LagRecord
{
	int m_nSequence = 0;
	float m_flCycle = 0.f;

	float m_flSimulationTime = 0.f;
	float m_flLowerBodyYawTarget = 0.f;

	float m_flPoseParameter[ 24 ] = { };

	Vector m_vecOrigin = { };
	QAngle m_angEyeAngles = { };

	LayerRecord m_LayerRecord[ MAX_LAYER_RECORDS ] = { };

	
	LagRecord() = default;

	LagRecord( const LagRecord& record )
	{
		m_nSequence = record.m_nSequence;
		m_flCycle = record.m_flCycle;

		m_flSimulationTime = record.m_flSimulationTime;
		m_flLowerBodyYawTarget = record.m_flLowerBodyYawTarget;

		std::memcpy( m_flPoseParameter, record.m_flPoseParameter, sizeof( m_flPoseParameter ) );

		m_vecOrigin = record.m_vecOrigin;
		m_angEyeAngles = record.m_angEyeAngles;

		std::memcpy( m_LayerRecord, record.m_LayerRecord, sizeof( m_LayerRecord ) );
	}
};

class VarMapEntry_t
{
public:
	unsigned short type = 0u;
	unsigned short m_bNeedsToInterpolate = 0u;

	void* data = nullptr;
	void* watcher = nullptr;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries = nullptr;
	int m_nInterpolatedEntries = 0;
	float m_lastInterpolationTime = 0.f;
};

class LagCompensation : public core::Singleton< LagCompensation >
{
public:
	auto FrameStageNotify( ClientFrameStage_t stage ) -> void;

	auto IsRecordOutOfBounds( const LagRecord& record ) -> bool;

	auto StartLagCompensation( C_CSPlayer* player, LagRecord& record ) -> bool;
	auto BacktrackPlayer( C_CSPlayer* player, LagRecord& record ) -> void;
	auto FinishLagCompensation( C_CSPlayer* player ) -> void;

	auto GetCorrectCommand( C_CSPlayer* player, bool backtrack = false ) -> int;

	auto ClearHistory() -> void;

public:
	auto BackupPlayer( C_CSPlayer* player ) -> void;
	auto RestorePlayer( C_CSPlayer* player ) -> void;

private:
	auto GetRecordCommand( C_CSPlayer* player, LagRecord& record ) -> int;

	auto GetVarMap( void* base ) -> VarMapping_t*
	{
		return reinterpret_cast< VarMapping_t* >( reinterpret_cast< std::uintptr_t >( base ) + 0x0024 );
	}

	auto DisableInterpolation( C_BaseEntity* base ) -> void
	{
		auto map = GetVarMap( base );

		if( !map )
			return;

		for( auto i = 0; i < map->m_nInterpolatedEntries; i++ )
		{
			auto e = &map->m_Entries[ i ];
			e->m_bNeedsToInterpolate = 0u;
		}
	}

private:
	auto GetLerpTime() -> float;

public:
	C_CSPlayer* m_player = nullptr;
	std::deque< LagRecord > m_lag_record[ MAX_PLAYERS ] = { };
	LagRecord m_restore_record[ MAX_PLAYERS ] = { };
};

}

}