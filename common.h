#ifndef __COMMON__H__
#define __COMMON__H__

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

//====================================================================================================================================
// ERROR & DEBUG related

#define DEBUG

// DEBUG_BREAK
#ifdef  WIN32
#   define  CMN_DEBUG_BREAK()    __debugbreak()
#else
#   define  CMN_DEBUG_BREAK()    asm("int $3")
#endif


// LOG
#define CMN_LOG( _message, ... ) do{ fprintf( stdout, (_message), __VA_ARGS__ );fflush( stdout ); } while(0)

// ASSERT
#ifdef DEBUG
#   define CMN_ASSERT(_assert, ...)   \
        if( !(_assert) ) {  \
            CMN_LOG( "Assertion failed !\n.FILE = '%s'\n.LINE='%d'\n.FUNCTION='%s'\n", __FILE__ , __LINE__, __FUNCTION__);\
            CMN_LOG( __VA_ARGS__ );\
            CMN_DEBUG_BREAK();\
            assert(_assert); \
        }
#else
#   define TP_ASSERT(_assert, ...)
#endif


#endif
