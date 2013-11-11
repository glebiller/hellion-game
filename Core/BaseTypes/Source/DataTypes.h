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

typedef float               f32;
typedef double              f64;

typedef int                 i32;
typedef long long           i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;

#ifdef _MSC_VER
typedef __int64                 int64;
typedef unsigned __int64        uint64;
#else
typedef long long               int64;
typedef unsigned long long      uint64;
#endif

typedef void*               Handle;
typedef u32                 Error;
typedef u32                 Id;

typedef void                (*Callback)(void* pUserData);
