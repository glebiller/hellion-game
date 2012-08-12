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

#include "Proto/Common/System.pb.h"
#include "Defines.h"
#include "Debug.h"


#if defined( MSC_COMPILER )
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
#endif

class ISystem;
class IPlatform;
class IEnvironment;
class IService;
class ITaskManager;


/**
 * The System namespace contains meta-data about the various sytems,
 * and various enums, datatypes and helper functions for system types.
 */
namespace System {

    /**
     * The List of components
     */
    enum Components {
        System, Scene, Object, Task
    };
    
    typedef u32 Type;

    /**
     * The Types namespace contains specifics about the system types.
     */
    namespace Types {

        // Disabling the warning for using the non-standard scope operator with enums.
#if defined( MSC_COMPILER )
#pragma warning( push )
#pragma warning( disable : 4482 )
#endif
        // A bit mask of the different systems available.  Not all need to be actual
        // systems.  Custom systems can identify themselves by setting their bit in
        // the upper 16-bits, and can use the MakeCustom() function to make a custom
        // type ID.
        static const u32 Null                   = 0;
        static const u32 Generic                = (1 << SystemProto::Generic);
        static const u32 AI                     = (1 << SystemProto::AI);
        static const u32 Animation              = (1 << SystemProto::Animation);
        static const u32 Audio                  = (1 << SystemProto::Audio);
        static const u32 Geometry               = (1 << SystemProto::Geometry);
        static const u32 Graphic                = (1 << SystemProto::Graphic);
        static const u32 Input                  = (1 << SystemProto::Input);
        static const u32 Network                = (1 << SystemProto::Network);
        static const u32 Physic                 = (1 << SystemProto::Physic);

        /*
        static const u32 Scripting              = (1 << SystemProto::Scripting);
        static const u32 Explosion              = (1 << SystemProto::Explosion);
        static const u32 Water                  = (1 << SystemProto::Water);
        */

        // If you extend this list to add a new system, also update the rest of the type-related
        // lists in this file as well as the PerformanceHints list in the TaskManager.

        static const u32 All                    = static_cast<u32>(-1);
        static const u32 MAX                    = 32;

#if defined( MSC_COMPILER )
#pragma warning( pop )
#endif

        /**
         * Get the index of the system with the given type ID.  Useful for looking up indexed properties.
         * This function works on both predefined and custom system type IDs.
         *
         * @param   SystemType  Type - The type ID of a system.
         * @return  u32 - Index of this system.
         */
        __forceinline u32 GetIndex(const System::Type SystemType) {
            u32 Index = All;
#if defined( MSC_COMPILER )
            _BitScanForward((unsigned long*)&Index, SystemType);
#elif defined( GCC_COMPILER )
            Index = __builtin_ffs(SystemType);
#endif
            return Index;
        };

        /**
         * Gets a type from an index.
         *
         * @param   SystemType  Type of the system.
         * @return  The type.
         */
        __forceinline u32 GetType(const SystemProto::Type SystemType) {
            return (1 << SystemType);
        }

        typedef u32 BitMask;
    }

    typedef u32 Change;

    /**
     * Defines the different changes as a bit mask that the systems can request
     * for other systems to perform. Custom changes are not allowed.
     */
    namespace Changes {
        static const u32 None                   = 0;

        namespace Generic {
            static const u32 CreateObject       = (1 <<  0);
            static const u32 DeleteObject       = (1 <<  1);
            static const u32 ExtendObject       = (1 <<  2);
            static const u32 UnextendObject     = (1 <<  3);
            static const u32 All                = CreateObject | DeleteObject | ExtendObject | UnextendObject;
        }

        /*
        namespace AI
        {
            static const u32 Behavior           = (1 << 25);
            static const u32 Velocity           = (1 << 26);
        }
        */

        namespace Audio {

        }

        namespace Geometry {
            static const u32 Position           = (1 <<  4);
            static const u32 Orientation        = (1 <<  5);
            static const u32 Scale              = (1 <<  6);
            static const u32 All                = Position | Orientation | Scale;
        }

        namespace Graphics {
            static const u32 IndexDecl          = (1 <<  7);
            static const u32 VertexDecl         = (1 <<  8);
            static const u32 IndexBuffer        = (1 <<  9);
            static const u32 VertexBuffer       = (1 << 10);
            static const u32 AABB               = (1 << 11);
            static const u32 AllMesh            = IndexDecl | VertexDecl | AABB | IndexBuffer | VertexBuffer;
            static const u32 ParticlesDecl      = (1 << 12);
            static const u32 Particles          = (1 << 13);
            static const u32 AllParticles       = ParticlesDecl | Particles;
            static const u32 Animation          = (1 << 14);
            static const u32 GUI                = (1 << 15);
            static const u32 All                = AllMesh | AllParticles;
        }

        namespace Input {
            static const u32 Velocity           = (1 << 16);
        }

        namespace Physics {
            static const u32 Position           = (1 << 17);
        }

        /*
        namespace POI
        {
            static const u32 Area               = (1 << 21);
            static const u32 Contact            = (1 << 22);
            static const u32 Target             = (1 << 23);
            static const u32 Intersection       = (1 << 24);
        }

        */

        static const u32 Link                   = (1 << 18);
        static const u32 ParentLink             = (1 << 19);
        static const u32 Custom                 = (static_cast<u32>(1) << 20);

        static const u32 All                    = static_cast<u32>(-1);

        typedef u32 BitMask;
    }

}

/**
 * Function type definitions for communicating with the system DLL.
 */
extern "C"
{
    /**
     * The list of managers interfaces.
     */
    struct ManagerInterfaces {
        IPlatform*          pPlatform;
        IEnvironment*       pEnvironment;
        IService*           pService;
        ITaskManager*       pTask;
    };

#if defined( MSC_COMPILER )

    /**
     * Function definition for calling into a system library for initializing a system.
     */
    typedef void (__stdcall* InitializeSystemLibFunction)(
        ManagerInterfaces* pManagers
    );

    /**
     * Function definition for calling into a system library for creating a system.
     * 
     * @return   The newly created system.
     */
    typedef ISystem* (__stdcall* CreateSystemFunction)(
        Debug::Debugger* p_Debugger
    );

    /**
     * Function definition for calling into a system library for destroying a system.
     * 
     * @param    pSystem The system to destroy.
     */
    typedef void (__stdcall* DestroySystemFunction)(
        ISystem* pSystem
    );

#elif defined( GCC_COMPILER )
    /**
     * Function definition for calling into a system library for initializing a system.
     */
    void __attribute__((stdcall)) InitializeSystemLibFunction(
        ManagerInterfaces* pManagers
    );

    /**
     * Function definition for calling into a system library for creating a system.
     * 
     * @return   The newly created system.
     */
    ISystem* __attribute__((stdcall)) CreateSystemFunction(
        Debug::Debugger* p_Debugger
    );

    /**
     * Function definition for calling into a system library for destroying a system.
     * 
     * @param    pSystem The system to destroy.
     */
    void __attribute__((stdcall)) DestroySystemFunction(
        ISystem* pSystem
    );    
#endif

}
