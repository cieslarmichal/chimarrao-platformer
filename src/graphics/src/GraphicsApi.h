#pragma once

#ifdef _WIN32
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API
#endif