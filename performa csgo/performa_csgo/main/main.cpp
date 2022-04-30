#include "../source/engine.hpp"
#include "../option/storage.hpp"

auto ImageQuit( bool synchronize ) -> void
{
	source::Destroy();

	if( synchronize )
		std::this_thread::sleep_for( 1s );

	source::Free();
}

auto ImageFree( HMODULE image, unsigned long code = 0u ) -> unsigned long
{
	FreeLibraryAndExitThread( image, code );
	return code;
}

auto ImageLoad( core::Data* data ) -> unsigned long
{
	auto& shared = core::Shared::Instance();

	if( source::Create() )
	{
		while( !shared.Quit() )
		{
			auto execute_state = shared.GetExecuteState();

			switch( execute_state )
			{
				case core::ExecuteLoad:
				{
					option::Load( shared.GetExecuteDirectory() );
					break;
				}
				case core::ExecuteSave:
				{
					option::Save( shared.GetExecuteDirectory() );
					break;
				}
			}

			std::this_thread::sleep_for( 100ms );
		}

		ImageQuit( true );
	}

	return ImageFree( data->m_image, EXIT_SUCCESS );
}

auto GetImageDirectory( HMODULE image, char* output ) -> bool
{
	char image_name_data[ MAX_PATH ] = { };
	if( !GetModuleFileNameA( image, image_name_data, MAX_PATH ) )
		return false;

	auto image_name = std::string( image_name_data );
	image_name = image_name.substr( 0u, image_name.find_last_of( '\\' ) );

	if( output )
	{
		std::memcpy( output, image_name.c_str(), image_name.size() * sizeof( char ) );
		return true;
	}

	return false;
}
auto GetImageDirectory( HMODULE image, wchar_t* output ) -> bool
{
	wchar_t image_name_data[ MAX_PATH ] = { };
	if( !GetModuleFileNameW( image, image_name_data, MAX_PATH ) )
		return false;

	auto image_name = std::wstring( image_name_data );
	image_name = image_name.substr( 0u, image_name.find_last_of( L'\\' ) );

	if( output )
	{
		std::memcpy( output, image_name.c_str(), image_name.size() * sizeof( wchar_t ) );
		return true;
	}

	return false;
}

auto OnImageLoad( HMODULE image, void* reserved ) -> void
{
	auto data = reinterpret_cast< core::Data* >( reserved );

	if( data )
	{
		std::memcpy( &core::m_data, ( const void* )( ( std::uintptr_t )data + 8 ), sizeof( core::Data ) );
	}
	else
	{
		data = &core::m_data;

		GetImageDirectory( image, core::m_data.m_loader_directory );
		GetImageDirectory( image, core::m_data.m_loader_directory_unicode );
	}

	core::m_data.m_image = image;

	DisableThreadLibraryCalls( image );
	CreateThread( nullptr, 0u, ( LPTHREAD_START_ROUTINE )ImageLoad, data, 0u, nullptr );
}

auto OnImageFree() -> void
{
	ImageQuit( false );
}

auto win32_image_entry( HMODULE image, unsigned long reason, void* reserved ) -> int
{
	switch( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			OnImageLoad( image, reserved );
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			OnImageFree();
			break;
		}
	}

	return TRUE;
}