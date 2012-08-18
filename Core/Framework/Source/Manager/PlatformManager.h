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

#include <vector>

#include "google/protobuf/message.h"
#include "Proto/Common/System.pb.h"

#include "Defines.h"
#include "DataTypes.h"
#include "Errors.h"
#include "IPlatform.h"
#include "Singleton.h"

class ISystem;

/**
 * Abstraction class for OS specific functionality.
 * 
 * @sa  IPlatform
 * @sa  Singleton
 */
class PlatformManager : public IPlatform, public Singleton {
    public:

        /**
         * Constructor.
         */
        PlatformManager(void);

        /**
         * Destructor.
         */
        ~PlatformManager(void);

    public:

        /**
         * Provides OS file system functionality.
         */
        class FileSystem {
                friend class PlatformManager;

            protected:

                /**
                 * Constructor.
                 *
                 * ### remarks  Only accessible via the PlatformMananger.
                 */
                FileSystem(void);

                /**
                 * Destructor.
                 *
                 * ### remarks  Only accessible via the PlatformMananger.
                 */
                ~FileSystem(void);

            public:

                /**
                 * Loads a system library and returns pointers to the system.
                 *
                 * @param   type        The system type to load.
                 * @param   ppSystem    Returned pointer to the ISystem implementation. Remains untouched on
                 *                      failure.
                 * @return  An error code.
                 */
                Error LoadSystemLibrary(SystemProto::Type type, ISystem** ppSystem);

                /**
                 * Loads a Proto file and create a Proto object.
                 *
                 * @param   pszFile         Filename of the Proto file to load.
                 * @param [in,out]  proto   Return the pointer to the Proto implementation object. Remains
                 *                          untouched on failure.
                 * @return  An error code.
                 */
                Error LoadProto(const char* pszFile, google::protobuf::Message* proto);

                /**
                 * Verfies the existence of a file for read access.
                 *
                 * @param   pszFileName The file name to look for.
                 * @return  true if successful, otherwise false.
                 */
                bool FileExists(In char* pszFileName);

                /**
                 * Sets the current directory to the location of a file.
                 *
                 * @param   pszFileName     The file name to look for.
                 * @param   apszLocations   A null terminated array of relative search directories.
                 * @param   pszCurrentDir   Location to store the new current directory or NULL.
                 * @param   BufferSize      The size of <c>pszCurrentDir</c>.
                 * @return  true if successful, otherwise false.
                 */
                bool SetCurrentDirToFileLocation(In char* pszFileName, In char* apszLocations[], Out char* pszCurrentDir = NULL, u32 BufferSize = 0);


            protected:

                struct SystemLib {
                    Handle                              hLib;
                    ISystem*                            pSystem;
                };
                std::vector<SystemLib>                  m_SystemLibs;
        };

        /**
         * Provides OS window system functionality.
         * 
         * @sa  IPlatform::IWindow
         */
        class WindowSystem : public IPlatform::IWindow {
                friend class PlatformManager;

            protected:

                /**
                 * Constructor.
                 * Only accessible via the PlatformMananger.
                 */
                WindowSystem(void);

            public:

                /**
                 * Processes any waiting window messages.
                 */
                void ProcessMessages(void);

                /**
                 * Set the windows handle.
                 *
                 * @param   windowHnd   Handle of the window.
                 */
                void SetHandle(size_t windowHnd);

                /**
                 * Get the windows handle.
                 *
                 * @return  The handle.
                 */
                size_t GetHandle();

                /**
                 * Sets a render window.
                 *
                 * @param   renderWindow    Handle of the render window.
                 */
                void SetRenderWindow(Handle renderWindow);

                /**
                 * Gets the render window.
                 *
                 * @return  The render window.
                 */
                Handle GetRenderWindow(void);

            private:

                size_t          m_WindowHnd;
                Handle          m_renderWindow;

        };

