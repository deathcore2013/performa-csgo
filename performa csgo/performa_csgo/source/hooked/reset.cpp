#include "procedure.hpp"

#include "../engine.hpp"
#include "../engine/renderer.hpp"

namespace source
{

namespace hooked
{

auto api_d3d Reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters ) -> HRESULT
{
	auto& renderer = engine::Renderer::Instance();
	renderer.DeviceLost();

	auto fn = ( source::m_direct_device_swap ?
				source::m_direct_device_swap->Get< ResetFn >( index::IDirect3DDevice9::Reset ) :
				memory::vget< ResetFn >( device, index::IDirect3DDevice9::Reset ) );

	auto code = fn( device, presentation_parameters );

	renderer.DeviceReset( code );
	return code;
}

}

}