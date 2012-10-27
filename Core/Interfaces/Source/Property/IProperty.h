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

#include "boost/function.hpp"
#include "Errors.h"
#include "Proto.h"

/**
 * Interface defining methods to Initialize & Set properties to an object.
 */
class IProperty {

    public:

        /**
         * Constructor.
         */
        IProperty(void);

        /**
         * Virtual Destructor.
         */
        virtual ~IProperty(void);

        /**
         * Sets the properties for this system.
         *
         * @param   properties  Property structure array to get values from.
         * @return  An error code.
         */
        Error setProperties(const ProtoPropertyList &properties);

    protected:

        /**
         * One time initialization function for the system.
         * 
         * @return  An error code.
         */
        virtual Error initialize(void); 

        typedef boost::function<void (ProtoStringList propertiesValue)> PropertySetter;
        typedef std::map<std::string, PropertySetter> PropertySetters;

        PropertySetters         m_propertySetters;
        bool                    m_bInitialized;

};
