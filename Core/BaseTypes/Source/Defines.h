// Copyright � 2008-2009 Intel Corporation
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

//
// Build options
//
#if defined( _DEBUG )
	#define DEBUG_BUILD
#endif

//
// Compiler options
//
#if defined( _MSC_VER )
	#define MSC_COMPILER
#elif defined( __GNUC__ )
	#define GCC_COMPILER
#else
	#pragma error "Unkown compiler"
#endif

//
// Custom values
//
#ifndef NULL
	#define NULL                            0
#endif

//
// Easy macro
//
#define UNREFERENCED_PARAM(P)               (P)
#define SAFE_DELETE(p)                    	if ((p)!=NULL){delete (p); (p)=NULL;}
#define SAFE_DELETE_ARRAY(p)              	if ((p)!=NULL){delete [] (p); (p)=NULL;}

//
// In Out InOut Inline
//
#ifdef MSC_COMPILER
	#define In                              __in const
	#define Out                             __out
	#define InOut                           __inout
#else
	#define In                              const
	#define Out
	#define InOut
	#define __forceinline                   inline __attribute__((always_inline))
#endif

//
// stdcall
//
#ifndef MSC_COMPILER
#define __stdcall
#endif
