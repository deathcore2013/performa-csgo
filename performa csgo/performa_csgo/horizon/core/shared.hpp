#pragma once

#include "base.hpp"
#include "singleton.hpp"

namespace horizon
{

namespace win32
{

class Input;

}

namespace core
{

enum ExecuteState
{
	ExecuteNone,
	ExecuteLoad,
	ExecuteSave,
};

class Shared : public Singleton< Shared >
{
public:
	auto Create( win32::Input& input ) -> bool;
	auto Destroy() -> void;

	auto Quit() const -> bool;
	auto Quit( bool value ) -> void;

	auto Gui() const -> bool;
	auto Gui( bool value ) -> void;

	auto GetExecuteDirectory() const -> const std::string&;
	auto SetExecuteDirectory( const std::string& execute_directory ) -> void;

	auto GetExecuteState() const -> ExecuteState;
	auto SetExecuteState( ExecuteState state ) -> void;

private:
	static auto Event( std::uint32_t message, std::uintptr_t wparam, std::intptr_t lparam ) -> bool;

private:
	bool m_quit = false;
	bool m_gui = false;

	std::string m_execute_directory = { "auto" };
	ExecuteState m_execute_state;
};

}

}