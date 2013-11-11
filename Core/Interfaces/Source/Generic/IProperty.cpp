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

#include "Generic/IProperty.h"

#include <google/protobuf/repeated_field.h>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <utility>

#include "Assert.h"
#include "Errors.h"
#include "MathUtils.h"
#include "Proto/Common.pb.h"
#include "System/Changes.h"

/**
 * @inheritDoc
 */
IProperty::IProperty(void)
    : m_bInitialized(false)
    , m_modified(0) {
    
};

/**
 * @inheritDoc
 */
IProperty::~IProperty(void) {
    
};

/**
 * @inheritDoc
 */
void IProperty::setProperties(const Proto::RepeatedProperty& properties) {
    for (auto property : properties) {
        setProperty(property);
    }
    
    if (m_modified != System::Changes::None) {
        propertyChanged(m_modified);
        m_modified = System::Changes::None;
    }
};

/**
 * @inheritDoc
 */
void IProperty::setProperty(const Proto::Property& property) {
    auto setter = m_propertySetters.find(property.name());
    if (setter == m_propertySetters.end()) {
        //ASSERTMSG1(false, "Parser could not find the property named %s in the list given by the system.", prop->name().c_str());
        return;
    }

    Proto::RepeatedString proto = property.value();
    setter->second(&proto);
};

/**
 * @inheritDoc
 */
Proto::RepeatedProperty IProperty::getProperties(void) {
    Proto::RepeatedProperty propertyList;
    for (auto getter : m_propertyGetters) {
        Proto::Property* property = propertyList.Add();
        property->set_name(getter.first);
        getter.second(property->mutable_value());
    }
    return propertyList;
};

/**
 * @inheritDoc
 */
Error IProperty::initialize(void) {
    ASSERT(!m_bInitialized);
    m_bInitialized = true;
    return Errors::Success;
};

/**
 * @inheritDoc
 */
template<typename T> void IProperty::setSimpleType(System::Changes::BitMask changeType, T* value, Proto::RepeatedString* values) {
    auto stringValue = values->begin();
    *value = boost::lexical_cast<T>(*stringValue);
    m_modified &= changeType;
};

template void IProperty::setSimpleType<bool>(System::Changes::BitMask changeType, bool* value, Proto::RepeatedString* values);
template void IProperty::setSimpleType<f32>(System::Changes::BitMask changeType, f32* value, Proto::RepeatedString* values);

/**
 * @inheritDoc
 */
template<typename T> void IProperty::getSimpleType(T* value, Proto::RepeatedString* values) {
    std::string* stringValue = nullptr;
    stringValue = values->Add();
    stringValue->append(boost::lexical_cast<std::string>(*value));
};

template void IProperty::getSimpleType<bool>(bool* value, Proto::RepeatedString* values);
template void IProperty::getSimpleType<f32>(f32* value, Proto::RepeatedString* values);

/**
 * @inheritDoc
 */
void IProperty::setString(System::Changes::BitMask changeType, std::string* string, Proto::RepeatedString* values) {
    auto value = values->begin();
    string->assign(*value);
    m_modified |= changeType;
};

/**
 * @inheritDoc
 */
void IProperty::getString(std::string* string, Proto::RepeatedString* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(*string);
};

/**
 * @inheritDoc
 */
void IProperty::setVector3(System::Changes::BitMask changeType, Math::Vector3* vector3, Proto::RepeatedString* values) {
    auto value = values->begin();
    vector3->x = boost::lexical_cast<f32>(*value);
    vector3->y = boost::lexical_cast<f32>(*(++value));
    vector3->z = boost::lexical_cast<f32>(*(++value));
    m_modified |= changeType;
};

/**
 * @inheritDoc
 */
void IProperty::getVector3(Math::Vector3* vector3, Proto::RepeatedString* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->z));
};

/**
 * @inheritDoc
 */
void IProperty::setVector4(System::Changes::BitMask changeType, Math::Vector4* vector4, Proto::RepeatedString* values) {
    auto value = values->begin();
    vector4->x = boost::lexical_cast<f32>(*value);
    vector4->y = boost::lexical_cast<f32>(*(++value));
    vector4->z = boost::lexical_cast<f32>(*(++value));
    vector4->w = boost::lexical_cast<f32>(*(++value));
    m_modified |= changeType;
};

/**
 * @inheritDoc
 */
void IProperty::getVector4(Math::Vector4* vector4, Proto::RepeatedString* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector4->x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector4->y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector4->z));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector4->w));
};

/**
 * @inheritDoc
 */
void IProperty::setQuaternion(System::Changes::BitMask changeType, Math::Quaternion* quaternion, Proto::RepeatedString* values) {
    auto value = values->begin();
    quaternion->x = boost::lexical_cast<f32>(*value);
    quaternion->y = boost::lexical_cast<f32>(*(++value));
    quaternion->z = boost::lexical_cast<f32>(*(++value));
    quaternion->w = boost::lexical_cast<f32>(*(++value));
    m_modified |= changeType;
};

/**
 * @inheritDoc
 */
void IProperty::getQuaternion(Math::Quaternion* quaternion, Proto::RepeatedString* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->z));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->w));
};

/**
 * @inheritDoc
 */
void IProperty::propertyChanged(System::Changes::BitMask uInChangedBits) {
    // NOOP
};
