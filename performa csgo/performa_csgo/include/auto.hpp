#pragma once

#include "win32.hpp"

#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <deque>
#include <mutex>
#include <map>

using namespace std::chrono_literals;
using namespace std::string_literals;

#define api_nt __stdcall
#define api_win32 __stdcall
#define api_d3d __stdcall

#define win32_image_entry api_win32 DllMain