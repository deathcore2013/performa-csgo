#pragma once

#include "../sdk.hpp"

#include "../../source/engine.hpp"
#include "../../source/engine/netprop_manager.hpp"

#pragma region region_index
namespace index
{
namespace IHandleEntity
{
constexpr auto SetRefEHandle = 1;
constexpr auto GetRefEHandle = 2;
}
namespace IClientUnknown
{
constexpr auto GetCollideable = 3;
constexpr auto GetClientNetworkable = 4;
constexpr auto GetClientRenderable = 5;
constexpr auto GetIClientEntity = 6;
constexpr auto GetBaseEntity = 7;
}
namespace ICollideable
{
constexpr auto OBBMins = 1;
constexpr auto OBBMaxs = 2;
}
namespace IClientNetworkable
{
constexpr auto GetClientClass = 2;
constexpr auto IsDormant = 9;
constexpr auto entindex = 10;
}
namespace IClientRenderable
{
constexpr auto GetModel = 8;
constexpr auto DrawModel = 9;
constexpr auto SetupBones = 13;
}
namespace IClientEntity
{
constexpr auto GetAbsOrigin = 10;
constexpr auto GetAbsAngles = 11;
}
namespace C_BaseEntity
{
constexpr auto IsPlayer = 152;
}
namespace C_BaseAnimating
{
constexpr auto UpdateClientSideAnimation = 218;
}
}
#pragma endregion

#pragma region region_netprop
#define horizon_netprop( type, name, array_name, prop_name ) \
auto name ## () const -> type& \
{ \
	static auto offset = source::engine::NetPropManager::Instance().Get( array_name, prop_name ); \
	return *( type* )( this + offset ); \
}

#define horizon_netprop_raw( type, name, array_name, prop_name ) \
auto name ## () const -> type \
{ \
	static auto offset = source::engine::NetPropManager::Instance().Get( array_name, prop_name ); \
	return ( type )( this + offset ); \
}

#define horizon_netprop_ex( type, name, array_name, prop_name, extra ) \
auto name ## () const -> type& \
{ \
	static auto offset = source::engine::NetPropManager::Instance().Get( array_name, prop_name ) + extra; \
	return *( type* )( this + offset ); \
}
#pragma endregion

class IHandleEntity
{
public:
	auto SetRefEHandle( const CBaseHandle& handle ) -> void
	{
		using Fn = void( __thiscall* )( void*, const CBaseHandle& );
		return memory::vget< Fn >( this, index::IHandleEntity::SetRefEHandle )( this, handle );
	}

	auto GetRefEHandle() -> const CBaseHandle&
	{
		using Fn = const CBaseHandle&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IHandleEntity::GetRefEHandle )( this );
	}
};

class IClientUnknown : public IHandleEntity
{
public:
	auto GetCollideable() -> ICollideable*
	{
		using Fn = ICollideable*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientUnknown::GetCollideable )( this );
	}

	auto GetClientNetworkable() -> IClientNetworkable*
	{
		using Fn = IClientNetworkable*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientUnknown::GetClientNetworkable )( this );
	}

	auto GetClientRenderable() -> IClientRenderable*
	{
		using Fn = IClientRenderable*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientUnknown::GetClientRenderable )( this );
	}

	auto GetIClientEntity() -> IClientEntity*
	{
		using Fn = IClientEntity*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientUnknown::GetIClientEntity )( this );
	}

	auto GetBaseEntity() -> C_BaseEntity*
	{
		using Fn = C_BaseEntity*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientUnknown::GetBaseEntity )( this );
	}
};

class ICollideable
{
public:
	auto OBBMins() -> const Vector&
	{
		using Fn = const Vector&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::ICollideable::OBBMins )( this );
	}

	auto OBBMaxs() -> const Vector&
	{
		using Fn = const Vector&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::ICollideable::OBBMaxs )( this );
	}
};

class IClientNetworkable
{
public:
	auto GetClientClass() -> ClientClass*
	{
		using Fn = ClientClass*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientNetworkable::GetClientClass )( this );
	}

	auto IsDormant() -> bool
	{
		using Fn = bool( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientNetworkable::IsDormant )( this );
	}

	auto entindex() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientNetworkable::entindex )( this );
	}
};

