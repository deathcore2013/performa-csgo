#pragma once

#include "entity.hpp"

namespace index
{
namespace C_WeaponCSBase
{
constexpr auto UpdateAccuracyPenalty = 470;
constexpr auto GetInaccuracy = 469;
constexpr auto GetSpread = 439;
}
}

class CCSWeaponInfo
{
public:
	horizon_pad( 0x0088 );
	const char* m_hud_name = nullptr;
	const char* m_weapon_name = nullptr;
	// horizon_pad( 0x00C8 );
	horizon_pad( 0x0038 );
	CSWeaponType m_type = WEAPONTYPE_UNKNOWN;
	horizon_pad( 0x0020 );
	int m_damage = 0;
	float m_armor_ratio = 0.f;
	int m_bullets = 0;
	float m_penetration = 0.f;
	horizon_pad( 0x0008 );
	float m_range = 0.f;
	float m_range_modifier = 0.f;
	horizon_pad( 0x0030 );
	float m_inaccuracy_crouch = 0.f;
	horizon_pad( 0x0030 );
	float m_inaccuracy_move = 0.f;
};

class C_BaseCombatWeapon : public C_BaseEntity
{
public:
	horizon_netprop( int, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1" );
	horizon_netprop( int, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2" );
	horizon_netprop( int, m_iPrimaryAmmoType, "DT_BaseCombatWeapon", "m_iPrimaryAmmoType" );
	horizon_netprop( int, m_iSecondaryAmmoType, "DT_BaseCombatWeapon", "m_iSecondaryAmmoType" );
	horizon_netprop( CSItemDefinitionIndex, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex" );
	horizon_netprop( float, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack" );
	horizon_netprop( float, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack" );
};

class C_WeaponCSBase : public C_BaseCombatWeapon
{
public:
	auto UpdateAccuracyPenalty() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::C_WeaponCSBase::UpdateAccuracyPenalty )( this );
	}

	auto GetInaccuracy() -> float
	{
		using Fn = float( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::C_WeaponCSBase::GetInaccuracy )( this );
	}

	auto GetSpread() -> float
	{
		using Fn = float( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::C_WeaponCSBase::GetSpread )( this );
	}

public:
	auto IsFireTime() -> bool;
	auto IsSecondaryFireTime() -> bool;

	auto IsGun() -> bool;
	auto IsPistol() -> bool;

	auto GetCSWeaponData() -> CCSWeaponInfo*;

public:
	horizon_netprop( float, m_fAccuracyPenalty, "DT_WeaponCSBase", "m_fAccuracyPenalty" );
	horizon_netprop( float, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex" );
};