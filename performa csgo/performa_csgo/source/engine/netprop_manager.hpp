#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

class NetPropManager : public core::Singleton< NetPropManager >
{
public:
	auto Create( IBaseClientDLL* base_client ) -> bool;
	auto Destroy() -> void;

	auto Get( const std::string& array_name, const std::string& prop_name, RecvProp** prop_out = nullptr ) -> int;

private:
	auto Get( RecvTable* recv_entry, const std::string& prop_name, RecvProp** prop_out = nullptr ) -> int;
	auto GetArrayEntry( const std::string& array_name ) -> RecvTable*;

private:
	std::vector< RecvTable* > m_recv_array = { };
};

}

}