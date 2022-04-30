#include "renderer.hpp"

namespace source
{

namespace engine
{

auto Renderer::Create( HWND window, IDirect3DDevice9* device ) -> bool
{
	if( !ImGui_ImplDX9_Init( window, device ) )
		return false;

	ImFontConfig font_config = { };

	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = true;

	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\UbuntuMono-R.ttf", 16.f, &font_config, io.Fonts->GetGlyphRangesCyrillic() );

	return true;
}

auto Renderer::Destroy() -> void
{
	
}

auto Renderer::Free() -> void
{
	ImGui_ImplDX9_Shutdown();
}

auto Renderer::Begin() -> bool
{
	ImGui_ImplDX9_NewFrame();

	ImGui::PushStyleColor( ImGuiCol_WindowBg, { 0, 0, 0, 0 } );
	auto result = ImGui::Begin( "overlay", reinterpret_cast< bool* >( true ), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs );

	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos( { 0.f, 0.f }, ImGuiCond_Always );
	ImGui::SetWindowSize( io.DisplaySize, ImGuiCond_Always );

	return result;
}

auto Renderer::End() -> void
{
	auto draw_list = ImGui::GetWindowDrawList();
	
	ImGui::End();
	ImGui::PopStyleColor();
}

auto Renderer::Present() -> void
{
	ImGui::Render();
}

auto Renderer::DeviceLost() -> void
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

auto Renderer::DeviceReset( HRESULT result ) -> void
{
	if( SUCCEEDED( result ) )
		ImGui_ImplDX9_CreateDeviceObjects();
}

auto Renderer::DrawQuad( const Vector2D& p0, const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const ImColor& color ) -> void
{
	auto render = ImGui::GetWindowDrawList();
	render->AddQuadFilled( { p0[ 0 ], p0[ 1 ] }, { p1[ 0 ], p1[ 1 ] }, { p2[ 0 ], p2[ 1 ] }, { p3[ 0 ], p3[ 1 ] }, color );
}

auto Renderer::DrawRect( const Vector2D& begin, const Vector2D& size, const ImColor& color ) -> void
{
	auto render = ImGui::GetWindowDrawList();
	render->AddRectFilled( { begin[ 0 ], begin[ 1 ] }, { begin[ 0 ] + size[ 0 ], begin[ 1 ] + size[ 1 ] }, color );
}

auto Renderer::DrawLine( const Vector2D& begin, const Vector2D& end, const ImColor& color, float width /*= 1.f*/ ) -> void
{
	auto render = ImGui::GetWindowDrawList();
	render->AddLine( { begin[ 0 ], begin[ 1 ] }, { end[ 0 ], end[ 1 ] }, color );
}

auto Renderer::DrawText( const Vector2D& begin, unsigned align, const ImColor& color, const char* message, ... ) -> void
{
	char output[ 1024 ] = { };

	va_list args;
	va_start( args, message );
	vsprintf_s( output, message, args );
	va_end( args );

	auto coord = ImVec2( begin[ 0 ], begin[ 1 ] );
	auto size = ImGui::CalcTextSize( output );

	if( align & TextRight )
		coord.x -= size.x;
	else if( align & TextCenterH )
		coord.x -= size.x / 2.f;

	if( align & TextCenterV )
		coord.y -= size.y / 2.f;

	auto render = ImGui::GetWindowDrawList();
	
	auto coord_out = ImVec2{ coord.x + 1.f, coord.y + 1.f };
	render->AddText( coord_out, ImColor{ 0.f, 0.f, 0.f, 1.f }, output );
	render->AddText( coord, color, output );
}

auto Renderer::GetTextSize( Vector2D& output, const char* message, ... ) -> void
{
	char format[ 1024 ] = { };

	va_list args;
	va_start( args, message );
	vsprintf_s( format, message, args );
	va_end( args );

	auto size = ImGui::CalcTextSize( format );

	output[ 0 ] = size.x;
	output[ 1 ] = size.y;
}

auto Renderer::DrawText( const Vector2D& begin, unsigned align, const ImColor& color, const wchar_t* message, ... ) -> void
{
	wchar_t output[ 1024 ] = { };

	va_list args;
	va_start( args, message );
	wvsprintf( output, message, args );
	va_end( args );

	auto coord = ImVec2( begin[ 0 ], begin[ 1 ] );
	auto size = ImGui::CalcTextSizeW( output );

	if( align & TextRight )
		coord.x -= size.x;
	else if( align & TextCenterH )
		coord.x -= size.x / 2.f;

	if( align & TextCenterV )
		coord.y -= size.y / 2.f;

	auto render = ImGui::GetWindowDrawList();

	auto coord_out = ImVec2{ coord.x + 1.f, coord.y + 1.f };
	render->AddTextW( coord_out, ImColor{ 0.f, 0.f, 0.f, 1.f }, output );
	render->AddTextW( coord, color, output );
}

}

}