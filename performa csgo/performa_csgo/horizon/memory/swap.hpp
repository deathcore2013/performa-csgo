#pragma once

#include "base.hpp"

namespace horizon
{

namespace memory
{

class SwapVmt
{
public:
	using Shared = std::shared_ptr< SwapVmt >;

public:
	SwapVmt( void* instance = nullptr );
	~SwapVmt();

public:
	auto Create( void* instance ) -> bool;
	auto Destroy() -> void;

	auto Set( bool state ) -> bool;
	auto Hook( void* hooked, std::size_t index ) -> bool;

	template< typename T >
	inline auto Get( std::size_t index ) -> T
	{
		if( !m_restore || index >= m_size )
			return ( T )nullptr;

		return ( T )m_restore[ index ];
	}

private:
	std::uintptr_t** m_instance = nullptr;
	std::uintptr_t* m_restore = nullptr;
	std::unique_ptr< std::uintptr_t[] > m_replace = nullptr;
	std::size_t m_size = 0u;
};

}

}