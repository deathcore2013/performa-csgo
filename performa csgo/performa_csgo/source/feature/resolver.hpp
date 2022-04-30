#pragma once

#include "../../valve/sdk.hpp"

#define MAX_LAYER_RECORDS 15
#define MAX_PLAYERS 64

struct RenderData
{
	float m_flSimulationTime = 0.f;
	float m_flLowerBodyYawTarget = 0.f;

	QAngle m_angEyeAngles = { };

	C_AnimationLayer m_AnimationLayers[ MAX_LAYER_RECORDS ] = { };
};

extern RenderData g_render_data;

namespace source
{

namespace engine
{

class Prediction;

}

namespace feature
{

struct LayerData
{
	int m_nAct = 0;
	int m_nSequence = 0;
	int m_nOrder = 0;

	float m_flCycle = 0.f;
	float m_flPrevCycle = 0.f;
	float m_flWeight = 0.f;
	float m_flWeightDeltaRate = 0.f;
};

struct PlayerResolveData
{
	bool m_bFakeAngles = false;
	bool m_bFakeWalking = false;

	bool m_bHasDistance = false;

	float m_flDistance = 0.f;

	bool m_bLowerBodyUpdate = false;

	float m_flLowerBodyLastUpdate = 0.f;
	float m_flLowerBodyNextUpdate = 0.f;

	int m_nShotsFired = 0;
	int m_nShotsHit = 0;
	int m_nShotsWrong = 0;

	float m_flSimulationTime = 0.f;

	float m_flFakeWalkDetect[ 2 ] = { };
	float m_flFakeWalkDetectTime[ 2 ] = { };

	int m_nResetWalk = 0;
	int m_nResetWalkNext = 0;

	float m_flFakeBruteYaw = 0.f;

	float m_flLowerBodyYaw = 0.f;
	float m_flLowerBodyYawTime = 0.f;

	float m_flLastLowerBodyYaw = 0.f;
	float m_flLastMoveLowerBodyYaw = 0.f;

	Vector m_vecVelocity = { };

	QAngle m_angRender = { };
	QAngle m_angAngles = { };
	QAngle m_angAdjust = { };

	LayerData m_LayerData[ MAX_LAYER_RECORDS ] = { };
};

class Resolver : public core::Singleton< Resolver >
{
public:
	auto FrameStageNotify( ClientFrameStage_t stage ) -> void;
	auto FireGameEvent( IGameEvent* game_event ) -> void;

	auto Correct( C_CSPlayer* player, bool frame_render_start = false ) -> void;
	auto GetPlayerData( C_CSPlayer* player )->PlayerResolveData&;
	auto ResetPlayerData( C_CSPlayer* player ) -> void;

	auto GetLowerBodyUpdate(C_CSPlayer* player) -> float;

private:
	auto OnPlayerHurt( C_CSPlayer* player, bool is_headshot ) -> void;
	auto OnBulletImpact() -> void;

public:
	CUserCmd* m_cmd = nullptr;
	C_CSPlayer* m_player = nullptr;
	PlayerResolveData m_player_resolve_data[ MAX_PLAYERS ] = { };
	float m_flLowerBodyUpdate[ MAX_PLAYERS ] = { };
};

}

}