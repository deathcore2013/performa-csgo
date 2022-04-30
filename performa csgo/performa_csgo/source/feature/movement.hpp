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

class Movement : public core::Singleton< Movement >
{
public:
	auto Begin( engine::Prediction& prediction, bool& send_packet ) -> bool;
	auto End() -> void;

	auto FrameStageNotify( ClientFrameStage_t stage ) -> void;

	auto GetRealAngles() const -> const QAngle&;
	auto GetFakeAngles() const -> const QAngle&;
	auto GetLowerBodyAngles() const -> const QAngle&;

	auto IsLowerBodyUpdate() const -> bool;

private:
	auto AnglesToPixels( const QAngle& angles_begin, const QAngle& angles_end ) -> QAngle;
	auto PixelsDistanceToAnglesDistance( const QAngle& pixels_distance ) -> QAngle;

private:
	bool* m_send_packet = nullptr;

	CUserCmd* m_cmd = nullptr;
	C_CSPlayer* m_player = nullptr;
	C_WeaponCSBase* m_weapon = nullptr;

	int m_flags = 0;

	bool m_jump_first = false;
	bool m_jump_fake = false;

	QAngle m_angles = { };
	QAngle m_angles_render = { };
	QAngle m_angles_previous = { };

	QAngle m_angles_fake = { };
	QAngle m_angles_real = { };
	QAngle m_angles_lower_body = { };

	
	// m_flLowerBodyYawTarget break
	bool m_bLowerBodyUpdate = false;

	QAngle m_angEyeAngles = { };
	float m_flLowerBody = 0.f;

	float m_flLowerBodyNextUpdate = 0.f;
	float m_flLowerBodyLastUpdate = 0.f;
	//


	ConVar* m_cvar_sensitivity = nullptr;
	ConVar* m_cvar_m_pitch = nullptr;
	ConVar* m_cvar_m_yaw = nullptr;
};

}

}