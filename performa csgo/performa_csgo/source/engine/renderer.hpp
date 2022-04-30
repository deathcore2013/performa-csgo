#pragma once

#include "../../valve/sdk.hpp"

#include "../../imgui/imgui.hpp"
#include "../../imgui/imgui_impl_dx9.hpp"

namespace source
{

namespace engine
{

enum TextLayout : unsigned
{
	TextLeft = 0u,
	TextRight = 1u,
	TextCenterV = 2u,
	TextCenterH = 4u,
	TextCenter = TextCenterV | TextCenterH,
};

class Renderer : public core::Singleton< Renderer >
{
public:
	auto Create( HWND window, IDirect3DDevice9* device ) -> bool;
	auto Destroy() -> void;
	auto Free() -> void;

	auto Begin() -> bool;
	auto End() -> void;

	auto Present() -> void;

	auto DeviceLost() -> void;
	auto DeviceReset( HRESULT result ) -> void;

public:
	auto DrawQuad( const Vector2D& p0, const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const ImColor& color ) -> void;
	auto DrawRect( const Vector2D& begin, const Vector2D& size, const ImColor& color ) -> void;
	auto DrawLine( const Vector2D& begin, const Vector2D& end, const ImColor& color, float width = 1.f )  -> void;
	auto DrawText( const Vector2D& begin, unsigned align, const ImColor& color, const char* message, ... )  -> void;
	auto DrawText( const Vector2D& begin, unsigned align, const ImColor& color, const wchar_t* message, ... )  -> void;
	auto GetTextSize( Vector2D& output, const char* message, ... ) -> void;
};

}

}