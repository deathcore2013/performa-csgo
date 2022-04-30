#pragma once

#include "../valve/sdk.hpp"

namespace source
{

extern IBaseClientDLL* m_base_client;
extern IClientEntityList* m_client_entity_list;
extern IGameMovement* m_game_movement;
extern IPrediction* m_prediction;

extern IVEngineClient* m_engine_client;
extern IVModelInfoClient* m_model_info_client;
extern IVRenderView* m_render_view;
extern IVModelRender* m_model_render;
extern IEngineTrace* m_engine_trace;

extern ICvar* m_cvar;
extern IMemAlloc* m_mem_alloc;

extern IPhysicsSurfaceProps* m_physics_surface_props;

extern IMaterialSystem* m_material_system;

extern IMoveHelper* m_move_helper;
extern IViewRender* m_view_render;
extern IInput* m_input;

extern IGameEventManager* m_game_event_manager;

extern CGlobalVarsBase* m_globals;
extern CBaseClientState* m_client_state;

extern IDirect3DDevice9* m_direct_device;


extern ITexture* m_texture_render_target;


extern memory::SwapVmt::Shared m_base_client_swap;
extern memory::SwapVmt::Shared m_prediction_swap;
extern memory::SwapVmt::Shared m_view_render_swap;
extern memory::SwapVmt::Shared m_render_view_swap;
extern memory::SwapVmt::Shared m_direct_device_swap;


auto Create() -> bool;
auto Destroy() -> void;
auto Free() -> void;

}