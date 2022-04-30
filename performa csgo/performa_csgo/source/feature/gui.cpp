#include "gui.hpp"

#include "../engine.hpp"

#include "../../imgui/imgui.hpp"
#include "../../imgui/imgui_impl_dx9.hpp"

#include "../../option/storage.hpp"

using namespace std::string_literals;

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND window, UINT message, WPARAM wparam, LPARAM lparam );

namespace source
{

namespace feature
{

auto Gui::Create( win32::Input& input ) -> bool
{
	input.AddEvent( &Gui::Event );

	m_cvar_mouse = source::m_cvar->FindVar( "cl_mouseenable" );
	m_cvar_raw_input = source::m_cvar->FindVar( "m_rawinput" );

	m_raw_input = m_cvar_raw_input->GetBool();

	auto& style = ImGui::GetStyle();

	#pragma region region_colors
	style.Colors[ ImGuiCol_Text ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
	style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.16f, 0.16f, 0.16f, 1.00f );
	style.Colors[ ImGuiCol_ChildWindowBg ] = ImVec4( 0.10f, 0.10f, 0.10f, 0.10f );
	style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.16f, 0.16f, 0.16f, 1.00f );
	style.Colors[ ImGuiCol_Border ] = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
	style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	style.Colors[ ImGuiCol_FrameBg ] = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
	style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.24f, 0.24f, 0.24f, 0.61f );
	style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.24f, 0.24f, 0.24f, 0.39f );
	style.Colors[ ImGuiCol_TitleBg ] = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );
	style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.00f );
	style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.00f );
	style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
	style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.12f, 0.12f, 0.12f, 1.00f );
	style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.61f );
	style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.39f );
	style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.61f );
	style.Colors[ ImGuiCol_Button ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.61f );
	style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.39f );
	style.Colors[ ImGuiCol_Header ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.61f );
	style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.39f );
	style.Colors[ ImGuiCol_Column ] = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
	style.Colors[ ImGuiCol_ColumnHovered ] = ImVec4( 0.59f, 0.59f, 0.59f, 0.61f );
	style.Colors[ ImGuiCol_ColumnActive ] = ImVec4( 0.59f, 0.59f, 0.59f, 0.39f );
	style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.30f );
	style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.60f );
	style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 1.00f, 1.00f, 1.00f, 0.90f );
	style.Colors[ ImGuiCol_CloseButton ] = ImVec4( 1.00f, 0.50f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_CloseButtonHovered ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.61f );
	style.Colors[ ImGuiCol_CloseButtonActive ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.39f );
	style.Colors[ ImGuiCol_PlotLines ] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 1.00f, 0.50f, 0.00f, 0.39f );
	style.Colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.20f, 0.20f, 0.20f, 0.35f );
	#pragma endregion

	#pragma region region_style
	style.WindowPadding = { 8.f, 8.f };
	style.WindowRounding = 0.f;
	style.ChildRounding = 0.f;
	style.FramePadding = { 4.f, 4.f };
	style.FrameRounding = 0.f;
	style.ItemSpacing = { 8, 4 };
	style.ItemInnerSpacing = { 4.f, 4.f };
	style.TouchExtraPadding = { 0.f, 0.f };
	style.IndentSpacing = 20.f;
	style.ScrollbarSize = 20.f;
	style.ScrollbarRounding = 0.f;
	style.GrabRounding = 0.f;
	#pragma endregion

	CreateTab( "aimbot", false );
	CreateTab( "triggerbot", false );
	CreateTab( "esp", false );
	CreateTab( "removal", false );
	CreateTab( "misc", false );
	CreateTab( "config", false );

	return true;
}

auto Gui::Destroy() -> void
{
	if( m_cvar_mouse )
	{
		m_cvar_mouse->SetValue( true );
		m_cvar_mouse = nullptr;
	}

	if( m_cvar_raw_input )
	{
		m_cvar_raw_input->SetValue( m_raw_input );
		m_cvar_raw_input = nullptr;
	}
}

auto get_extension( const std::string& data )
{
	auto dot = data.find_last_of( '.' );

	std::string extension = { };

	if( dot != std::string::npos )
		extension = data.substr( dot + 1u, data.size() );

	return extension;
}

