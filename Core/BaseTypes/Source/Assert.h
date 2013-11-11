// Copyright © 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#pragma once

#include "Defines.h"

#ifdef ASSERT
    #undef ASSERT
#endif
#ifdef assert
    #undef assert
#endif

#ifdef DEBUG_BUILD
    #ifdef _MSC_VER
        #include <crtdbg.h>
        #define ASSERT(c)                          (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, #c )) || (__debugbreak(), 0))
        #define ASSERTMSG(c, m)                    (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, m )) || (__debugbreak(), 0))
        #define ASSERTMSG1(c,  m, p1)              (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, m, p1 )) || (__debugbreak(), 0))
        #define ASSERTMSG2(c,  m, p1, p2)          (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, m, p1, p2 )) || (__debugbreak(), 0))
        #define ASSERTMSG3(c,  m, p1, p2, p3)      (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, m, p1, p2, p3 )) || (__debugbreak(), 0))
        #define ASSERTMSG4(c,  m, p1, p2, p3, p4)  (void) ((!!(c)) || (1 != _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, nullptr, m, p1, p2, p3, p4 )) || (__debugbreak(), 0))
    #else
        #define ASSERT(c)                         
        #define ASSERTMSG(c, m)                   
        #define ASSERTMSG1(c,  m, p1)              
        #define ASSERTMSG2(c,  m, p1, p2)          
        #define ASSERTMSG3(c,  m, p1, p2, p3)      
        #define ASSERTMSG4(c,  m, p1, p2, p3, p4)  
    #endif
#else
    #define ASSERT(c)
    #define ASSERTMSG(c, m)
    #define ASSERTMSG1(c,  m, p1)
    #define ASSERTMSG2(c,  m, p1, p2)
    #define ASSERTMSG3(c,  m, p1, p2, p3)
    #define ASSERTMSG4(c,  m, p1, p2, p3, p4)
#endif

#define COMPILE_ASSERT(e)							typedef char __FILE____LINE__[ (e) ? 1 : -1 ]
#define assert(c)									ASSERT(c)