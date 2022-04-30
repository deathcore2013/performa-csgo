#pragma once

#include "../../valve/sdk.hpp"

struct ImColor;

namespace source
{

namespace hooked
{

extern bool g_chams;

auto ForceMaterial( const ImColor& color, IMaterial* mat, bool color_change = true ) -> void;
auto CreateMaterialMemory( bool texture, bool ignore_z )->IMaterial*;

using FrameStageNotifyFn = void( __thiscall* )( void*, ClientFrameStage_t );

using RunCommandFn = void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, IMoveHelper* );

using RenderViewFn = void( __thiscall* )( void*, CViewSetup&, CViewSetup&, int, int );

using SceneEndFn = void( __thiscall* )( void* );

using ResetFn = HRESULT( api_d3d* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
using PresentFn = HRESULT( api_d3d* )( IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA* );

extern PresentFn oPresent;

auto __fastcall CreateMove( int sequence_number, float input_sample_frametime, bool active ) -> void;
auto __fastcall FrameStageNotify( void* ecx, void* edx, ClientFrameStage_t curStage ) -> void;

auto __fastcall RunCommand( void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper ) -> void;

auto __fastcall RenderView( void* ecx, void* edx, CViewSetup& view, CViewSetup& hudViewSetup, ClearFlags_t nClearFlags, RenderViewInfo_t whatToDraw ) -> void;

auto __fastcall SceneEnd( void* ecx, void* edx ) -> void;

auto api_d3d Reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters ) -> HRESULT;
auto api_d3d Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region ) -> HRESULT;



auto DT_CSPlayer_m_flLowerBodyYawTarget( const CRecvProxyData* recv_proxy_data, void* data, void* output ) -> void;

}

}