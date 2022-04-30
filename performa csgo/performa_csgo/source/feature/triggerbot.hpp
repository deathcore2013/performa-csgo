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

class Triggerbot : public core::Singleton< Triggerbot >
{
public:
	auto Create( win32::Input& input ) -> bool;
	auto Destroy() -> void;

	auto CreateMove() -> void;

	auto SetKeyPressed( bool value ) -> void;

private:
	auto Trace( int random_seed ) -> bool;

private:
	static auto Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool;
	
private:
	CUserCmd* m_cmd = nullptr;
	C_CSPlayer* m_player = nullptr;
	C_WeaponCSBase* m_weapon = nullptr;
	CCSWeaponInfo* m_data = nullptr;

	bool m_key_pressed = false;
	bool m_key_fire = false;

	core::Timer m_timer = { };
};

}

}