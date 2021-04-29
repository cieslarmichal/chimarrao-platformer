#pragma once

#ifdef _WIN32
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API
#endif