#include "weapon.hpp"

auto C_WeaponCSBase::IsFireTime() -> bool
{
	return ( source::m_globals->curtime >= m_flNextPrimaryAttack() );
}

auto C_WeaponCSBase::IsSecondaryFireTime() -> bool
{
	return ( source::m_globals->curtime >= m_flNextSecondaryAttack() );
}

auto C_WeaponCSBase::IsGun() -> bool
{
	auto data = GetCSWeaponData();

	if( !data )
		return false;

	return ( data->m_type != WEAPONTYPE_KNIFE &&
			 data->m_type != WEAPONTYPE_C4 &&
			 data->m_type != WEAPONTYPE_GRENADE );
}

auto C_WeaponCSBase::IsPistol() -> bool
{
	auto data = GetCSWeaponData();
	return ( data ? data->m_type == WEAPONTYPE_PISTOL : false );
}

auto C_WeaponCSBase::GetCSWeaponData() -> CCSWeaponInfo*
{
	using Fn = CCSWeaponInfo*( __thiscall* )( void* );
	static auto original = memory::scan< Fn >( "client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B" );
	return original( this );
}