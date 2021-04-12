#pragma once

#ifdef _WIN32
#define ANIMATIONS_API __declspec(dllexport)
#else
#define ANIMATIONS_API
#endif