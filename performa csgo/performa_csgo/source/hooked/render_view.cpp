#include "procedure.hpp"

#include "../engine.hpp"

#include "../feature/movement.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

#include "../../imgui/imgui.hpp"

namespace source
{

namespace hooked
{

bool g_chams = false;

auto RenderToTexture( void* ecx, CViewSetup& view, ITexture* texture ) -> void
{
	auto context = source::m_material_system->GetRenderContext();

	context->PushRenderTargetAndViewport();
	context->SetRenderTarget( texture );

	auto fn = ( source::m_view_render_swap ?
				source::m_view_render_swap->Get< RenderViewFn >( index::IViewRender::RenderView ) :
				memory::vget< RenderViewFn >( ecx, index::IViewRender::RenderView ) );

	fn( ecx, view, view, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH | VIEW_CLEAR_STENCIL, 0 );

	context->PopRenderTargetAndViewport();
	context->Release();
}



auto __fastcall RenderView( void* ecx, void* edx, CViewSetup& view, CViewSetup& hudViewSetup, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw ) -> void
{
	g_chams = false;

	auto fn = ( source::m_view_render_swap ?
				source::m_view_render_swap->Get< RenderViewFn >( index::IViewRender::RenderView ) :
				memory::vget< RenderViewFn >( ecx, index::IViewRender::RenderView ) );

	fn( ecx, view, hudViewSetup, nClearFlags, whatToDraw );

	g_chams = true;

	auto width = 0;
	auto height = 0;
	source::m_engine_client->GetScreenSize( width, height );

	QAngle angles = { };
	source::m_engine_client->GetViewAngles( angles );

	CViewSetup view_texture = view;

	view_texture.x = view_texture.oldX = 0;
	view_texture.y = view_texture.oldY = 0;
	view_texture.width = view_texture.oldWidth = width;
	view_texture.height = view_texture.oldHeight = height;
	view_texture.angles = angles;
	view_texture.m_flAspectRatio = static_cast< float >( width ) / static_cast< float >( height );

	RenderToTexture( ecx, view_texture, source::m_texture_render_target );

	
}

}

}