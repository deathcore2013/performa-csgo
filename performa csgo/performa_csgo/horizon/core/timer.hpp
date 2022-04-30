#pragma once

#include "base.hpp"

namespace horizon
{

namespace core
{

class Timer
{
public:
	Timer( bool start = false )
	{
		if( start )
			Reset();
	}

public:
	auto Reset() -> void
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	auto Elapsed() -> long long
	{
		auto elapsed = std::chrono::high_resolution_clock::now() - m_start;
		return std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count();
	}

private:
	std::chrono::high_resolution_clock::time_point m_start = { };
};

}

}