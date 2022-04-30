#include "player_array.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace engine
{

auto PlayerArray::FrameStageNotify( ClientFrameStage_t stage ) -> void
{
	if( stage != FRAME_NET_UPDATE_END )
		return;

	for( auto index = 1; index <= source::m_engine_client->GetMaxClients(); index++ )
	{
		auto player = C_CSPlayer::GetCSPlayer( index );

		Update( player );
	}
}

auto PlayerArray::Update( C_CSPlayer* player ) -> void
{
	if( !player )
		return;

	auto index = ( player->entindex() - 1 );
	auto data = &m_player_data[ index ];

	data->m_player = player;
	
	data->m_dormant = player->IsDormant();
	data->m_dead = ( player->m_lifeState() != LIFE_ALIVE );
	data->m_defusing = player->m_bIsDefusing();
	data->m_has_helmet = player->m_bHasHelmet();

	data->m_team = player->m_iTeamNum();
	data->m_hitbox_set = player->m_nHitboxSet();
	data->m_sequence = player->m_nSequence();
	data->m_flags = player->m_fFlags();
	data->m_command_base = player->m_nTickBase();
	data->m_health = player->m_iHealth();
	data->m_armor = player->m_ArmorValue();
	data->m_shots_fired = player->m_iShotsFired();

	data->m_simulation_time = player->m_flSimulationTime();
	data->m_cycle = player->m_flCycle();

	for( auto i = 0; i < 24; i++ )
		data->m_pose_parameter[ i ] = player->m_flPoseParameter()[ i ];

	data->m_flash_alpha = player->m_flFlashMaxAlpha();
	data->m_lower_body_yaw = player->m_flLowerBodyYawTarget();

	data->m_origin = player->m_vecOrigin();
	data->m_velocity = player->m_vecVelocity();

	data->m_recoil_angles = player->m_aimPunchAngle();
	data->m_view_recoil_angles = player->m_viewPunchAngle();
	data->m_eye_angles = player->m_angEyeAngles();

	data->m_coordinate_frame = player->m_rgflCoordinateFrame();
	player->GetBoneTransform( data->m_bone_transform );
}

}

}