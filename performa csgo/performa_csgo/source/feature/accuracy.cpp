#include "accuracy.hpp"

#include "../engine.hpp"
#include "../engine/prediction.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

namespace source
{

namespace feature
{

auto Accuracy::TraceLine( const Vector& end, C_BaseEntity* target /*= nullptr*/ ) -> bool
{
	auto& prediction = engine::Prediction::Instance();

	auto player = prediction.GetPlayer();

	CTraceFilterSimple skip( player );

	Ray_t ray = { };
	ray.Init( player->EyePosition(), end );

	trace_t trace = { };
	source::m_engine_trace->TraceRay( ray, 0x46004003, &skip, &trace );

	if( target )
		return ( trace.fraction == 1.f || trace.m_pEnt == target );

	return ( trace.fraction == 1.f );
}

auto Accuracy::CompensateSpread( const QAngle& angles_input, QAngle& angles_output, int random_seed, bool inverse /*= false*/ ) -> void
{
	auto& prediction = engine::Prediction::Instance();

	auto cmd = prediction.GetUserCmd();
	auto weapon = prediction.GetWeapon();

	auto weapon_inaccuracy = weapon->GetInaccuracy();
	auto weapon_spread = weapon->GetSpread();

	RandomSeed( ( random_seed & 255 ) + 1 );

	auto rand1 = RandomFloat( 0.f, 1.f );
	auto rand_pi1 = RandomFloat( 0.f, 2.f * g_pi_radians );
	auto rand2 = RandomFloat( 0.f, 1.f );
	auto rand_pi2 = RandomFloat( 0.f, 2.f * g_pi_radians );

	int id = weapon->m_iItemDefinitionIndex();
	auto recoil_index = weapon->m_flRecoilIndex();

	if( id == WEAPON_REVOLVER )
	{
		if( cmd->buttons & IN_ATTACK2 )
		{
			rand1 = 1.f - rand1 * rand1;
			rand2 = 1.f - rand2 * rand2;
		}
	}
	else if( id == 28 && recoil_index < 3.f )
	{
		for( int i = 3; i > recoil_index; i-- )
		{
			rand1 *= rand1;
			rand2 *= rand2;
		}

		rand1 = 1.f - rand1;
		rand2 = 1.f - rand2;
	}

	auto rand_inaccuracy = rand1 * weapon_inaccuracy;
	auto rand_spread = rand2 * weapon_spread;

	Vector2D spread =
	{
		std::cos( rand_pi1 ) * rand_inaccuracy + std::cos( rand_pi2 ) * rand_spread,
		std::sin( rand_pi1 ) * rand_inaccuracy + std::sin( rand_pi2 ) * rand_spread,
	};

	if( inverse )
		spread.Negate();

	Vector side, up;
	Vector forward = QAngle::Zero.ToVector( &side, &up );

	Vector direction = forward + ( side * spread[ 0 ] ) + ( up * spread[ 1 ] );
	direction.Normalize();

	QAngle angles_spread = direction.ToEulerAngles();

	angles_spread[ 0 ] -= angles_input[ 0 ];
	angles_spread.Normalize();

	forward = angles_spread.ToVectorTranspose( &side, &up );

	angles_spread = forward.ToEulerAngles( &up );
	angles_spread.Normalize();

	angles_spread[ 1 ] += angles_input[ 1 ];
	angles_spread.Normalize();

	angles_output = angles_spread;
	angles_output.Normalize();
}

auto Accuracy::CompensateRecoil( const QAngle& angles_input, QAngle& angles_output, bool inverse /*= false*/ ) -> void
{
	auto& prediction = engine::Prediction::Instance();

	auto cmd = prediction.GetUserCmd();
	auto player = prediction.GetPlayer();
	auto weapon = prediction.GetWeapon();

	auto angles_recoil = player->m_aimPunchAngle() * 2.f;

	if( inverse )
		angles_recoil.Negate();

	angles_output = angles_input - angles_recoil;
}

auto Accuracy::GetDamage( C_CSPlayer* target_player, const Vector& start, const Vector& end ) -> float
{
	static auto cvar_friendlyfire = source::m_cvar->FindVar( "mp_friendlyfire" );

	auto local = C_CSPlayer::GetLocalCSPlayer();
	auto gun = ( C_WeaponCSBase* )local->GetActiveWeapon();
	auto gun_data = gun->GetCSWeaponData();

	CTraceFilterSimple trace_ignore( local );

	Ray_t ray = { };
	ray.Init( start, end );

	trace_t trace = { };
	source::m_engine_trace->TraceRay( ray, CS_MASK_SHOOT, &trace_ignore, &trace );

	auto player = ToCSPlayer( trace.m_pEnt );

	if( !( trace.fraction == 1.f || player == target_player ) )
		return -1.f;

	auto damage = static_cast< float >( gun_data->m_damage );
	auto current_damage = ( damage * std::pow( gun_data->m_range_modifier, ( gun_data->m_range * 0.002f ) ) );

	if( player && player == target_player )
		current_damage = ScaleDamage( player, current_damage, gun_data->m_armor_ratio, trace.hitgroup, cvar_friendlyfire->GetBool(), local->m_iTeamNum() == player->m_iTeamNum() );

	return current_damage;
}

auto Accuracy::GetPenetrationDamage( C_CSPlayer* target_player, const Vector& start, const Vector& end, int* hitgroup /*= nullptr*/ ) -> float
{
	auto local = C_CSPlayer::GetLocalCSPlayer();
	auto gun = ( C_WeaponCSBase* )local->GetActiveWeapon();
	auto gun_data = gun->GetCSWeaponData();

	trace_data_t trace_data = { };

	trace_data.m_target_player = target_player;
	trace_data.m_start = start;
	trace_data.m_direction = ( end - start );
	trace_data.m_direction.Normalize();

	if( !TraceFire( local, gun, trace_data ) )
		return -1.f;

	if( hitgroup )
		*hitgroup = trace_data.m_hitgroup;

	return trace_data.m_current_damage;
}

auto Accuracy::IsArmored( C_CSPlayer* player, int hitgroup ) -> bool
{
	auto armored = false;

	auto has_helmet = player->m_bHasHelmet();
	auto armor_value = static_cast< float >( player->m_ArmorValue() );

	if( armor_value > 0.f )
	{
		switch( hitgroup )
		{
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
			{
				armored = true;
				break;
			}
			case HITGROUP_HEAD:
			{
				armored = has_helmet;
				break;
			}
		}
	}

	return armored;
}

auto Accuracy::ScaleDamage( C_CSPlayer* player, float damage, float weapon_armor_ratio, int hitgroup, bool friendlyfire, bool is_friendly ) -> float
{
	auto armor_heavy = player->m_bHasHeavyArmor();
	auto armor_value = static_cast< float >( player->m_ArmorValue() );

	switch( hitgroup )
	{
		case HITGROUP_HEAD:
		{
			if( armor_heavy )
				damage *= 2.f;
			else
				damage *= 4.f;
			break;
		}
		case HITGROUP_STOMACH:
		{
			damage *= 1.25f;
			break;
		}
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
		{
			damage *= 0.75f;
			break;
		}
	}

	if( IsArmored( player, hitgroup ) )
	{
		auto armor_scale = 1.f;
		auto armor_ratio = ( weapon_armor_ratio * 0.5f );
		auto armor_bonus_ratio = 0.5f;

		if( armor_heavy )
		{
			armor_scale = 0.33f;
			armor_ratio = ( weapon_armor_ratio * 0.5f ) * 0.5f;
			armor_bonus_ratio = 0.33f;
		}

		damage *= armor_ratio;

		if( armor_heavy )
			damage *= 0.85f;

		if( ( ( damage - ( damage * armor_ratio ) ) * ( armor_scale * armor_bonus_ratio ) ) > armor_value )
			damage = ( damage - ( armor_value / armor_bonus_ratio ) );
	}

	return damage;
}

auto Accuracy::TraceToExit( Vector& end, trace_t* enter_trace, Vector start, Vector direction, trace_t* exit_trace ) -> bool
{
	auto distance = 0.f;

	while( distance <= 90.f )
	{
		distance += 4.f;
		end = ( start + direction * distance );

		auto contents = source::m_engine_trace->GetPointContents( end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr );

		if( contents & MASK_SHOT_HULL && !( contents & CONTENTS_HITBOX ) )
			continue;

		auto destination = ( end - ( direction * 4.f ) );

		Ray_t ray = { };
		
		ray.Init( end, destination );
		source::m_engine_trace->TraceRay( ray, MASK_SHOT, nullptr, exit_trace );

		if( exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX )
		{
			CTraceFilterSimple trace_skip( exit_trace->m_pEnt );

			ray.Init( end, start );
			source::m_engine_trace->TraceRay( ray, MASK_SHOT_HULL, nullptr, exit_trace );

			if( ( exit_trace->fraction < 1.f || exit_trace->allsolid ) && !exit_trace->startsolid )
			{
				end = exit_trace->endpos;
				return true;
			}

			continue;
		}

		if( !( exit_trace->fraction < 1.f || exit_trace->allsolid || exit_trace->startsolid ) || exit_trace->startsolid )
		{
			if( exit_trace->m_pEnt )
			{
				if( enter_trace->DidHitNonWorldEntity() && IsBreakableEntity( enter_trace->m_pEnt ) )
					return true;
			}

			continue;
		}

		if( ( ( exit_trace->surface.flags >> 7 ) & 1 ) && !( ( enter_trace->surface.flags >> 7 ) & 1 ) )
			continue;

		if( exit_trace->plane.normal.Dot( direction ) <= 1.f )
		{
			auto fraction = ( exit_trace->fraction * 4.f );
			end = ( end - ( direction * fraction ) );
			return true;
		}
	}

	return false;
}

auto Accuracy::TracePenetration( CCSWeaponInfo* gun_data, trace_data_t& data ) -> bool
{
	auto enter_surface_data = source::m_physics_surface_props->GetSurfaceData( data.m_enter_trace.surface.surfaceProps );
	auto enter_material = enter_surface_data->game.material;

	auto enter_penetration_modifier = enter_surface_data->game.penetration_modifier;

	data.m_trace_length += ( data.m_enter_trace.fraction * data.m_trace_length_remaining );
	data.m_current_damage *= std::pow( gun_data->m_range_modifier, ( data.m_trace_length * 0.002f ) );

	if( ( data.m_trace_length > 3000.f ) || ( enter_penetration_modifier < 0.1f ) )
		data.m_penetrate_count = 0;

	if( data.m_penetrate_count <= 0 )
		return false;

	Vector end = { };
	trace_t exit_trace = { };

	if( !TraceToExit( end, &data.m_enter_trace, data.m_enter_trace.endpos, data.m_direction, &exit_trace ) )
		return false;

	auto exit_surface_data = source::m_physics_surface_props->GetSurfaceData( exit_trace.surface.surfaceProps );
	auto exit_material = exit_surface_data->game.material;

	auto exit_penetration_modifier = exit_surface_data->game.penetration_modifier;

	auto combined_damage_modifier = 0.16f;
	auto combined_penetration_modifier = 0.f;

	if( ( ( data.m_enter_trace.contents & CONTENTS_GRATE ) != 0 ) || ( enter_material == CHAR_TEX_GLASS ) || ( enter_material == CHAR_TEX_GRATE ) )
	{
		combined_penetration_modifier = 3.f;
		combined_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = ( ( enter_penetration_modifier + exit_penetration_modifier ) * 0.5f );
	}

	if( enter_material == exit_material )
	{
		if( exit_material == CHAR_TEX_WOOD || exit_material == 85 )
			combined_penetration_modifier = 3.f;
		else if( exit_material == CHAR_TEX_PLASTIC )
			combined_penetration_modifier = 2.f;
	}

	auto penetration_modifier = std::max( 0.f, 1.f / combined_penetration_modifier );
	auto damage_modifier = ( ( data.m_current_damage * combined_damage_modifier ) + penetration_modifier * 3.f * std::max( 0.f, ( 3.f / gun_data->m_penetration ) * 1.25f ) );
	auto penetration_distance = ( exit_trace.endpos - data.m_enter_trace.endpos ).Length();

	penetration_distance *= penetration_distance;
	penetration_distance *= penetration_modifier;
	penetration_distance /= 24.f;

	auto damage_lost = std::max( 0.f, ( damage_modifier + penetration_distance ) );

	if( damage_lost > data.m_current_damage )
		return false;

	if( damage_lost >= 0.f )
		data.m_current_damage -= damage_lost;

	if( data.m_current_damage < 1.f )
		return false;

	data.m_start = exit_trace.endpos;
	data.m_penetrate_count--;

	return true;
}

auto UTIL_TraceLine( const Vector& start, const Vector& end, unsigned int mask, C_BaseEntity* ignore, trace_t* trace ) -> void
{
	CTraceFilterSimple trace_ignore( ignore );

	Ray_t ray = { };
	ray.Init( start, end );

	source::m_engine_trace->TraceRay( ray, mask, &trace_ignore, trace );
}

auto Accuracy::TraceFire( C_CSPlayer* local, C_WeaponCSBase* gun, trace_data_t& data ) -> bool
{
	static auto cvar_friendlyfire = source::m_cvar->FindVar( "mp_friendlyfire" );

	data.m_penetrate_count = 4;
	data.m_trace_length = 0.f;

	auto gun_data = gun->GetCSWeaponData();

	data.m_current_damage = static_cast< float >( gun_data->m_damage );

	while( ( data.m_penetrate_count > 0 ) && ( data.m_current_damage >= 1.f ) )
	{
		data.m_trace_length_remaining = ( gun_data->m_range - data.m_trace_length );
		auto end = ( data.m_start + data.m_direction * data.m_trace_length_remaining );

		UTIL_TraceLine( data.m_start, end, CS_MASK_SHOOT, local, &data.m_enter_trace );

		if( data.m_enter_trace.fraction == 1.f )
			break;

		data.m_player = ToCSPlayer( data.m_enter_trace.m_pEnt );

		if( data.m_enter_trace.hitgroup != HITGROUP_GEAR && data.m_player && data.m_target_player && data.m_player == data.m_target_player )
		{
			data.m_trace_length += ( data.m_enter_trace.fraction * data.m_trace_length_remaining );
			data.m_current_damage *= std::pow( gun_data->m_range_modifier, ( data.m_trace_length * 0.002f ) );

			data.m_current_damage = ScaleDamage( data.m_player, data.m_current_damage, gun_data->m_armor_ratio, data.m_enter_trace.hitgroup, cvar_friendlyfire->GetBool(), local->m_iTeamNum() == data.m_player->m_iTeamNum() );

			data.m_hitbox = data.m_enter_trace.hitbox;
			data.m_hitgroup = data.m_enter_trace.hitgroup;

			return true;
		}

		if( !TracePenetration( gun_data, data ) )
			break;
	}

	return false;
}

auto Accuracy::IsBreakableEntity( C_BaseEntity* pEnt ) -> bool
{
	using fn_t = bool( __thiscall* )( C_BaseEntity* );
	static auto fn = memory::scan< fn_t >( "client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68" );
	return fn( pEnt );
}

}

}