auto Gui::Present() -> void
{
	auto& shared = core::Shared::Instance();

	if( m_render_mouse != shared.Gui() )
	{
		m_render_mouse = shared.Gui();

		m_cvar_mouse->SetValue( !m_render_mouse );
		m_cvar_raw_input->SetValue( m_render_mouse );
	}

	auto& io = ImGui::GetIO();
	io.MouseDrawCursor = m_render_mouse;

	if( shared.Gui() )
	{
		auto render_style = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

		#pragma region render_frame
		auto frame_height = 0.f;

		D3DVIEWPORT9 viewport = { };
		if( SUCCEEDED( source::m_direct_device->GetViewport( &viewport ) ) )
			frame_height = static_cast< float >( viewport.Height );

		auto frame_pos = ImVec2{ 0.f, 0.f };
		auto frame_size = ImVec2{ 156.f, frame_height };

		ImGui::SetNextWindowPos( frame_pos, ImGuiCond_Always );
		ImGui::SetNextWindowSize( frame_size, ImGuiCond_Always );

		auto frame_render_style = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

		if( ImGui::Begin( "frame", nullptr, frame_render_style ) )
		{
			auto& gui_style = ImGui::GetStyle();

			auto tab_size = ImVec2{ 140.f, 30.f };
			auto tab_array_size = ( ( 30.f * ( static_cast< float >( m_tab_array.size() ) + gui_style.ItemSpacing.y ) ) - gui_style.ItemSpacing.y ) * 0.5f;

			auto frame_tab_center_y = frame_height * 0.5f;
			auto frame_tab_start_y = ( frame_tab_center_y - tab_array_size ) * 0.5f;

			ImGui::Dummy( { 0.f, frame_tab_start_y } );
			ImGui::Dummy( { 0.f, 0.f } );

			auto frame_name = "horizon"s;
			auto frame_name_size = ImGui::CalcTextSize( frame_name.c_str(), nullptr, true );

			ImGui::SameLine( 0.f, ( frame_size.x * 0.5f ) - ( frame_name_size.x * 0.5f ) - gui_style.ItemSpacing.x );
			ImGui::Text( frame_name.c_str() );
			ImGui::Dummy( { 0.f, frame_tab_start_y } );

			ImGui::PushStyleVar( ImGuiStyleVar_ButtonTextAlign, { 0.f, 0.5f } );
			ImGui::PushStyleColor( ImGuiCol_Text, { 1.f, 1.f, 1.f, 1.f } );
			ImGui::PushStyleColor( ImGuiCol_Button, { 0.1f, 0.1f, 0.1f, 1.f } );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, { 0.12f, 0.12f, 0.12f, 1.f } );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, { 0.15f, 0.15f, 0.15f, 1.f } );

			for( auto& tab : m_tab_array )
			{
				if( ImGui::Button( tab.m_name.c_str(), tab_size ) )
					tab.m_render = !tab.m_render;
			}

			ImGui::PopStyleColor( 4 );
			ImGui::PopStyleVar( 1 );
		}
		ImGui::End();
		#pragma endregion

		/*auto frame_feature_render = false;

		for( const auto& tab : m_tab_array )
		{
			if( tab.m_render )
				frame_feature_render = true;
		}

		if( frame_feature_render )
		{
			auto frame_feature_render_style = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

			auto frame_feature_width = ( static_cast< float >( viewport.Width ) - frame_size.x ) * 0.8f;
			auto frame_feature_height = frame_size.y * 0.65f;

			auto frame_feature_pos = ImVec2{ frame_size.x, ( frame_height * 0.5f ) - ( frame_feature_height * 0.5f ) };
			auto frame_feature_size = ImVec2{ 560.f, frame_feature_height };

			ImGui::SetNextWindowPos( frame_feature_pos, ImGuiCond_Always );
			ImGui::SetNextWindowSize( frame_feature_size, ImGuiCond_Always );

			if( ImGui::Begin( "frame_feature", nullptr, frame_feature_render_style ) )
			{
				
			}
			ImGui::End();
		}*/

		if( m_tab_array.at( tab_aimbot ).m_render )
		{
			if( ImGui::Begin( "aimbot", nullptr, render_style ) )
			{
				ImGui::Checkbox( "per weapon", &option::m_aimbot.m_per_weapon );

				if( option::m_aimbot.m_per_weapon )
				{
					auto weapon_name_array = option::GetWeaponNameArray();

					ImGui::SameLine();
					ImGui::PushItemWidth( 140.f );
					ImGui::Combo( "###weapon_aimbot", &m_aimbot_weapon, weapon_name_array );
					ImGui::PopItemWidth();

					auto index = option::GetWeaponIndex( weapon_name_array[ m_aimbot_weapon ] );
					auto& aimbot = option::m_weapon[ index ].m_aimbot;

					ImGui::PushItemWidth( 160.f );
					if( ImGui::CollapsingHeader( "core" ) )
					{
						ImGui::Combo( "mode###core_mode", &aimbot.m_mode, aimbot.m_mode_array );
						ImGui::KeyBox( "key###core_key", &aimbot.m_key );
						ImGui::Checkbox( "auto fire###core_auto_fire", &aimbot.m_auto_fire );
						ImGui::Checkbox( "auto cycle###core_auto_cycle", &aimbot.m_auto_cycle );
						ImGui::Combo( "silent", &aimbot.m_silent, aimbot.m_silent_array );
						ImGui::SliderInt( "hitchance###core_hitchance", &aimbot.m_hitchance, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "target" ) )
					{
						ImGui::Combo( "spot###target_spot", &aimbot.m_target_spot, aimbot.m_target_spot_array );
						ImGui::Checkbox( "height correction###target_height_correction", &aimbot.m_target_height_correction );
						ImGui::Combo( "hitscan###target_hitscan", &aimbot.m_target_hitscan, aimbot.m_target_hitscan_array );
						ImGui::SliderFloat( "hitscan scale###target_hitscan_scale", &aimbot.m_target_hitscan_scale, 0.f, 1.f );
						ImGui::Combo( "adjustment###target_adjustment", &aimbot.m_target_adjustment, aimbot.m_target_adjustment_array );
						ImGui::Combo( "resolver###target_resolver", &aimbot.m_target_resolver, aimbot.m_target_resolve_array );
						ImGui::Combo( "priority###target_priority", &aimbot.m_target_priority, aimbot.m_target_priority_array );
						ImGui::SliderFloat( "fov###target_fov", &aimbot.m_target_fov, 0.f, 180.f );
						ImGui::Checkbox( "friendly###target_friendly", &aimbot.m_target_friendly );
					}
					if( ImGui::CollapsingHeader( "step" ) )
					{
						ImGui::Combo( "mode###step_mode", &aimbot.m_step_mode, aimbot.m_step_mode_array );
						ImGui::SliderFloat( "pitch###step_pitch", &aimbot.m_step_pitch, 0.f, 100.f );
						ImGui::SliderFloat( "yaw###step_yaw", &aimbot.m_step_yaw, 0.f, 100.f );
					}
					if( ImGui::CollapsingHeader( "time" ) )
					{
						ImGui::SliderInt( "delay###time_delay", &aimbot.m_time_delay, 0, 5000 );
						ImGui::SliderInt( "duration###time_duration", &aimbot.m_time_duration, 0, 5000 );
						ImGui::SliderInt( "cycle delay###time_cycle_delay", &aimbot.m_time_cycle_delay, 0, 5000 );
					}
					if( ImGui::CollapsingHeader( "rcs" ) )
					{
						ImGui::Combo( "mode###rcs_mode", &aimbot.m_rcs_mode, aimbot.m_rcs_mode_array );
						ImGui::SliderInt( "delay###rcs_delay", &aimbot.m_rcs_delay, 0, 10 );
						ImGui::SliderInt( "pitch###rcs_pitch", &aimbot.m_rcs_pitch, 0, 100, "%.0f%%" );
						ImGui::SliderInt( "yaw###rcs_yaw", &aimbot.m_rcs_yaw, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "advanced" ) )
					{
						ImGui::Checkbox( "auto wall###advanced_auto_wall", &aimbot.m_auto_wall );
						ImGui::SliderInt( "min damage###advanced_min_damage", &aimbot.m_min_damage, 0, 100 );
						// ImGui::Combo( "spot scan###advanced_spot_scan", &aimbot.m_spot_scan, aimbot.m_spot_scan_array );
						// ImGui::SliderInt( "spot scale###advanced_spot_scale", &aimbot.m_spot_scale, 0, 100, "%0.f%%" );
					}
					ImGui::PopItemWidth();
				}
				else
				{
					auto& aimbot = option::m_aimbot;

					ImGui::PushItemWidth( 160.f );
					if( ImGui::CollapsingHeader( "core" ) )
					{
						ImGui::Combo( "mode###core_mode", &aimbot.m_mode, aimbot.m_mode_array );
						ImGui::KeyBox( "key###core_key", &aimbot.m_key );
						ImGui::Checkbox( "auto fire###core_auto_fire", &aimbot.m_auto_fire );
						ImGui::Checkbox( "auto cycle###core_auto_cycle", &aimbot.m_auto_cycle );
						ImGui::Combo( "silent###core_silent", &aimbot.m_silent, aimbot.m_silent_array );
						ImGui::SliderInt( "hitchance###core_hitchance", &aimbot.m_hitchance, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "target" ) )
					{
						ImGui::Combo( "spot###target_spot", &aimbot.m_target_spot, aimbot.m_target_spot_array );
						ImGui::Checkbox( "height correction###target_height_correction", &aimbot.m_target_height_correction );
						ImGui::Combo( "hitscan###target_hitscan", &aimbot.m_target_hitscan, aimbot.m_target_hitscan_array );
						ImGui::SliderFloat( "hitscan scale###target_hitscan_scale", &aimbot.m_target_hitscan_scale, 0.f, 1.f );
						ImGui::Combo( "adjustment###target_adjustment", &aimbot.m_target_adjustment, aimbot.m_target_adjustment_array );
						ImGui::Combo( "resolver###target_resolver", &aimbot.m_target_resolver, aimbot.m_target_resolve_array );
						ImGui::Combo( "priority###target_priority", &aimbot.m_target_priority, aimbot.m_target_priority_array );
						ImGui::SliderFloat( "fov###target_fov", &aimbot.m_target_fov, 0.f, 180.f );
						ImGui::Checkbox( "friendly###target_friendly", &aimbot.m_target_friendly );
					}
					if( ImGui::CollapsingHeader( "step" ) )
					{
						ImGui::Combo( "mode###step_mode", &aimbot.m_step_mode, aimbot.m_step_mode_array );
						ImGui::SliderFloat( "pitch###step_pitch", &aimbot.m_step_pitch, 0.f, 100.f );
						ImGui::SliderFloat( "yaw###step_yaw", &aimbot.m_step_yaw, 0.f, 100.f );
					}
					if( ImGui::CollapsingHeader( "time" ) )
					{
						ImGui::SliderInt( "delay###time_delay", &aimbot.m_time_delay, 0, 5000 );
						ImGui::SliderInt( "duration###time_duration", &aimbot.m_time_duration, 0, 5000 );
						ImGui::SliderInt( "cycle delay###time_cycle_delay", &aimbot.m_time_cycle_delay, 0, 5000 );
					}
					if( ImGui::CollapsingHeader( "rcs" ) )
					{
						ImGui::Combo( "mode###rcs_mode", &aimbot.m_rcs_mode, aimbot.m_rcs_mode_array );
						ImGui::SliderInt( "delay###rcs_delay", &aimbot.m_rcs_delay, 0, 10 );
						ImGui::SliderInt( "pitch###rcs_pitch", &aimbot.m_rcs_pitch, 0, 100, "%.0f%%" );
						ImGui::SliderInt( "yaw###rcs_yaw", &aimbot.m_rcs_yaw, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "advanced" ) )
					{
						ImGui::Checkbox( "auto wall###advanced_auto_wall", &aimbot.m_auto_wall );
						ImGui::SliderInt( "min damage###advanced_min_damage", &aimbot.m_min_damage, 0, 100 );
						// ImGui::Combo( "spot scan###advanced_spot_scan", &aimbot.m_spot_scan, aimbot.m_spot_scan_array );
						// ImGui::SliderInt( "spot scale###advanced_spot_scale", &aimbot.m_spot_scale, 0, 100, "%0.f%%" );
					}
					ImGui::PopItemWidth();
				}
			}
			ImGui::End();
		}

		if( m_tab_array.at( tab_triggerbot ).m_render )
		{
			if( ImGui::Begin( "triggerbot", nullptr, render_style ) )
			{
				ImGui::Checkbox( "per weapon", &option::m_triggerbot.m_per_weapon );

				if( option::m_triggerbot.m_per_weapon )
				{
					auto weapon_name_array = option::GetWeaponNameArray();

					ImGui::SameLine();
					ImGui::PushItemWidth( 140.f );
					ImGui::Combo( "###weapon_triggerbot", &m_triggerbot_weapon, weapon_name_array );
					ImGui::PopItemWidth();

					auto index = option::GetWeaponIndex( weapon_name_array[ m_triggerbot_weapon ] );
					auto& triggerbot = option::m_weapon[ index ].m_triggerbot;

					ImGui::PushItemWidth( 160.f );
					if( ImGui::CollapsingHeader( "core" ) )
					{
						ImGui::Combo( "mode###core_mode", &triggerbot.m_mode, triggerbot.m_mode_array );
						ImGui::KeyBox( "key###core_key", &triggerbot.m_key );
						ImGui::SliderInt( "hitchance###core_hitchance", &triggerbot.m_hitchance, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "fire" ) )
					{
						ImGui::SliderInt( "delay###fire_delay", &triggerbot.m_fire_delay, 0, 5000 );
						ImGui::SliderInt( "burst###fire_burst", &triggerbot.m_fire_burst, 0, 10 );
					}
					if( ImGui::CollapsingHeader( "target" ) )
					{
						ImGui::Checkbox( "friendly", &triggerbot.m_target_friendly );
						ImGui::ListBoxHeader( "###spot" );
						ImGui::Selectable( "head", &triggerbot.m_target_head );
						ImGui::Selectable( "chest", &triggerbot.m_target_chest );
						ImGui::Selectable( "stomach", &triggerbot.m_target_stomach );
						ImGui::Selectable( "arms", &triggerbot.m_target_arms );
						ImGui::Selectable( "legs", &triggerbot.m_target_legs );
						ImGui::ListBoxFooter();
					}
					if( ImGui::CollapsingHeader( "penetration" ) )
					{
						ImGui::Checkbox( "auto wall###penetration_auto_wall", &triggerbot.m_auto_wall );
						ImGui::SliderInt( "min damage###penetration_min_damage", &triggerbot.m_min_damage, 0, 100 );
					}
					ImGui::PopItemWidth();
				}
				else
				{
					auto& triggerbot = option::m_triggerbot;

					ImGui::PushItemWidth( 160.f );
					if( ImGui::CollapsingHeader( "core" ) )
					{
						ImGui::Combo( "mode###core_mode", &triggerbot.m_mode, triggerbot.m_mode_array );
						ImGui::KeyBox( "key###core_key", &triggerbot.m_key );
						ImGui::SliderInt( "hitchance###core_hitchance", &triggerbot.m_hitchance, 0, 100, "%.0f%%" );
					}
					if( ImGui::CollapsingHeader( "fire" ) )
					{
						ImGui::SliderInt( "delay###fire_delay", &triggerbot.m_fire_delay, 0, 5000 );
						ImGui::SliderInt( "burst###fire_burst", &triggerbot.m_fire_burst, 0, 10 );
					}
					if( ImGui::CollapsingHeader( "target" ) )
					{
						ImGui::Checkbox( "friendly###target_friendly", &triggerbot.m_target_friendly );
						ImGui::ListBoxHeader( "###spot" );
						ImGui::Selectable( "head###target_head", &triggerbot.m_target_head );
						ImGui::Selectable( "chest###target_chest", &triggerbot.m_target_chest );
						ImGui::Selectable( "stomach###target_stomach", &triggerbot.m_target_stomach );
						ImGui::Selectable( "arms###target_arms", &triggerbot.m_target_arms );
						ImGui::Selectable( "legs###target_legs", &triggerbot.m_target_legs );
						ImGui::ListBoxFooter();
					}
					if( ImGui::CollapsingHeader( "penetration" ) )
					{
						ImGui::Checkbox( "auto wall###penetration_auto_wall", &triggerbot.m_auto_wall );
						ImGui::SliderInt( "min damage###penetration_min_damage", &triggerbot.m_min_damage, 0, 100 );
					}
					ImGui::PopItemWidth();
				}
			}
			ImGui::End();
		}

		if( m_tab_array.at( tab_esp ).m_render )
		{
			if( ImGui::Begin( "esp", nullptr, render_style ) )
			{
				auto& style = ImGui::GetStyle();
				auto& esp = option::m_esp;

				ImGui::Dummy( { 140.f, 0.f } );

				ImGui::PushItemWidth( 160.f );
				if( ImGui::CollapsingHeader( "core" ) )
				{
					ImGui::Combo( "box mode###core_box_mode", &esp.m_box_mode, esp.m_box_mode_array );
					ImGui::Checkbox( "chams###core_chams", &esp.m_chams );
					ImGui::Checkbox( "outlined###core_outlined", &esp.m_outlined );
				}
				if( ImGui::CollapsingHeader( "target" ) )
				{
					ImGui::Checkbox( "friendly###target_friendly", &esp.m_target_friendly );
					ImGui::Checkbox( "bomb###target_bomb", &esp.m_target_bomb );
				}
				if( ImGui::CollapsingHeader( "information" ) )
				{
					ImGui::Checkbox( "name###information_name", &esp.m_name );
					ImGui::Checkbox( "weapon###information_weapon", &esp.m_weapon );
					ImGui::Combo( "health###information_health", &esp.m_health, esp.m_style_array );
					ImGui::Combo( "armor###information_armor", &esp.m_armor, esp.m_style_array );
					ImGui::Checkbox( "skeleton###information_skeleton", &esp.m_skeleton );
					ImGui::Checkbox( "defusing###information_defusing", &esp.m_defusing );
				}
				ImGui::PopItemWidth();
			}
			ImGui::End();
		}

		if( m_tab_array.at( tab_removal ).m_render )
		{
			if( ImGui::Begin( "removal", nullptr, render_style ) )
			{
				auto& removal = option::m_removal;

				ImGui::Dummy( { 140.f, 0.f } );

				ImGui::PushItemWidth( 160.f );
				if( ImGui::CollapsingHeader( "core" ) )
				{
					ImGui::Checkbox( "spread###core_spread", &removal.m_spread );
					ImGui::Checkbox( "recoil###core_recoil", &removal.m_recoil );
				}
				if( ImGui::CollapsingHeader( "visual" ) )
				{
					ImGui::Checkbox( "recoil###visual_recoil", &removal.m_visual_recoil );
					ImGui::Checkbox( "smoke###visual_smoke", &removal.m_visual_smoke );
					ImGui::SliderInt( "flash###visual_flash", &removal.m_visual_flash, 0, 100, "%.0f%%" );
				}
				ImGui::PopItemWidth();
			}
			ImGui::End();
		}

		if( m_tab_array.at( tab_misc ).m_render )
		{
			if( ImGui::Begin( "misc", nullptr, render_style ) )
			{
				auto& style = ImGui::GetStyle();
				auto& misc = option::m_misc;

				ImGui::Dummy( { 140.f, 0.f } );

				ImGui::PushItemWidth( 160.f );
				if( ImGui::CollapsingHeader( "core" ) )
				{
					ImGui::Checkbox( "auto jump###core_auto_jump", &misc.m_auto_jump );
					ImGui::Checkbox( "auto pistol###core_auto_pistol", &misc.m_auto_pistol );
					ImGui::Checkbox( "bomb warning###core_bomb_warning", &misc.m_bomb_warning );
					ImGui::Checkbox( "thirdperson###core_thirdperson", &misc.m_thirdperson );
					ImGui::Checkbox( "ghost fake###core_ghost_fake", &misc.m_ghost_fake );
					ImGui::Checkbox( "ghost lower body###core_ghost_lower_body", &misc.m_ghost_lower_body );
					ImGui::Combo( "restriction###core_restriction", &misc.m_restriction, misc.m_restriction_array );
				}
				if( ImGui::CollapsingHeader( "strafe" ) )
				{
					ImGui::Combo( "mode###strafe_mode", &misc.m_strafe_mode, misc.m_strafe_mode_array );
					ImGui::Checkbox( "ground###strafe_ground", &misc.m_strafe_ground );
				}
				if( ImGui::CollapsingHeader( "antiaim" ) )
				{
					ImGui::Checkbox( "target###antiaim_target", &misc.m_antiaim_target );
					ImGui::Checkbox( "break lower body###antiaim_break_lower_body", &misc.m_antiaim_break_lower_body );
					ImGui::SliderInt( "packet###antiaim_packet", &misc.m_antiaim_packet, 0, 15 );
					ImGui::Combo( "pitch###antiaim_pitch", &misc.m_antiaim_pitch, misc.m_antiaim_pitch_array );
					ImGui::SliderFloat( "pitch mod###antiaim_pitch_mod", &misc.m_antiaim_pitch_mod, -180.f, 180.f );
					ImGui::Combo( "yaw real###antiaim_yaw_real", &misc.m_antiaim_yaw_real, misc.m_antiaim_yaw_array );
					ImGui::SliderFloat( "yaw real mod###antiaim_yaw_real_mod", &misc.m_antiaim_yaw_real_mod, -180.f, 180.f );
					ImGui::Combo( "yaw fake###antiaim_yaw_fake", &misc.m_antiaim_yaw_fake, misc.m_antiaim_yaw_array );
					ImGui::SliderFloat( "yaw fake mod###antiaim_yaw_fake_mod", &misc.m_antiaim_yaw_fake_mod, -180.f, 180.f );
				}
				if( ImGui::CollapsingHeader( "crosshair" ) )
				{
					ImGui::Combo( "mode###crosshair_mode", &misc.m_crosshair_mode, misc.m_crosshair_mode_array );
					ImGui::Checkbox( "outlined###crosshair_outlined", &misc.m_crosshair_outlined );
					ImGui::Checkbox( "recoil###crosshair_recoil", &misc.m_crosshair_recoil );
				}
				ImGui::PopItemWidth();
			}
			ImGui::End();
		}

		if( m_tab_array.at( tab_config ).m_render )
		{
			if( ImGui::Begin( "config", nullptr, render_style ) )
			{
				ImGui::ListBoxHeader( "" );

				WIN32_FIND_DATAA search_data = { };
				LARGE_INTEGER search_data_size = { };

				auto directory = std::string( core::m_data.m_loader_directory );
				directory.append( "\\csgo\\*" );

				auto search_object = FindFirstFileA( directory.c_str(), &search_data );

				std::vector< std::string > search_data_array = { };
				if( search_object != INVALID_HANDLE_VALUE )
				{
					auto search_index = 0;

					do
					{
						auto extension = get_extension( search_data.cFileName );

						if( !( search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && !extension.compare( "ini" ) )
						{
							const auto is_selected = ( search_index == m_directory );

							search_data_size.LowPart = search_data.nFileSizeLow;
							search_data_size.HighPart = search_data.nFileSizeHigh;

							char search_data_name[ MAX_PATH ] = { };
							sprintf_s( search_data_name, "%s [%lld bytes]", search_data.cFileName, search_data_size.QuadPart );

							ImGui::PushID( search_index );
							if( ImGui::Selectable( search_data_name, is_selected ) )
							{
								m_directory = search_index;
								strncpy_s( m_directory_input, search_data.cFileName, sizeof( m_directory_input ) );
							}
							ImGui::PopID();

							search_index++;
							search_data_array.emplace_back( search_data.cFileName );
						}
					}
					while( FindNextFileA( search_object, &search_data ) );

					FindClose( search_object );
				}
				ImGui::ListBoxFooter();

				ImGui::InputText( "", m_directory_input, sizeof( m_directory_input ) );

				if( ImGui::Button( "load" ) && m_directory != -1 )
				{
					shared.SetExecuteDirectory( search_data_array[ m_directory ] );
					shared.SetExecuteState( core::ExecuteLoad );
				}

				ImGui::SameLine();

				if( ImGui::Button( "save" ) && std::strlen( m_directory_input ) )
				{
					shared.SetExecuteDirectory( m_directory_input );
					shared.SetExecuteState( core::ExecuteSave );
				}

				ImGui::SameLine();

				if( ImGui::Button( "delete" ) )
					ImGui::OpenPopup( "delete?" );

				if( ImGui::BeginPopupModal( "delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize ) )
				{
					ImGui::Text( "you are going to delete \"%s\"!\nare you sure?\n\n", m_directory_input );
					ImGui::Separator();

					if( ImGui::Button( "yes", { 120.f, 0.f } ) )
					{
						option::Delete( m_directory_input );
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if( ImGui::Button( "no", { 120.f, 0.f } ) )
						ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
	}
}

auto Gui::CreateTab( const std::string& name, bool selected /*= false*/ ) -> void
{
	auto index = static_cast< int >( m_tab_array.size() );
	m_tab_array.emplace_back( selected, index, name );
}

auto Gui::Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool
{
	auto& input = win32::Input::Instance();
	auto& shared = core::Shared::Instance();
	
	if( !shared.Gui() )
		return false;

	return !!ImGui_ImplWin32_WndProcHandler( input.GetWindow(), message, wparam, lparam );
}

}

}