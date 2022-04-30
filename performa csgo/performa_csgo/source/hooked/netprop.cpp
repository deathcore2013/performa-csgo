#include "procedure.hpp"

#include "../feature/resolver.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

namespace source
{

namespace hooked
{

auto DT_CSPlayer_m_flLowerBodyYawTarget( const CRecvProxyData* recv_proxy_data, void* data, void* output ) -> void
{
	auto& resolver = feature::Resolver::Instance();

	auto player = ( C_CSPlayer* )data;

	if( player->IsGood() )
	{

	}
}

}

}