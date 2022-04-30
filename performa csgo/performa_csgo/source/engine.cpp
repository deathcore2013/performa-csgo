#include "engine.hpp"

#include "engine/factory.hpp"
#include "engine/netprop_manager.hpp"
#include "engine/renderer.hpp"
#include "engine/prediction.hpp"

#include "feature/aimbot.hpp"
#include "feature/gui.hpp"
#include "feature/triggerbot.hpp"
#include "feature/resolver.hpp"

#include "hooked/procedure.hpp"

#include "../valve/source/player.hpp"

#include "../option/storage.hpp"

namespace source
{

class GameEvent : public IGameEventListener2
{
public:
	auto FireGameEvent( IGameEvent* game_event ) -> void
	{
		auto& resolver = feature::Resolver::Instance();
		resolver.FireGameEvent( game_event );
	}

	auto GetEventDebugID() -> int
	{
		return 42;
	}

	auto Create()
	{
		source::m_game_event_manager->AddListener( this, "player_hurt", false );
		source::m_game_event_manager->AddListener( this, "bullet_impact", false );
	}

	auto Destroy()
	{
		source::m_game_event_manager->RemoveListener( this );
		// source::m_game_event_manager->RemoveListener( this );
	}
};

GameEvent m_game_event = { };

}

namespace source
{

IBaseClientDLL* m_base_client = nullptr;
IClientEntityList* m_client_entity_list = nullptr;
IGameMovement* m_game_movement = nullptr;
IPrediction* m_prediction = nullptr;

IVEngineClient* m_engine_client = nullptr;
IVModelInfoClient* m_model_info_client = nullptr;
IVRenderView* m_render_view = nullptr;
IVModelRender* m_model_render = nullptr;
IEngineTrace* m_engine_trace = nullptr;

ICvar* m_cvar = nullptr;
IMemAlloc* m_mem_alloc = nullptr;

IPhysicsSurfaceProps* m_physics_surface_props = nullptr;

IMaterialSystem* m_material_system = nullptr;

IMoveHelper* m_move_helper = nullptr;
IViewRender* m_view_render = nullptr;
IInput* m_input = nullptr;

IGameEventManager* m_game_event_manager = nullptr;

CGlobalVarsBase* m_globals = nullptr;
CBaseClientState* m_client_state = nullptr;

IDirect3DDevice9* m_direct_device = nullptr;


ITexture* m_texture_render_target = nullptr;


memory::SwapVmt::Shared m_base_client_swap = { };
memory::SwapVmt::Shared m_prediction_swap = { };
memory::SwapVmt::Shared m_view_render_swap = { };
memory::SwapVmt::Shared m_render_view_swap = { };
memory::SwapVmt::Shared m_direct_device_swap = { };


auto Create() -> bool
{
	auto& input = win32::Input::Instance();
	auto& shared = core::Shared::Instance();

	auto& netprop_manager = engine::NetPropManager::Instance();
	auto& renderer = engine::Renderer::Instance();

	auto& gui = feature::Gui::Instance();
	auto& aimbot = feature::Aimbot::Instance();
	auto& triggerbot = feature::Triggerbot::Instance();

	if( !option::Create() )
		return false;

	if( !input.Create() )
		return false;

	if( !shared.Create( input ) )
		return false;

	engine::Factory factory_client( "client.dll" );
	engine::Factory factory_engine( "engine.dll" );
	engine::Factory factory_vstdlib( "vstdlib.dll" );
	engine::Factory factory_vphysics( "vphysics.dll" );
	engine::Factory factory_materialsystem( "materialsystem.dll" );

	m_base_client = factory_client.Get< IBaseClientDLL* >( "VClient" );
	m_client_entity_list = factory_client.Get< IClientEntityList* >( "VClientEntityList" );
	m_game_movement = factory_client.Get< IGameMovement* >( "GameMovement" );
	m_prediction = factory_client.Get< IPrediction* >( "VClientPrediction" );

	m_engine_client = factory_engine.Get< IVEngineClient* >( "VEngineClient" );
	m_model_info_client = factory_engine.Get< IVModelInfoClient* >( "VModelInfoClient" );
	m_render_view = factory_engine.Get< IVRenderView* >( "VEngineRenderView" );
	m_model_render = factory_engine.Get< IVModelRender* >( "VEngineModel" );
	m_engine_trace = factory_engine.Get< IEngineTrace* >( "EngineTraceClient" );

	m_cvar = factory_vstdlib.Get< ICvar* >( "VEngineCvar" );
	m_mem_alloc = *( IMemAlloc** )GetProcAddress( GetModuleHandleA( "tier0.dll" ), "g_pMemAlloc" );

	m_physics_surface_props = factory_vphysics.Get< IPhysicsSurfaceProps* >( "VPhysicsSurfaceProps" );

	m_material_system = factory_materialsystem.Get< IMaterialSystem* >( "VMaterialSystem" );

	m_move_helper = memory::scan< IMoveHelper* >( "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01", 2, 2u );
	m_view_render = memory::scan< IViewRender* >( "client.dll", "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10", 1, 1u );
	m_input = memory::scan< IInput* >( "client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85", 1, 1u );

	m_game_event_manager = factory_engine.Get< IGameEventManager* >( "GAMEEVENTSMANAGER" );

	m_globals = memory::scan< CGlobalVarsBase* >( "client.dll", "A1 ? ? ? ? 5E 8B 40 10", 1, 2u );
	m_client_state = **( CBaseClientState*** )( ( *( std::uintptr_t** )m_engine_client )[ 12 ] + 0x0010 );

	m_direct_device = memory::scan< IDirect3DDevice9* >( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C", 1, 2u );

	if( !netprop_manager.Create( m_base_client ) )
		return false;

	if( !renderer.Create( input.GetWindow(), m_direct_device ) )
		return false;

	if( !gui.Create( input ) )
		return false;

	if( !aimbot.Create( input ) )
		return false;

	if( !triggerbot.Create( input ) )
		return false;

	m_game_event.Create();

	m_material_system->ForceBeginRenderTargetAllocation();
	m_texture_render_target = m_material_system->CreateFullFrameRenderTarget( "texture_rt" );
	m_material_system->ForceEndRenderTargetAllocation();

	m_base_client_swap = std::make_shared< memory::SwapVmt >( m_base_client );
	m_prediction_swap = std::make_shared< memory::SwapVmt >( m_prediction );
	m_view_render_swap = std::make_shared< memory::SwapVmt >( m_view_render );
	m_render_view_swap = std::make_shared< memory::SwapVmt >( m_render_view );
	m_direct_device_swap = std::make_shared< memory::SwapVmt >( m_direct_device );

	m_base_client_swap->Hook( &hooked::CreateMove, index::IBaseClientDLL::CreateMove );
	m_base_client_swap->Hook( &hooked::FrameStageNotify, index::IBaseClientDLL::FrameStageNotify );

	m_prediction_swap->Hook( &hooked::RunCommand, index::IPrediction::RunCommand );

	m_view_render_swap->Hook( &hooked::RenderView, index::IViewRender::RenderView );

	m_render_view_swap->Hook( &hooked::SceneEnd, index::IVRenderView::SceneEnd );

	DWORD_PTR* Present_V = memory::scan< DWORD_PTR* >( "gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB 74 1F", 2, 1u );

	// Save the original, this is used at the end of our hooked function            
	hooked::oPresent = ( hooked::PresentFn )( *Present_V );

	// Set steam's hook to ours
	*Present_V = ( DWORD_PTR )( &hooked::Present );

	m_direct_device_swap->Hook( &hooked::Reset, index::IDirect3DDevice9::Reset );
	// m_direct_device_swap->Hook( &hooked::Present, index::IDirect3DDevice9::Present );

	return true;
}

auto Destroy() -> void
{
	auto& input = win32::Input::Instance();
	auto& shared = core::Shared::Instance();

	auto& netprop_manager = engine::NetPropManager::Instance();
	auto& renderer = engine::Renderer::Instance();

	auto& gui = feature::Gui::Instance();
	auto& aimbot = feature::Aimbot::Instance();
	auto& triggerbot = feature::Triggerbot::Instance();

	m_base_client_swap->Destroy();
	m_prediction_swap->Destroy();
	m_view_render_swap->Destroy();
	m_render_view_swap->Destroy();
	m_direct_device_swap->Destroy();

	m_game_event.Destroy();

	triggerbot.Destroy();
	aimbot.Destroy();
	gui.Destroy();

	renderer.Destroy();
	netprop_manager.Destroy();

	shared.Destroy();
	input.Destroy();

	option::Destroy();
}

auto Free() -> void
{
	auto& renderer = engine::Renderer::Instance();

	renderer.Free();
}

}