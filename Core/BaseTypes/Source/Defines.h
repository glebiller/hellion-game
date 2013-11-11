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
// Compiler options
//
#if defined(_MSC_VER)
#define MSC_COMPILER
#elif defined(__GNUC__)
#define GCC_COMPILER
#else
#pragma error "Unkown compiler"
#endif

//
// Windows Specific
// 
#define WIN32_LEAN_AND_MEAN
#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0501
#endif

//
// Build options
//
#if defined(_DEBUG)
#define DEBUG_BUILD
#endif

#define LOGGER_ENABLED

// The current mechanism of by-job statistics does not work correctly in case of
// nested parallelism, since work-stealing TBB scheduler may (and does) interleave
// different jobs on the same thread. To make it work correctly the statistics
// should be accumulated on a per-thread basis. Besides with the fine grained job
// tasks (which are necessary to achieve acceptable load balancing) the overhead
// of statistics tracking may become noticeable.
#define STATISTICS_BY_JOB_TYPE

#define USE_SPIN_MUTEX 1

//
// Log level
// 
#undef LOGOG_LEVEL
#define LOGOG_LEVEL LOGOG_LEVEL_WARN

//
// Custom values
//
#if !defined(NULL)
#define NULL nullptr
#endif

//
// stdcall
//
#if !defined(MSC_COMPILER)
#define __stdcall
#endif

//
// Unused param
//
#if defined(MSC_COMPILER)
#define UNUSED_PARAM(p) (p);
#else
#define UNUSED_PARAM(p)
#endif
