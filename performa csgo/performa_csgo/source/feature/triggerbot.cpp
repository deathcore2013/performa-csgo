#include "triggerbot.hpp"
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

auto Triggerbot::Create( win32::Input& input ) -> bool
{
	input.AddEvent( &Triggerbot::Event );
	return true;
}

auto Triggerbot::Destroy() -> void
{

}

auto Triggerbot::CreateMove() -> void
{
	auto& prediction = engine::Prediction::Instance();

	auto& triggerbot = option::m_current.m_triggerbot;

	m_cmd = prediction.GetUserCmd();
	m_player = prediction.GetPlayer();
	m_weapon = prediction.GetWeapon();
	m_data = m_weapon->GetCSWeaponData();

	if( !triggerbot.m_mode )
		return;

	if( triggerbot.m_mode == 2 )
	{
		if( !m_key_pressed )
		{
			m_key_fire = false;
			m_timer.Reset();
			return;
		}
	}

	if( triggerbot.m_fire_delay )
	{
		if( m_timer.Elapsed() < triggerbot.m_fire_delay )
			return;
	}

	if( Trace( m_cmd->random_seed ) )
	{
		m_key_fire = true;
		m_cmd->buttons |= IN_ATTACK;
	}

	if( m_key_fire )
	{
		if( m_player->m_iShotsFired() < triggerbot.m_fire_burst )
			m_cmd->buttons |= IN_ATTACK;
		else
			m_key_fire = false;
	}
}

auto Triggerbot::SetKeyPressed( bool value ) -> void
{
	m_key_pressed = value;
}

auto Triggerbot::Trace( int random_seed ) -> bool
{
	auto& accuracy = Accuracy::Instance();

	auto& triggerbot = option::m_current.m_triggerbot;

	auto viewangles( m_cmd->viewangles );
	auto start( m_player->EyePosition() );

	Vector end = { };

	if( triggerbot.m_hitchance )
	{
		auto succeeded = 0;

		for( auto i = 0; i < 256; i++ )
		{
			auto va( viewangles );

			accuracy.CompensateSpread( viewangles, va, i, true );
			accuracy.CompensateRecoil( va, va, true );

			auto direction = va.ToVector();
			end = direction * m_data->m_range + start;

			C_BaseEntity* hit = nullptr;

			auto hitbox = -1;
			auto hitgroup = -1;

			if( triggerbot.m_auto_wall )
			{
				auto damage = accuracy.GetPenetrationDamage( nullptr, start, end );

				if( damage < static_cast< float >( triggerbot.m_min_damage ) )
					continue;
			}
			else
			{
				CTraceFilterSimple skip( m_player );

				Ray_t ray = { };
				ray.Init( start, end );

				trace_t trace = { };
				source::m_engine_trace->TraceRay( ray, 0x46004003, &skip, &trace );

				hit = trace.m_pEnt;
			}

			auto player = ToCSPlayer( hit );

			if( !player )
				continue;

			if( !triggerbot.m_target_friendly )
			{
				if( player->m_iTeamNum() == m_player->m_iTeamNum() )
					return false;
			}

			if( ( triggerbot.m_target_head && hitgroup == 1 ) ||
				( triggerbot.m_target_chest && hitgroup == 2 ) ||
				( triggerbot.m_target_stomach && hitgroup == 3 ) ||
				( triggerbot.m_target_arms && ( hitgroup == 4 || hitgroup == 5 ) ) ||
				( triggerbot.m_target_legs && ( hitgroup == 6 || hitgroup == 7 ) ) )
				succeeded++;
		}

		auto current = ( static_cast< float >( succeeded ) * 100.f ) / 256.f;
		auto desired = static_cast< float >( triggerbot.m_hitchance );

		if( current >= desired )
			return true;
	}
	else
	{
		auto va( viewangles );

		accuracy.CompensateRecoil( viewangles, va, true );

		auto direction = va.ToVector();
		end = direction * m_data->m_range + start;

		C_BaseEntity* hit = nullptr;

		auto hitbox = -1;
		auto hitgroup = -1;

		if( triggerbot.m_auto_wall )
		{
			auto damage = accuracy.GetPenetrationDamage( nullptr, start, end );

			if( damage < static_cast< float >( triggerbot.m_min_damage ) )
				return false;
		}
		else
		{
			CTraceFilterSimple skip( m_player );

			Ray_t ray = { };
			ray.Init( start, end );

			trace_t trace = { };
			source::m_engine_trace->TraceRay( ray, 0x46004003, &skip, &trace );

			hit = trace.m_pEnt;
		}

		auto player = ToCSPlayer( hit );

		if( !player )
			return false;

		if( !triggerbot.m_target_friendly )
		{
			if( player->m_iTeamNum() == m_player->m_iTeamNum() )
				return false;
		}

		if( ( triggerbot.m_target_head && hitgroup == 1 ) ||
			( triggerbot.m_target_chest && hitgroup == 2 ) ||
			( triggerbot.m_target_stomach && hitgroup == 3 ) ||
			( triggerbot.m_target_arms && ( hitgroup == 4 || hitgroup == 5 ) ) ||
			( triggerbot.m_target_legs && ( hitgroup == 6 || hitgroup == 7 ) ) )
			return true;
	}

	return false;
}

auto Triggerbot::Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool
{
	auto& data = Triggerbot::Instance();

	auto& triggerbot = option::m_current.m_triggerbot;

	if( triggerbot.m_mode != 2 )
		return false;

	if( triggerbot.m_key == 1 ) // Mouse 1
	{
		if( message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_LBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( triggerbot.m_key == 2 ) // Mouse 2
	{
		if( message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_RBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( triggerbot.m_key == 4 ) // Mouse 3
	{
		if( message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK )
			data.SetKeyPressed( true );
		else if( message == WM_MBUTTONUP )
			data.SetKeyPressed( false );
	}
	else if( triggerbot.m_key == 5 ) // Mouse 4
	{
		auto key = HIWORD( wparam );

		if( key == XBUTTON1 )
		{
			if( message == WM_XBUTTONDOWN || message == WM_XBUTTONDBLCLK )
				data.SetKeyPressed( true );
			else if( message == WM_XBUTTONUP )
				data.SetKeyPressed( false );
		}
	}
	else if( triggerbot.m_key == 6 ) // Mouse 5
	{
		auto key = HIWORD( wparam );

		if( key == XBUTTON2 )
		{
			if( message == WM_XBUTTONDOWN || message == WM_XBUTTONDBLCLK )
				data.SetKeyPressed( true );
			else if( message == WM_XBUTTONUP )
				data.SetKeyPressed( false );
		}
	}
	else
	{
		if( wparam == triggerbot.m_key )
		{
			if( message == WM_KEYDOWN || message == WM_SYSKEYDOWN )
				data.SetKeyPressed( true );
			else if( message == WM_KEYUP || message == WM_SYSKEYUP )
				data.SetKeyPressed( false );
		}
	}

	return false;
}

}

}