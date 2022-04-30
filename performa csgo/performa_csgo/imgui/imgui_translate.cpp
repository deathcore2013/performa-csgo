#include "imgui_translate.hpp"

// #include "../crypt.hpp"

#include <array>
#include <string>

namespace ImGui
{
const std::string key_array[ 124 ] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"",
	"Mouse 3",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"",
	"Enter",
	"",
	"",
	"Shift",
	"Ctrl",
	"Alt",
	"Pause",
	"Caps Lock",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12"
};

uint32_t GetCodeFromName( const std::string& name )
{
	for( int i = 0; i < 124; i++ )
	{
		if( name.compare( key_array[ i ] ) == 0 )
			return i;
	}

	return NULL;
}

std::string GetNameFromCode( uint32_t code )
{
	auto name = key_array[ code ];

	if( name.empty() )
		name = "none";

	return name;
}
}