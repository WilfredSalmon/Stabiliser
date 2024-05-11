#pragma once

#include <version>

#define FST_STRINGIFY_X( X ) #X
#define FST_STRINGIFY( X ) FST_STRINGIFY_X( X )

#ifdef _MSC_VER
#define FST_MSVC_PUSH_AND_DISABLE_WARNINGS( ... ) \
	_Pragma( "warning( push )" ) \
	_Pragma( FST_STRINGIFY( warning( disable : ##__VA_ARGS__ ## ) ) )

#define FST_MSVC_POP_WARNINGS _Pragma( "warning( pop )" )

#else
#define FST_MSVC_PUSH_AND_DISABLE_WARNINGS( ... )
#define FST_MSVC_POP_WARNINGS
#endif

#ifdef __GNUC__
#define FST_GCC_PUSH_AND_DISABLE_WARNINGS(...)\
	_Pragma( "GCC diagnostic push" ) \
	_Pragma( FST_STRINGIFY( GCC warning ignored ##__VA_ARGS__ ) )

#define FST_GCC_POP_WARNINGS _Pragma( "GCC warning pop" )
#else
#define FST_GCC_PUSH_AND_DISABLE_WARNINGS(...)
#define FST_GCC_POP_WARNINGS
#endif

#ifndef __cpp_size_t_suffix 
// Added in C++23, we can mimic it ourselves
// Disable warning for user defined literal not starting with _, we know this one is safe since we compile with C++20
FST_MSVC_PUSH_AND_DISABLE_WARNINGS( 4455 )
FST_GCC_PUSH_AND_DISABLE_WARNINGS( "-Wno-c++23-extensions" )
constexpr std::size_t operator""uz( unsigned long long n )
{
	return n;
}
FST_MSVC_POP_WARNINGS
FST_GCC_POP_WARNINGS
#endif

#define FST_NO_UNIQUE_ADDRESS [[no_unique_address]] [[msvc::no_unique_address]]
