#include "menu.hpp"

#include "../engine/renderer.hpp"

namespace source
{

namespace feature
{

auto Menu::Present() -> void
{
	RenderForm( 100.f, 100.f );

	RenderTab( 100.f + 4.f + 2.f, 100.f + 22.f + 2.f, "ragebot", side_left );
	RenderTab( 100.f + 4.f + 2.f + 120.f + 2.f, 100.f + 22.f + 2.f, "esp", side_none );
	RenderTab( 100.f + 4.f + 2.f + 120.f + 20.f + 2.f + 120.f + 2.f, 100.f + 22.f + 2.f, "misc", side_right );

	RenderGroupBox( 100.f + 4.f + 2.f, 100.f + 22.f + 2.f + 22.f + 10.f, "stuff" );
}

auto Menu::RenderForm( float x, float y ) -> void
{
	auto& renderer = engine::Renderer::Instance();

	renderer.DrawRect( { x, y }, { 600.f, 420.f }, { 0, 0, 0, 255 } );
	renderer.DrawRect( { x + 1.f, y + 1.f }, { 598.f, 418.f }, { 25, 25, 25, 255 } );

	renderer.DrawRect( { x + 3.f, y + 21.f }, { 594.f, 396.f }, { 120, 120, 120, 255 } );
	renderer.DrawRect( { x + 4.f, y + 22.f }, { 592.f, 394.f }, { 45, 45, 45, 255 } );

	renderer.DrawText( { x + 600.f / 2.f, y + 20.f / 2.f }, engine::TextCenter, { 1.f, 1.f, 1.f, 1.f }, "kidua" );

	// renderer.DrawText( { x })
}

auto Menu::RenderTab( float x, float y, const char* name, tab_side side /*= side_none*/ ) -> void
{
	auto& renderer = engine::Renderer::Instance();

	constexpr auto render_corner = 20.f;
	constexpr auto render_width = 120.f;
	constexpr auto render_height = 22.f;


	ImGui::GetStyle().AntiAliasedLines = false;
	ImGui::GetStyle().AntiAliasedShapes = false;

	if( side == side_left )
	{
		renderer.DrawQuad( { x, y }, { x, y + render_height }, { x + render_width + render_corner, y + render_height }, { x + render_width, y }, { 35, 35, 35, 255 } );
		renderer.DrawQuad( { x + 1.f, y + 1.f }, { x + 1.f, y + render_height - 1.f }, { x + render_width + render_corner - 2.f, y + render_height - 1.f }, { x + render_width - 1.f, y + 1.f }, { 55, 55, 55, 255 } );
		renderer.DrawText( { x + render_width / 2.f, y + render_height / 2.f }, engine::TextCenter, { 1.f, 1.f, 1.f, 1.f }, name );
	}
	else if( side == side_none )
	{
		// 
		//  p0                    p3
		// -------------------------
		// \                        \
		//  \                        \
		//   -------------------------
		//  p1                     p2

		auto p0 = Vector2D{ x, y };
		auto p1 = Vector2D{ x + render_corner, y + render_height };
		auto p2 = Vector2D{ x + render_corner + render_width + render_corner, y + render_height };
		auto p3 = Vector2D{ x + render_corner + render_width, y };

		renderer.DrawQuad( p0, p1, p2, p3, { 35, 35, 35, 255 } );
		renderer.DrawQuad( p0 + Vector2D{ 2.f, 1.f }, p1 + Vector2D{ 1.f, -1.f }, p2 + Vector2D{ -2.f, -1.f }, p3 + Vector2D{ -1.f, 1.f }, { 55, 55, 55, 255 } );
		renderer.DrawText( { x + render_corner + render_width / 2.f, y + render_height / 2.f }, engine::TextCenter, { 1.f, 1.f, 1.f, 1.f }, name );
		// renderer.DrawQuad( { x, y }, { x + render_corner, y + render_height }, { x + render_width + render_corner * 2.f, y + render_height }, { x + render_width + render_corner, y }, { 35, 35, 35, 255 } );
	}
	else if( side == side_right )
	{
		// 
		//  p0                    p3
		// ---------------------------
		// \                         |
		//  \                        |
		//   -------------------------
		//  p1                     p2
		// 
		auto p0 = Vector2D{ x, y };
		auto p1 = Vector2D{ x + render_corner, y + render_height };
		auto p2 = Vector2D{ x + render_corner + render_width, y + render_height };
		auto p3 = Vector2D{ x + render_corner + render_width, y };

		renderer.DrawQuad( p0, p1, p2, p3, { 35, 35, 35, 255 } );
		renderer.DrawQuad( p0 + Vector2D{ 2.f, 1.f }, p1 + Vector2D{ 1.f, -1.f }, p2 + Vector2D{ -1.f, -1.f }, p3 + Vector2D{ -1.f, 1.f }, { 55, 55, 55, 255 } );
		renderer.DrawText( { x + render_corner + render_width / 2.f, y + render_height / 2.f }, engine::TextCenter, { 1.f, 1.f, 1.f, 1.f }, name );
	}
}

auto Menu::RenderGroupBox( float x, float y, const char* name ) -> void
{
	auto& renderer = engine::Renderer::Instance();

	// 
	// .-- name --------.
	// |                |
	// |                |
	// .----------------.
	// 

	constexpr auto render_width = 260.f;
	constexpr auto render_height = 200.f;

	Vector2D size = { };
	renderer.GetTextSize( size, name );

	renderer.DrawLine( { x, y }, { x + 40.f, y }, { 120, 120, 120, 255 } );
	renderer.DrawLine( { x + 40.f + size[ 0 ], y }, { x + render_width - 40.f - size[ 0 ], y }, { 120, 120, 120, 255 } );
	renderer.DrawLine( { x, y }, { x, y + render_height }, { 120, 120, 120, 255 } );
	renderer.DrawLine( { x, y + render_height }, { x + render_width - 40.f - size[ 0 ], y + render_height }, { 120, 120, 120, 255 } );
	renderer.DrawLine( { x + render_width - 40.f - size[ 0 ], y }, { x + render_width - 40.f - size[ 0 ], y + render_height }, { 120, 120, 120, 255 } );

	renderer.DrawText( { x + 40.f + size[ 0 ] / 2.f, y - size[ 1 ] / 2.f }, engine::TextCenterH, { 1.f, 1.f, 1.f, 1.f }, name );
}

auto Menu::RenderCheckbox( float x, float y, const char* name ) -> void
{

}

auto Menu::RenderSlider( float x, float y, const char* name ) -> void
{

}

}

}