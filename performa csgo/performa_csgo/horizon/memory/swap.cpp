#include "swap.hpp"

namespace horizon
{

namespace memory
{

SwapVmt::SwapVmt( void* instance /*= nullptr*/ )
{
	if( instance )
		Create( instance );
}

SwapVmt::~SwapVmt()
{
	Destroy();
}

auto SwapVmt::Create( void* instance ) -> bool
{
	m_instance = reinterpret_cast< std::uintptr_t** >( instance );

	if( !m_instance )
	{
		return false;
	}

	m_restore = *m_instance;

	if( !m_restore )
	{
		return false;
	}

	while( m_restore[ m_size ] )
		m_size++;

	if( !m_size )
	{
		return false;
	}

	m_replace = std::make_unique< std::uintptr_t[] >( m_size );
	std::memcpy( m_replace.get(), m_restore, m_size * sizeof( std::uintptr_t ) );

	if( !Set( true ) )
	{
		return false;
	}

	return true;
}

auto SwapVmt::Destroy() -> void
{
	Set( false );

	m_instance = nullptr;
	m_restore = nullptr;
	m_replace.reset();
	m_size = 0u;
}

auto SwapVmt::Set( bool state ) -> bool
{
	if( !m_instance || !m_restore || !m_replace )
	{
		return false;
	}

	auto data = state ? m_replace.get() : m_restore;
	return ( *m_instance = data );
}

auto SwapVmt::Hook( void* hooked, std::size_t index ) -> bool
{
	if( !m_replace )
	{
		return false;
	}

	return ( m_replace[ index ] = ( std::uintptr_t )hooked );
}

}

}