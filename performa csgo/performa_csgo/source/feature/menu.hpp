#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace feature
{

enum tab_side
{
	side_none,
	side_left,
	side_right,
};

class Menu : public core::Singleton< Menu >
{
public:
	auto Present() -> void;

private:
	auto RenderForm( float x, float y ) -> void;
	auto RenderTab( float x, float y, const char* name, tab_side side = side_none ) -> void;
	auto RenderGroupBox( float x, float y, const char* name ) -> void;
	auto RenderCheckbox( float x, float y, const char* name ) -> void;
	auto RenderSlider( float x, float y, const char* name ) -> void;

private:
	bool m_open = false;
	Vector2D m_form = { };
	Vector2D m_mouse = { };
	
	// POINT m_mouse = { };
};

}

}