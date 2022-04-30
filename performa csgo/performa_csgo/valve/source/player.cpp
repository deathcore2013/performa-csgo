#include "player.hpp"

#include "../../option/storage.hpp"

auto C_BasePlayer::GetBasePlayer( int index ) -> C_BasePlayer*
{
	auto base_entity = C_BaseEntity::GetBaseEntity( index );
	return ToBasePlayer( base_entity );
}

auto C_BasePlayer::EyePosition() -> Vector
{
	return ( m_vecOrigin() + m_vecViewOffset() );
}

auto C_BasePlayer::GetSequenceAct( int sequence ) -> int
{
	using Fn = int( __fastcall* )( void*, studiohdr_t*, int );
	static auto fn = memory::scan< Fn >( "client.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74" );

	auto model = GetModel();

	if( !model )
		return -1;

	auto studio = source::m_model_info_client->GetStudiomodel( model );

	if( studio )
		return -1;

	return fn( this, studio, sequence );
}

auto C_BasePlayer::m_flSpawnTime() -> float
{
	static auto offset = 0x00A290;
	return *( float* )( this + offset );
}

auto C_BasePlayer::m_pCurrentCommand()->CUserCmd*&
{
	static auto offset = memory::scan< std::intptr_t >( "client.dll", "89 BE ? ? ? ? E8 ? ? ? ? 85 FF", 2, 1u );
	return *( CUserCmd** )( this + offset );
}


auto C_CSPlayer::GetCSPlayer( int index ) -> C_CSPlayer*
{
	auto base_entity = C_BaseEntity::GetBaseEntity( index );
	return ToCSPlayer( base_entity );
}

auto C_CSPlayer::GetLocalCSPlayer() -> C_CSPlayer*
{
	auto index = source::m_engine_client->GetLocalPlayer();
	return GetCSPlayer( index );
}

auto C_CSPlayer::IsLocal() -> bool
{
	if( !IsGood() )
		return false;

	return ( entindex() == source::m_engine_client->GetLocalPlayer() );
}

auto C_CSPlayer::IsGood() -> bool
{
	return ( this && m_lifeState() == LIFE_ALIVE && !IsDormant() );
}

auto C_CSPlayer::GetSpotPointArray( int spot, matrix3x4_t* transform, std::vector< Vector >& output ) -> bool
{
	auto& aimbot = option::m_current.m_aimbot;

	output.clear();

	auto model = GetModel();

	if( model )
	{
		auto studio = source::m_model_info_client->GetStudiomodel( model );

		if( studio )
		{
			auto set = studio->pHitboxSet( m_nHitboxSet() );

			if( set )
			{
				auto box = set->pHitbox( spot );

				if( box )
				{
					auto bbmin = box->bbmin;
					auto bbmax = box->bbmax;

					if( box->radius != -1.f )
					{
						bbmin -= Vector{ box->radius, box->radius, box->radius };
						bbmax += Vector{ box->radius, box->radius, box->radius };
					}

					auto mins = box->bbmin.Transform( transform[ box->bone ] );
					auto maxs = box->bbmax.Transform( transform[ box->bone ] );

					auto center = ( ( mins + maxs ) * 0.5f );
					auto eye_angles = m_angEyeAngles();

					if( spot == 0 && eye_angles[ 0 ] >= 66.f )
					{
						CTraceFilterSimple trace_ignore( this );

						Ray_t ray = { };
						ray.Init( center + Vector{ 0.f, 0.f, 15.f }, center );

						trace_t trace = { };
						source::m_engine_trace->TraceRay( ray, MASK_SHOT, &trace_ignore, &trace );

						if( ( trace.endpos[ 2 ] - center[ 2 ] ) > 1.f && ( trace.endpos[ 2 ] - center[ 2 ] ) < 6.f && ( trace.fraction == 1.f || trace.m_pEnt == this ) )
						{
							center = trace.endpos;
							center[ 2 ] -= 0.5f;
						}
						else
						{
							constexpr auto adjust = ( 5.3f / 89.f );
							center[ 2 ] += ( eye_angles[ 0 ] * adjust );
						}
					}

					output.emplace_back( center );

					auto scale = ( ( box->radius / 2.f ) * aimbot.m_target_hitscan_scale );

					auto top = Vector{ center[ 0 ], center[ 1 ], center[ 2 ] + scale };
					output.emplace_back( Vector{ top[ 0 ] + scale, top[ 1 ] + scale, top[ 2 ] } );
					output.emplace_back( Vector{ top[ 0 ] + scale, top[ 1 ] - scale, top[ 2 ] } );
					output.emplace_back( Vector{ top[ 0 ] - scale, top[ 1 ] + scale, top[ 2 ] } );
					output.emplace_back( Vector{ top[ 0 ] - scale, top[ 1 ] - scale, top[ 2 ] } );

					auto bot = Vector{ center[ 0 ], center[ 1 ], center[ 2 ] - scale };
					output.emplace_back( Vector{ bot[ 0 ] + scale, bot[ 1 ] + scale, bot[ 2 ] } );
					output.emplace_back( Vector{ bot[ 0 ] + scale, bot[ 1 ] - scale, bot[ 2 ] } );
					output.emplace_back( Vector{ bot[ 0 ] - scale, bot[ 1 ] + scale, bot[ 2 ] } );
					output.emplace_back( Vector{ bot[ 0 ] - scale, bot[ 1 ] - scale, bot[ 2 ] } );

					output.emplace_back( Vector{ center[ 0 ], center[ 1 ], center[ 2 ] + scale } );
					output.emplace_back( Vector{ center[ 0 ] + scale, center[ 1 ], center[ 2 ] } );
					output.emplace_back( Vector{ center[ 0 ] - scale, center[ 1 ], center[ 2 ] } );
					output.emplace_back( Vector{ center[ 0 ], center[ 1 ] + scale, center[ 2 ] } );
					output.emplace_back( Vector{ center[ 0 ], center[ 1 ] - scale, center[ 2 ] } );
				}
			}
		}
	}

	return !output.empty();
}