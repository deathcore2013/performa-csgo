#pragma once

#include "../../valve/sdk.hpp"

namespace source
{

namespace engine
{

class Factory
{
	using factory_map_t = std::unordered_map< std::string, void* >;

public:
	Factory( const std::string& image_name );

public:
	template< typename T >
	inline auto Get( const std::string& name )
	{
		auto factory = m_factory_map[ name ];
		return ( T )factory;
	}

private:
	auto CreateFactoryMap() -> void;

private:
	HMODULE m_image = nullptr;
	InterfaceReg* m_interface_array = nullptr;
	factory_map_t m_factory_map = { };
};

}

}