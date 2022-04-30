#pragma once

#include "base.hpp"

namespace horizon
{

namespace core
{

struct Data
{
	HMODULE m_image = nullptr;
	char m_loader_directory[ MAX_PATH ] = { };
	wchar_t m_loader_directory_unicode[ MAX_PATH ] = { };
};

extern Data m_data;

}

}