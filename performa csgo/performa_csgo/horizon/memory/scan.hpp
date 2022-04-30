#pragma once

#include "base.hpp"

namespace horizon
{

namespace memory
{

namespace detail
{

template< typename T >
bool is_in_range( const T& data, const T& min, const T& max )
{
	return ( data >= min && data <= max );
}

template< typename T >
auto get_bits( const T& data )
{
	auto min = ( T )'0';
	auto max = ( T )'9';

	if( is_in_range( data, min, max ) )
		return ( data - ( T )'0' );

	return ( ( data & ~0x0020 ) - ( T )'A' + 0x000A );
}

template< typename T >
auto get_byte( const T& data )
{
	auto data_array = ( std::uint8_t* )data;
	return ( get_bits( data_array[ 0 ] ) << 4 | get_bits( data_array[ 1 ] ) );
}

}

template< typename T >
T scan_region( std::uintptr_t region_begin, std::uint32_t region_size, const std::string& signature, std::intptr_t displacement = 0, std::size_t read = 0u )
{
	if( !region_begin || !region_size || signature.empty() )
		return ( T )0;

	auto scan_begin = ( std::uint8_t* )region_begin;
	auto scan_end = ( std::uint8_t* )scan_begin + region_size;

	auto scan_data = ( std::uint8_t* )signature.c_str();

	std::uint8_t* scan_point = nullptr;

	for( auto scan_current = scan_begin; scan_current < scan_end; scan_current += 1u )
	{
		if( *scan_data == '\?' || *scan_current == detail::get_byte( scan_data ) )
		{
			if( !scan_point )
				scan_point = scan_current;

			if( !scan_data[ 2 ] )
				break;

			scan_data += ( *( std::uint16_t* )scan_data == '\?\?' ||
						   *( std::uint8_t* )scan_data != '\?' ) ? 3u : 2u;

			if( !*scan_data )
				break;
		}
		else if( scan_point )
		{
			scan_current = scan_point;
			scan_data = ( std::uint8_t* )signature.c_str();
			scan_point = nullptr;
		}
	}

	if( scan_point )
	{
		scan_point += displacement;

		for( auto i = 0u; i < read; i++ )
		{
			scan_point = ( std::uint8_t* )( *( std::uintptr_t* )scan_point );

			if( !scan_point )
				break;
		}
	}

	return ( T )scan_point;
}

template< typename T >
T scan( const std::string& image_name, const std::string& signature, std::intptr_t displacement = 0, std::size_t read = 0u )
{
	auto image = ( std::uintptr_t )GetModuleHandleA( image_name.c_str() );
	auto dos_hdr = ( IMAGE_DOS_HEADER* )image;
	auto nt_hdrs = ( IMAGE_NT_HEADERS* )( image + dos_hdr->e_lfanew );

	return memory::scan_region< T >( image, nt_hdrs->OptionalHeader.SizeOfImage, signature, displacement, read );
}

}

}