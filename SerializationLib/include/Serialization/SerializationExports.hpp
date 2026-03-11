#pragma once

#ifdef SERIZ_EXPORTS
	#define SERIZ_API __declspec(dllexport)
#else
	#define SERIZ_API __declspec(dllimport)
#endif