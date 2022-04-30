#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace feature
{

enum gui_tab
{
	tab_aimbot,
	tab_triggerbot,
	tab_esp,
	tab_removal,
	tab_misc,
	tab_config,
};

struct gui_tab_t
{
	bool m_render = false;
	int m_index = -1;
	std::string m_name = { };

	gui_tab_t() = default;

	gui_tab_t( bool render, int index, const std::string& name )
		: m_render( render )
		, m_index( index )
		, m_name( name )
	{ }
};

class Gui : public core::Singleton< Gui >
{
public:
	auto Create( win32::Input& input ) -> bool;
	auto Destroy() -> void;

	auto Present() -> void;

	auto CreateTab( const std::string& name, bool render = false ) -> void;

private:
	static auto Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool;

private:
	ConVar* m_cvar_mouse = nullptr;
	ConVar* m_cvar_raw_input = nullptr;

	bool m_render_mouse = false;
	bool m_raw_input = false;

	std::vector< gui_tab_t > m_tab_array = { };

	int m_aimbot_weapon = 0;
	int m_triggerbot_weapon = 0;

	int m_directory = -1;
	char m_directory_input[ MAX_PATH ] = { };
};

}

}