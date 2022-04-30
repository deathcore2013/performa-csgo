#pragma once

#include "base.hpp"

#define horizon_concat( x, y ) x ## y
#define horizon_concatiate( x, y ) horizon_concat( x, y )
#define horizon_pad( size ) private: std::uint8_t horizon_concatiate( __pad, __COUNTER__ )[ size ] = { }; public:

namespace horizon
{

namespace memory
{

template< typename T >
inline auto vget( void* instance, unsigned int index ) -> T
{
	auto procedure_array = *reinterpret_cast< std::uintptr_t** >( instance );
	return ( T )procedure_array[ index ];
}

template< typename T >
inline auto read( std::uintptr_t address ) -> T
{
	return *( T* )address;
}

template< typename T >
inline auto write( std::uintptr_t address, const T& data ) -> T
{
	*( T* )address = data;
}

}

}