        /**
         * Provides OS timer functionality.
         */
        class Timers {
                friend class PlatformManager;

            protected:

                /**
                 * Constructor.
                 * Only accessible via the PlatformMananger.
                 */
                Timers(void);

            public:

                /**
                 * Get the granularity of the timer in seconds.
                 *
                 * @return  The timer granularity.
                 */
                f32 GetGranularity(void);

                /**
                 * Creates a timer object.
                 *
                 * @param   Interval    The interval that the timer object signals.
                 * @return  A handle to the timer object.
                 */
                Handle Create(f32 Interval);

                /**
                 * Destroys a timer object.
                 *
                 * @param   hTimer  A handle to the timer object.
                 */
                void Destroy(Handle hTimer);

                /**
                 * Waits for the timer object to signal.
                 *
                 * @param   hTimer  A handle to the timer object.
                 * @param   bWait   Should the call wait for the timer or calculate time immediately. (default =
                 *                  true)
                 * @return  The time elapsed in seconds from the previous signal.
                 */
                f32 Wait(Handle hTimer, bool bWait = true);
        };

        /**
         * Provides OS debugging functionality.
         */
        class Debugging {
                friend class PlatformManager;

            protected:

                /**
                 * Constructor.
                 * Only accessible via the PlatformMananger.
                 */
                Debugging(void);

            public:

                /**
                 * Outputs a message to the debugger.
                 *
                 * @param   pszMessage  The message to output.
                 */
                void OutputMessage(const char* pszMessage);

        };

        /**
         * Provides OS file system functionality.
         * @sa  IPlatform::IProcessor
         * @sa  IPlatform::IProcessor
         */
        class Processor : public IPlatform::IProcessor {
                friend class PlatformManager;

            protected:

                /**
                 * Constructor.
                 * Only accessible via the PlatformMananger.
                 */
                Processor(void);

            public:

                /**
                 * Gets the number processors.
                 * Implementation of IPlatform::IProcessor::GetNumProcessors.
                 *
                 * @return  The number processors.
                 */
                virtual u32 GetNumProcessors(void);

                /**
                 * Affinitize thread to processor.
                 * Implementation of IPlatform::IProcessor::AffinitizeThreadToProcessor.
                 *
                 * @param   ProcessorNumber The processor number.
                 */
                virtual void AffinitizeThreadToProcessor(u32 ProcessorNumber);

            protected:

                u32                             m_TechnologyBitMask;
                std::vector<u32>                m_ProcessorMasks;
        };


    protected:

        FileSystem                          m_FileSystem;
        WindowSystem                        m_WindowSystem;
        Timers                              m_Timers;
        Debugging                           m_Debugging;
        Processor                           m_Processor;


    public:

        /**
         * Gets a reference to the FileSystem class.
         *
         * @return  A reference to the FileSystem class.
         */
        FileSystem& FileSystem(void) {
            return m_FileSystem;
        }

        /**
         * Gets a reference to the WindowSystem class.
         *
         * @return  A reference to the WindowSystem class.
         */
        WindowSystem& WindowSystem(void) {
            return m_WindowSystem;
        }

        /**
         * Gets a reference to the Timers class.
         *
         * @return  A reference to the Timers class.
         */
        Timers& Timers(void) {
            return m_Timers;
        }

        /**
         * Gets a reference to the Debugging class.
         *
         * @return  A reference to the Debugging class.
         */
        Debugging& Debugging(void) {
            return m_Debugging;
        }

        /**
         * Gets the processor.
         * Implementation of IPlatform::IProcessor::Processor.
         *
         * @return  .
         */
        virtual IProcessor& Processor(void) {
            return m_Processor;
        }

        /**
         * Gets the window.
         * Implementation of IPlatform::IWindow::Window.
         *
         * @return  .
         */
        virtual IWindow& Window(void) {
            return m_WindowSystem;
        }

};


DeclareSingleton(PlatformManager);
