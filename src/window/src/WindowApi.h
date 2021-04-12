#pragma once

#ifdef _WIN32
#define WINDOW_API __declspec(dllexport)
#else
#define WINDOW_API
#endif