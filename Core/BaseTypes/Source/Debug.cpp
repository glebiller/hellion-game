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

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif

#include "Assert.h"
#include "Debug.h"

#ifdef DEBUG_BUILD

#include <cstdio>
#include <ctime>

#include "boost/thread/locks.hpp"

#define MAX_STRING_LENGTH 2048

/**
 * The log file names.
 */
LogFile s_LogFiles[ LogType::e_LogTypeCount ] = {
    { NULL, "Debug.log",           ""          },
    { NULL, "Debug_AI.log",        "AI"        },
    { NULL, "Debug_Animation.log", "Animation" },
    { NULL, "Debug_Audio.log",     "Audio"     },
    { NULL, "Debug_Geometry.log",  "Geometry"  },
    { NULL, "Debug_Graphics.log",  "Graphics"  },
    { NULL, "Debug_Input.log",     "Input"     },
    { NULL, "Debug_Network.log",   "Network"   },
    { NULL, "Debug_Physics.log",   "Physics"   },
};

COMPILE_ASSERT(sizeof(s_LogFiles) / sizeof(s_LogFiles[ 0 ]) == LogType::e_LogTypeCount);

/**
 * @inheritDoc
 */
Debug::Debugger::Debugger(bool bLogging) {
    m_bLogging = bLogging;

    if (m_bLogging) {
        // Copy the s_LogFiles into the instance
        memcpy(m_LogFiles, s_LogFiles, sizeof(s_LogFiles));
        // Create a directory with the current timestamp
        // ( Format: Month_Day_HourMinute )
        /*time_t Time;
        time(&Time);
        tm Date;
        localtime_s(&Date, &Time);
        char FolderName[ MAX_STRING_LENGTH ];
        sprintf_s(FolderName, MAX_STRING_LENGTH, "..\\Logs\\%.2d%.2d%.2d%.2d\\", (Date.tm_mon + 1), Date.tm_mday, Date.tm_hour, Date.tm_min);
        int Result = _mkdir(FolderName);
        ASSERT(Result == 0);

        if (Result == 0) {*/
            // Open all the log files
            for (u8 Index = 0; Index < LogType::e_LogTypeCount; Index++) {
                char FileName[ MAX_STRING_LENGTH ];
                strncpy_s(FileName, MAX_STRING_LENGTH, "logs", MAX_STRING_LENGTH - strlen(m_LogFiles[ Index ].FileName));
                strcat_s(FileName, MAX_STRING_LENGTH, m_LogFiles[ Index ].FileName);
                fopen_s(&m_LogFiles[ Index ].FileHandle, FileName, "w");
            }
        //}
    }
}

/**
 * @inheritDoc
 */
Debug::Debugger::~Debugger() {
    if (m_bLogging) {
        // Close all the log files
        for (u8 Index = 0; Index < LogType::e_LogTypeCount; Index++) {
            fclose(m_LogFiles[ Index ].FileHandle);
        }
    }
}

/**
 * @inheritDoc
 */
void Debug::Print(const char* Format, ...) {
    va_list ArgList;
    va_start(ArgList, Format);
    s_Debugger->Print(Format, ArgList);
    va_end(ArgList);
}

/**
 * @inheritDoc
 */
void Debug::Debugger::Print(const char* Format, va_list ArgList) {
    boost::lock_guard<boost::mutex> lock(mutex);
    vprintf(Format, ArgList);
}

/**
 * @inheritDoc
 */
void Debug::Debugger::Log(LogType::LogType Type, const char* Format, va_list ArgList) {
    if (!m_bLogging) {
        return;
    }

    ASSERT(m_LogFiles[ Type ].FileHandle);
    boost::lock_guard<boost::mutex> lock(mutex);

    // Write to log file
    vfprintf(m_LogFiles[ Type ].FileHandle, Format, ArgList);
    fflush(m_LogFiles[ Type ].FileHandle);
    
    // Ouput to stdout
    vprintf(Format, ArgList);

    // If this is a system specific log file, also log it to the general log
    if (Type != LogType::e_Debug) {
        ASSERT(m_LogFiles[ LogType::e_Debug ].FileHandle);
        char Buffer[ MAX_STRING_LENGTH ];
        vsprintf(Buffer, Format, ArgList);
        fprintf(m_LogFiles[ LogType::e_Debug ].FileHandle, "[%s] %s", m_LogFiles[ Type ].SystemName, Buffer);
        fflush(m_LogFiles[ LogType::e_Debug ].FileHandle);
    }
}

#endif