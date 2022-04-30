#include "shared.hpp"

#include "../win32/input.hpp"

namespace horizon
{

namespace core
{

auto Shared::Create( win32::Input& input ) -> bool
{
	input.AddEvent( &Shared::Event );

	return true;
}

auto Shared::Destroy() -> void
{

}

auto Shared::Quit() const -> bool
{
	return m_quit;
}

auto Shared::Quit( bool value ) -> void
{
	m_quit = value;
	m_gui = !value;
}

auto Shared::Gui() const -> bool
{
	return m_gui;
}

auto Shared::Gui( bool value ) -> void
{
	m_gui = value;
}

auto Shared::GetExecuteDirectory() const -> const std::string&
{
	return m_execute_directory;
}

auto Shared::SetExecuteDirectory( const std::string& execute_directory ) -> void
{
	m_execute_directory = execute_directory;
}

auto Shared::GetExecuteState() const -> ExecuteState
{
	return m_execute_state;
}

auto Shared::SetExecuteState( ExecuteState state ) -> void
{
	m_execute_state = state;
}

auto Shared::Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool
{
	auto& shared = Shared::Instance();

	auto code = false;

	switch( message )
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			switch( wparam )
			{
				case VK_F11:
				{
					code = true;
					shared.Quit( true );
					break;
				}
				case VK_INSERT:
				{
					code = true;
					shared.Gui( !shared.Gui() );
					break;
				}
			}
			break;
		}
	}

	return code;
}

}

}