class IClientRenderable
{
public:
	auto GetModel() -> const model_t*
	{
		using Fn = const model_t*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientRenderable::GetModel )( this );
	}

	auto DrawModel( int flags, const int& instasnce ) -> int
	{
		using Fn = int( __thiscall* )( void*, int, const int& );
		return memory::vget< Fn >( this, index::IClientRenderable::DrawModel )( this, flags, instasnce );
	}

	auto SetupBones( matrix3x4_t* pBoneToWorld, int nMaxBones, int boneMask, float currentTime ) -> bool
	{
		using Fn = bool( __thiscall* )( void*, matrix3x4_t*, int, int, float );
		return memory::vget< Fn >( this, index::IClientRenderable::SetupBones )( this, pBoneToWorld, nMaxBones, boneMask, currentTime );
	}
};

class IClientEntity : public IClientUnknown
{
public:
	auto OBBMins() -> const Vector&
	{
		return GetCollideable()->OBBMins();
	}

	auto OBBMaxs() -> const Vector&
	{
		return GetCollideable()->OBBMaxs();
	}

public:
	auto GetClientClass() -> ClientClass*
	{
		return GetClientNetworkable()->GetClientClass();
	}

	auto IsDormant() -> bool
	{
		return GetClientNetworkable()->IsDormant();
	}

	auto entindex() -> int
	{
		return GetClientNetworkable()->entindex();
	}

public:
	auto GetModel() -> const model_t*
	{
		return GetClientRenderable()->GetModel();
	}

	auto DrawModel( int flags, const int& instance ) -> int
	{
		return GetClientRenderable()->DrawModel( flags, instance );
	}

	auto SetupBones( matrix3x4_t* pBoneToWorld, int nMaxBones, int boneMask, float currentTime ) -> bool
	{
		return GetClientRenderable()->SetupBones( pBoneToWorld, nMaxBones, boneMask, currentTime );
	}

public:
	auto GetAbsOrigin() -> const Vector&
	{
		using Fn = const Vector&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientEntity::GetAbsOrigin )( this );
	}

	auto GetAbsAngles() -> const QAngle&
	{
		using Fn = const QAngle&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientEntity::GetAbsAngles )( this );
	}
};

class C_BaseEntity : public IClientEntity
{
public:
	static auto GetBaseEntity( int index ) -> C_BaseEntity*;

	static auto SetPredictionSeed( CUserCmd* cmd ) -> void;

public:
	auto IsPlayer() -> bool
	{
		using Fn = bool( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::C_BaseEntity::IsPlayer )( this );
	}

	auto SetAbsOrigin( const Vector& origin ) -> void;
	auto SetAbsAngles( const QAngle& angles ) -> void;

public:
	horizon_netprop( int, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum" );
	horizon_netprop( float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime" );
	horizon_netprop( CBaseHandle, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity" );
	horizon_netprop( Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin" );

public:
	horizon_netprop_ex( const matrix3x4_t, m_rgflCoordinateFrame, "DT_BaseEntity", "m_CollisionGroup", -0x0030 );
};

class C_BaseAnimating : public C_BaseEntity
{
public:
	auto GetBoneTransform( matrix3x4_t* output, float time = 0.f ) -> bool;
	auto GetBoneWorld( int index, matrix3x4_t* transform, Vector& output ) -> bool;
	auto GetBoxBoundWorld( int index, matrix3x4_t* transform, Vector& min, Vector& max ) -> bool;
	auto GetBoxWorld( int index, matrix3x4_t* transform, Vector& output ) -> bool;

	auto GetNumAnimOverlays() -> int;
	auto GetAnimOverlay( int i ) -> C_AnimationLayer*;

public:
	auto UpdateClientSideAnimation() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::C_BaseAnimating::UpdateClientSideAnimation )( this );
	}

public:
	auto InvalidateBoneCache() -> void;

	auto m_AnimOverlay() -> CUtlVector< C_AnimationLayer >&;

public:
	horizon_netprop( int, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet" );
	horizon_netprop( int, m_nSequence, "DT_BaseAnimating", "m_nSequence" );
	horizon_netprop( float, m_flCycle, "DT_BaseAnimating", "m_flCycle" );
	horizon_netprop_raw( float*, m_flPoseParameter, "DT_BaseAnimating", "m_flPoseParameter" );
};

class C_BaseCombatCharacter : public C_BaseAnimating
{
public:
	auto GetActiveWeapon() -> C_BaseCombatWeapon*
	{
		auto handle = m_hActiveWeapon();
		return reinterpret_cast< C_BaseCombatWeapon* >( source::m_client_entity_list->GetClientEntityFromHandle( handle ) );
	}

public:
	horizon_netprop( CBaseHandle, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon" );
	horizon_netprop( float, m_flNextAttack, "DT_BaseCombatCharacter", "m_flNextAttack" );
};