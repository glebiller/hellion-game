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

#include <boost/function.hpp>
#include <map>
#include <string>

#include "DataTypes.h"
#include "Proto.h"
#include "System/Changes.h"

namespace Math { struct Quaternion; }
namespace Math { struct Vector3; }
namespace Math { struct Vector4; }
namespace Proto { class Property; }

/**
 * Interface defining methods to Initialize & Set properties to an object.
 */
class IProperty {
public:
    
    typedef boost::function<void (Proto::RepeatedString* propertiesValue)> PropertySetter;
    typedef std::map<std::string, PropertySetter> PropertySetters;
    
    typedef boost::function<void (Proto::RepeatedString* propertiesValue)> PropertyGetter;
    typedef std::map<std::string, PropertyGetter> PropertyGetters;

    /**
     * Constructor.
     */
    IProperty(void);

    /**
     * Virtual Destructor.
     */
    virtual ~IProperty(void);

    /**
     * One time initialization function for the system.
     * 
     * @return  An error code.
     */
    virtual Error initialize(void);

    /**
     * Sets the properties for this system.
     *
     * @param   properties Property structure array to get values from.
     */
    void setProperties(const Proto::RepeatedProperty& properties);

    /**
     * Get the properties and their values for this system.
     * 
     * @return The map of properties associated with their values.
     */
    Proto::RepeatedProperty getProperties(void);

    /**
     * Sets simple type.
     *
     * @tparam  T   Generic type parameter.
     * @param   changeType      Type of the change.
     * @param [in,out]  value   If non-null, the value.
     * @param [in,out]  values  If non-null, the values.
     */
    template<typename T>
    void setSimpleType(System::Changes::BitMask changeType, T* value, Proto::RepeatedString* values);

    /**
     * Gets f32.
     *
     * @tparam  T   Generic type parameter.
     * @param [in,out]  value   The value to fill.
     * @param [in,out]  values  If non-null, the values.
     */
    template<typename T>
    void getSimpleType(T* value, Proto::RepeatedString* values);

    /**
     * Sets a string.
     *
     * @param   changeType      Type of the change.
     * @param [in,out]  string  If non-null, the string.
     * @param [in,out]  values  If non-null, the values.
     */
    void setString(System::Changes::BitMask changeType, std::string* string, Proto::RepeatedString* values);

    /**
     * Gets a string.
     *
     * @param [in,out]  string  If non-null, the string.
     * @param [in,out]  values  If non-null, the values.
     */
    void getString(std::string* string, Proto::RepeatedString* values);

    /**
     * Sets vector 3.
     *
     * @param   changeType      Type of the change.
     * @param [in,out]  vector3 If non-null, the third vector.
     * @param [in,out]  values  If non-null, the values.
     */
    void setVector3(System::Changes::BitMask changeType, Math::Vector3* vector3, Proto::RepeatedString* values);

    /**
     * Gets vector 3.
     *
     * @param [in,out]  vector3 If non-null, the third vector.
     * @param [in,out]  values  If non-null, the values.
     */
    void getVector3(Math::Vector3* vector3, Proto::RepeatedString* values);

    /**
     * Sets vector 4.
     *
     * @param   changeType      Type of the change.
     * @param [in,out]  vector4 If non-null, the third vector.
     * @param [in,out]  values  If non-null, the values.
     */
    void setVector4(System::Changes::BitMask changeType, Math::Vector4* vector4, Proto::RepeatedString* values);

    /**
     * Gets vector 4.
     *
     * @param [in,out]  vector4 If non-null, the third vector.
     * @param [in,out]  values  If non-null, the values.
     */
    void getVector4(Math::Vector4* vector4, Proto::RepeatedString* values);

    /**
     * Sets a quaternion.
     *
     * @param   changeType          Type of the change.
     * @param [in,out]  quaternion  If non-null, the quaternion.
     * @param [in,out]  values      If non-null, the values.
     */
    void setQuaternion(System::Changes::BitMask changeType, Math::Quaternion* quaternion, Proto::RepeatedString* values);

    /**
     * Gets a quaternion.
     *
     * @param [in,out]  quaternion  If non-null, the quaternion.
     * @param [in,out]  values      If non-null, the values.
     */
    void getQuaternion(Math::Quaternion* quaternion, Proto::RepeatedString* values);

    /**
     * Method called to notify the object that a property has been changed.
     *
     * @param   uInChangedBits  The u32 bit field that describes the conceptual change with respect
     *                          to the published interests.
     */
    virtual void propertyChanged(System::Changes::BitMask uInChangedBits);

private:
    /**
     * Sets the property for this system.
     *
     * @param   property Property to get values from.
     */
    void setProperty(const Proto::Property& property);

protected:
    bool                        m_bInitialized;
    PropertySetters             m_propertySetters;
    PropertyGetters             m_propertyGetters;
    System::Changes::BitMask    m_modified;

};
