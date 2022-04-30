#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

struct PlayerData
{
	C_CSPlayer* m_player = nullptr;

	bool m_dormant = true;
	bool m_dead = true;
	bool m_defusing = false;
	bool m_has_helmet = false;

	int m_team = 0;
	int m_hitbox_set = 0;
	int m_sequence = 0;
	int m_flags = 0;
	int m_command_base = 0;
	int m_health = 0;
	int m_armor = 0;
	int m_shots_fired = 0;

	float m_simulation_time = 0.f;
	float m_cycle = 0.f;
	float m_pose_parameter[ 24 ] = { };
	float m_flash_alpha = 0.f;
	float m_lower_body_yaw = 0.f;

	Vector m_origin = { };
	Vector m_velocity = { };

	QAngle m_recoil_angles = { };
	QAngle m_view_recoil_angles = { };
	QAngle m_eye_angles = { };

	matrix3x4_t m_coordinate_frame = { };
	matrix3x4_t m_bone_transform[ 128 ] = { };
};

class PlayerArray
{
public:
	auto FrameStageNotify( ClientFrameStage_t stage ) -> void;

private:
	auto Update( C_CSPlayer* player ) -> void;

private:
	PlayerData m_player_data[ 64 ] = { };
};

}

}