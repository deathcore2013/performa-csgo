#include "factory.hpp"

namespace source
{

namespace engine
{

Factory::Factory( const std::string& image_name )
	: m_image( GetModuleHandleA( image_name.c_str() ) )
{
	CreateFactoryMap();
}

auto Factory::CreateFactoryMap() -> void
{
	auto create_interface = reinterpret_cast< std::uintptr_t >( GetProcAddress( m_image, "CreateInterface" ) );

	if( !create_interface )
		return;

	auto jump = *reinterpret_cast< std::uintptr_t* >( create_interface + 0x05 );

	if( jump & ( 1 << 0x1F ) )
		jump = 0xFFFFFFF1 - jump;
	else
		jump = -0x06;

	if( jump != 0xFFFFFFFA && jump > 0x002000 )
		return;

	m_interface_array = **( InterfaceReg*** )( create_interface - jump );

	if( !m_interface_array )
		return;

	for( auto factory = m_interface_array; factory; factory = factory->m_pNext )
	{
		auto factory_name = std::string( factory->m_pName );
		factory_name = factory_name.substr( 0u, factory_name.size() - 3u );

		auto factory_data = factory->m_CreateFn();

		m_factory_map[ factory_name ] = factory_data;
	}
}

}

}