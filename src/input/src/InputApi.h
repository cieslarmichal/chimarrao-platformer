#pragma once

#ifdef _WIN32
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API
#endif