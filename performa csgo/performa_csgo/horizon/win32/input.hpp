#pragma once

#include "base.hpp"

#include "../core/singleton.hpp"

namespace horizon
{

namespace win32
{

using EventFn = bool( * )( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam );

class Input : public core::Singleton< Input >
{
public:
	auto Create() -> bool;
	auto Destroy() -> void;

	auto SetWindow( HWND window ) -> void;
	auto GetWindow() const -> HWND;

	auto AddEvent( EventFn procedure ) -> void;
	auto ExecuteEventArray( HWND window, UINT message, WPARAM wparam, LPARAM lparam ) -> LRESULT;

private:
	static auto api_win32 ProcedureEnum( HWND window, LPARAM lparam ) -> int;
	static auto api_win32 Procedure( HWND window, UINT message, WPARAM wparam, LPARAM lparam ) -> LRESULT;

private:
	HWND m_window = nullptr;
	WNDPROC m_procedure = nullptr;
	std::vector< EventFn > m_event_array = { };
};

}

}