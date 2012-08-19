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

#include "DataTypes.h"

/**
 * An interface for getting and setting information about the environment.
 */
class IEnvironment {
    public:

        /**
         * An interface for environment variable functionality.
         */
        class IVariables {
            public:

                /**
                 * Returns the environment variable value as a string.
                 *
                 * @param   pszName         The name of the variable.
                 * @param   pszDefaultValue The value returned if the variable doesn't exist.
                 * @return  The value of the variable.
                 */
                virtual const char* GetAsString(In char* pszName, In char* pszDefaultValue = "") = 0;

                /**
                 * Compares the environment variable against a string, returning true if equal.
                 *
                 * @param   pszName         The name of the variable.
                 * @param   pszCompareValue The value to compare against.
                 * @return  true if the variable exists and is equal to pszCompareValue, false otherwise.
                 */
                virtual bool IsString(In char* pszName, In char* pszCompareValue) = 0;

                /**
                 * Returns the environment variable value as a bool.
                 *
                 * @param   pszName         The name of the variable.
                 * @param   bDefaultValue   The value returned if the variable doesn't exist.
                 * @return  The value of the variable.
                 */
                virtual bool GetAsBool(In char* pszName, In bool bDefaultValue = false) = 0;

                /**
                 * Returns the environment variable value as an int.
                 *
                 * @param   pszName         The name of the variable.
                 * @param   DefaultValue    The value returned if the variable doesn't exist.
                 * @return  The value of the variable.
                 */
                virtual i32 GetAsInt(In char* pszName, In i32 DefaultValue = 0) = 0;

                /**
                 * Returns the environment variable value as a float.
                 *
                 * @param   pszName         The name of the variable.
                 * @param   DefaultValue    The value returned if the variable doesn't exist.
                 * @return  The value of the variable.
                 */
                virtual f32 GetAsFloat(In char* pszName, In f32 DefaultValue = 0.0f) = 0;

                /**
                 * Sets new value for environment variable.
                 *
                 * @param   pszName     The name of the variable.
                 * @param   pszValue    The new value for the variable.
                 */
                virtual void SetValue(In char* pszName, In char* pszValue) = 0;

        };

        /**
         * Returns a reference to the variables class for access to its functionality.
         *
         * @return  A reference to the variables class.
         */
        virtual IVariables& Variables(void) = 0;

        /**
         * An interface for environment runtime functionality.
         */
        class IRuntime {
            public:

                enum Status {
                    Unknown, Running, Paused, NextScene, Quit
                };

                /// <summary>
                ///   Returns the status of execution.
                /// </summary>
                /// <returns>The execution status.</returns>
                virtual Status GetStatus(void) = 0;

                /// <summary>
                ///   Sets the status of execution.
                /// </summary>
                /// <param name="Status">The execution status.</param>
                virtual void SetStatus(Status Status) = 0;
        };

        /**
         * Returns a reference to the runtime class for access to its functionality.
         *
         * @return  A reference to the runtime class.
         */
        virtual IRuntime& Runtime(void) = 0;

};
