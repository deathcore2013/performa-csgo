#include "entity.hpp"

auto C_BaseEntity::GetBaseEntity( int index ) -> C_BaseEntity*
{
	auto client_entity = source::m_client_entity_list->GetClientEntity( index );
	return ( client_entity ? client_entity->GetBaseEntity() : nullptr );
}

auto C_BaseEntity::SetPredictionSeed( CUserCmd* cmd ) -> void
{
	static auto m_pPredictionRandomSeed = memory::scan< int* >( "client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04", 2, 1u );

	if( cmd )
		*m_pPredictionRandomSeed = cmd->random_seed;
	else
		*m_pPredictionRandomSeed = -1;
}

auto C_BaseEntity::SetAbsOrigin( const Vector& origin ) -> void
{
	using Fn = void( __thiscall* )( void*, const Vector& );

	static auto fn = memory::scan< Fn >( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" );
	return fn( this, origin );
}

auto C_BaseEntity::SetAbsAngles( const QAngle& angles ) -> void
{
	using Fn = void( __thiscall* )( void*, const QAngle& );

	static auto fn = memory::scan< Fn >( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" );
	return fn( this, angles );
}


auto C_BaseAnimating::GetBoneTransform( matrix3x4_t* output, float time /*= 0.f*/ ) -> bool
{
	*( int* )( this + 0x0A28 ) = 0;
	*( int* )( this + 0x0A30 ) = source::m_globals->framecount;

	return SetupBones( output, 128, 0x0100, time );
}

auto C_BaseAnimating::GetBoneWorld( int index, matrix3x4_t* transform, Vector& output ) -> bool
{
	if( transform )
	{
		output[ 0 ] = transform[ index ][ 0 ][ 3 ];
		output[ 1 ] = transform[ index ][ 1 ][ 3 ];
		output[ 2 ] = transform[ index ][ 2 ][ 3 ];
	}

	return !output.IsZero();
}

auto C_BaseAnimating::GetBoxBoundWorld( int index, matrix3x4_t* transform, Vector& min, Vector& max ) -> bool
{
	if( transform )
	{
		auto model = GetModel();

		if( model )
		{
			auto studio = source::m_model_info_client->GetStudiomodel( model );

			if( studio )
			{
				auto box = studio->pHitbox( index, m_nHitboxSet() );

				if( box )
				{
					min = box->bbmin.Transform( transform[ box->bone ] );
					max = box->bbmax.Transform( transform[ box->bone ] );
				}
			}
		}
	}

	return ( !min.IsZero() && !max.IsZero() );
}

auto C_BaseAnimating::GetBoxWorld( int index, matrix3x4_t* transform, Vector& output ) -> bool
{
	Vector min = { };
	Vector max = { };

	if( GetBoxBoundWorld( index, transform, min, max ) )
		output = ( min + max ) * 0.5f;

	return !output.IsZero();
}

auto C_BaseAnimating::GetNumAnimOverlays() -> int
{
	auto& anim_overlay = m_AnimOverlay();
	return anim_overlay.m_Size;
}

auto C_BaseAnimating::GetAnimOverlay( int i ) -> C_AnimationLayer*
{
	auto& anim_overlay = m_AnimOverlay();
	return &anim_overlay.m_Memory.m_pMemory[ i ];
}

auto C_BaseAnimating::InvalidateBoneCache() -> void
{
	static std::uintptr_t invalidate_bone_cache = 0u;
	
	if( !invalidate_bone_cache )
	{
		invalidate_bone_cache = memory::scan< std::uintptr_t >( "client.dll", "80 3D ? ? ? ? 00 74 16 A1" );

		if( !invalidate_bone_cache )
			return;
	}

	auto model_bone_counter = **( unsigned long** )( invalidate_bone_cache + 0x000A );
	
	*( unsigned int* )( this + 0x002914 ) = 0xFF7FFFFFu;
	*( unsigned int* )( this + 0x002680 ) = ( model_bone_counter - 1u );
}

auto C_BaseAnimating::m_AnimOverlay() -> CUtlVector< C_AnimationLayer >&
{
	return *( CUtlVector< C_AnimationLayer >* )( this + 0x002970 );
}